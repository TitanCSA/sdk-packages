// node-interception adaptation. Original wrapper: Rami Sabbagh, LGPL-3.0-or-later.
#include <windows.h>
#include <napi.h>
#include "interception.h"
#include <atomic>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#ifdef INTERCEPTOR_TEST_BACKEND
#include "../test/native/FakeSdk.h"
#endif
using namespace Napi;
namespace {
struct State {
    InterceptionContext handle=nullptr;std::atomic<bool> waiting{false},closed{false};
    void Close(){if(!closed.exchange(true)&&handle)interception_destroy_context(handle);}
    ~State(){Close();}
};
struct Environment {std::vector<std::weak_ptr<State>> states;void Close(){for(auto& w:states)if(auto s=w.lock())s->Close();}};
constexpr napi_type_tag tag={0x1c12c81a5dc44d49ULL,0xa533a5bea61310bcULL};
std::shared_ptr<State> Get(const CallbackInfo& i) {
    bool matches=false;
    if(!i.Length()||!i[0].IsObject()||napi_check_object_type_tag(i.Env(),i[0],&tag,&matches)!=napi_ok||!matches)
        throw TypeError::New(i.Env(),"Expected a context created by this addon");
    return *i[0].As<Object>().Get("_native").As<External<std::shared_ptr<State>>>().Data();
}
void Check(Env e){if(interceptor_error()){auto error=Error::New(e,interceptor_error_message());error.Set("win32Code",Number::New(e,interceptor_error()));throw error;}}
std::shared_ptr<State> Open(const CallbackInfo& i){auto s=Get(i);if(s->closed)throw Error::New(i.Env(),"The context is destroyed");return s;}
double Arg(const CallbackInfo& i,size_t n,double min,double max,const char* name){
    if(n>=i.Length()||!i[n].IsNumber())throw TypeError::New(i.Env(),name);
    auto v=i[n].As<Number>().DoubleValue();if(!std::isfinite(v)||std::floor(v)!=v||v<min||v>max)throw RangeError::New(i.Env(),name);return v;
}
int DeviceArg(const CallbackInfo& i){return static_cast<int>(Arg(i,1,1,20,"Device ID must be an integer from 1 to 20"));}
Value Create(const CallbackInfo& i){
    auto s=std::make_shared<State>();s->handle=interception_create_context();Check(i.Env());
    auto obj=Object::New(i.Env());
    obj.DefineProperty(PropertyDescriptor::Value("_native",External<std::shared_ptr<State>>::New(i.Env(),new std::shared_ptr<State>(s),[](Env,std::shared_ptr<State>* p){(*p)->Close();delete p;}),napi_default));
    napi_type_tag_object(i.Env(),obj,&tag);auto& states=i.Env().GetInstanceData<Environment>()->states;
    states.erase(std::remove_if(states.begin(),states.end(),[](const std::weak_ptr<State>& entry){return entry.expired();}),states.end());states.push_back(s);return obj;
}
void Destroy(const CallbackInfo& i){Get(i)->Close();}
Value Destroyed(const CallbackInfo& i){return Boolean::New(i.Env(),Get(i)->closed);}
void SetFilter(const CallbackInfo& i){auto s=Open(i);if(i.Length()<3||!i[1].IsString())throw TypeError::New(i.Env(),"Filter type must be keyboard or mouse");auto type=i[1].As<String>().Utf8Value();if(type!="keyboard"&&type!="mouse")throw TypeError::New(i.Env(),"Filter type must be keyboard or mouse");auto f=static_cast<unsigned short>(Arg(i,2,0,65535,"Filter must be a 16-bit integer"));interception_set_filter(s->handle,type=="keyboard"?interception_is_keyboard:interception_is_mouse,f);Check(i.Env());}
Value GetFilter(const CallbackInfo& i){auto s=Open(i);auto f=interception_get_filter(s->handle,DeviceArg(i));Check(i.Env());return Number::New(i.Env(),f);}
Value GetPrecedence(const CallbackInfo& i){auto s=Open(i);auto p=interception_get_precedence(s->handle,DeviceArg(i));Check(i.Env());return Number::New(i.Env(),p);}
void SetPrecedence(const CallbackInfo& i){auto s=Open(i);int id=DeviceArg(i),p=static_cast<int>(Arg(i,2,INT32_MIN,INT32_MAX,"Precedence must be a signed integer"));interception_set_precedence(s->handle,id,p);Check(i.Env());}
std::shared_ptr<State> StartWait(const CallbackInfo& i){auto s=Open(i);if(s->waiting.exchange(true))throw Error::New(i.Env(),"Only one wait operation per context may be pending");return s;}
struct AsyncWaitRequest {
    Promise::Deferred deferred;
    std::shared_ptr<State> state;
    napi_threadsafe_function function=nullptr;
    HANDLE ready=nullptr,wait=nullptr;
    std::atomic<bool> completed{false};
    bool cleanupCalled=false,cleanupRegistered=false,delivered=false;
    napi_env environment;
    DWORD error=0;int device=0;std::string message;
    AsyncWaitRequest(Napi::Env env,std::shared_ptr<State> value):deferred(Promise::Deferred::New(env)),state(std::move(value)),environment(env){}
    ~AsyncWaitRequest(){
        if(cleanupRegistered&&!cleanupCalled)napi_remove_env_cleanup_hook(environment,Cleanup,this);
        if(wait)UnregisterWaitEx(wait,INVALID_HANDLE_VALUE);
        if(ready)CloseHandle(ready);
        if(!delivered)state->waiting=false;
    }
    static void Cleanup(void* data){
        auto* request=static_cast<AsyncWaitRequest*>(data);request->cleanupCalled=true;
        if(request->wait){UnregisterWaitEx(request->wait,INVALID_HANDLE_VALUE);request->wait=nullptr;}
        request->state->Close();
        if(!request->completed.exchange(true))napi_release_threadsafe_function(request->function,napi_tsfn_abort);
    }
    static void Finalize(napi_env,void* data,void*){delete static_cast<AsyncWaitRequest*>(data);}
    static void Deliver(napi_env env,napi_value,void*,void* data){
        auto* request=static_cast<AsyncWaitRequest*>(data);request->delivered=true;request->state->waiting=false;
        if(!env || request->cleanupCalled)return;
        try {
            Napi::Env e(env);
            if(request->error&&!request->state->closed){auto error=Error::New(e,request->message.empty()?"Native input wait failed":request->message);error.Set("win32Code",Number::New(e,request->error));request->deferred.Reject(error.Value());}
            else request->deferred.Resolve(request->device&&!request->state->closed?Number::New(e,request->device).As<Value>():e.Null());
        }catch(...){napi_throw_error(env,nullptr,"Input completion failed");}
    }
    void Finish(bool timedOut){
        if(completed.exchange(true))return;
        try{if(!timedOut&&!state->closed){device=interception_wait_with_timeout(state->handle,0);error=interceptor_error();if(error)message=interceptor_error_message();}}
        catch(...){error=ERROR_NOT_ENOUGH_MEMORY;}
        napi_call_threadsafe_function(function,this,napi_tsfn_nonblocking);
        napi_release_threadsafe_function(function,napi_tsfn_release);
    }
    static void CALLBACK Signaled(void* data,BOOLEAN timedOut){static_cast<AsyncWaitRequest*>(data)->Finish(timedOut!=FALSE);}
};
unsigned long Timeout(const CallbackInfo& i,bool timed){return timed?static_cast<unsigned long>(Arg(i,1,0,4294967294.0,"Timeout must be an integer from 0 to 4294967294 ms")):0xffffffffUL;}
Value WaitSync(const CallbackInfo& i,bool timed){auto t=Timeout(i,timed);auto s=StartWait(i);int id=interception_wait_with_timeout(s->handle,t);s->waiting=false;Check(i.Env());return id?Number::New(i.Env(),id).As<Value>():i.Env().Null();}
Value WaitAsync(const CallbackInfo& i,bool timed){
    auto timeout=Timeout(i,timed);auto state=StartWait(i);
    AsyncWaitRequest* request=nullptr;
    try {
        request=new AsyncWaitRequest(i.Env(),state);
        request->ready=static_cast<HANDLE>(interceptor_duplicate_ready_event(state->handle));Check(i.Env());
        auto name=String::New(i.Env(),"Interceptor input readiness");
        auto status=napi_create_threadsafe_function(i.Env(),nullptr,nullptr,name,1,1,request,AsyncWaitRequest::Finalize,nullptr,AsyncWaitRequest::Deliver,&request->function);
        if(status!=napi_ok){delete request;request=nullptr;throw Error::New(i.Env(),"Cannot create input completion callback");}
        // From here the TSFN finalizer owns request, including on errors.
        auto promise=request->deferred.Promise();
        if(!RegisterWaitForSingleObject(&request->wait,request->ready,AsyncWaitRequest::Signaled,request,timeout,WT_EXECUTEONLYONCE)){
            request->error=GetLastError();request->message="Register input readiness wait failed";request->Finish(true);return promise;
        }
        status=napi_add_env_cleanup_hook(i.Env(),AsyncWaitRequest::Cleanup,request);
        if(status!=napi_ok){request->error=ERROR_GEN_FAILURE;request->message="Cannot register input wait cleanup";UnregisterWaitEx(request->wait,INVALID_HANDLE_VALUE);request->wait=nullptr;request->Finish(true);}
        else request->cleanupRegistered=true;
        return promise;
    }catch(...){if(request&&!request->function)delete request;state->waiting=false;throw;}
}
Value Wait(const CallbackInfo& i){return WaitSync(i,false);} Value WaitTimeout(const CallbackInfo& i){return WaitSync(i,true);}
Value AsyncWait(const CallbackInfo& i){return WaitAsync(i,false);} Value AsyncTimeout(const CallbackInfo& i){return WaitAsync(i,true);}
double Field(const Object& o,const char* field,double min,double max,bool optional=false){auto v=o.Get(field);if(optional&&v.IsUndefined())return 0;if(!v.IsNumber())throw TypeError::New(o.Env(),std::string("Missing numeric field: ")+field);auto d=v.As<Number>().DoubleValue();if(!std::isfinite(d)||std::floor(d)!=d||d<min||d>max)throw RangeError::New(o.Env(),std::string("Field out of range: ")+field);return d;}
Value Send(const CallbackInfo& i){
    auto s=Open(i);auto id=DeviceArg(i);if(i.Length()<3||!i[2].IsObject())throw TypeError::New(i.Env(),"Expected an input stroke");auto o=i[2].As<Object>();auto type=o.Get("type");if(!type.IsString()||type.As<String>().Utf8Value()!=(id<=10?"keyboard":"mouse"))throw TypeError::New(i.Env(),"Stroke type does not match its destination device");
    alignas(InterceptionMouseStroke) InterceptionStroke stroke{};
    if(id<=10){auto& k=*reinterpret_cast<InterceptionKeyStroke*>(stroke);k.code=static_cast<unsigned short>(Field(o,"code",0,65535));k.state=static_cast<unsigned short>(Field(o,"state",0,65535));k.information=static_cast<unsigned>(Field(o,"information",0,UINT32_MAX,true));}
    else{auto& m=*reinterpret_cast<InterceptionMouseStroke*>(stroke);m.state=static_cast<unsigned short>(Field(o,"state",0,65535));m.flags=static_cast<unsigned short>(Field(o,"flags",0,65535));m.rolling=static_cast<short>(Field(o,"rolling",INT16_MIN,INT16_MAX));m.x=static_cast<int>(Field(o,"x",INT32_MIN,INT32_MAX));m.y=static_cast<int>(Field(o,"y",INT32_MIN,INT32_MAX));m.information=static_cast<unsigned>(Field(o,"information",0,UINT32_MAX,true));}
    auto sent=interception_send(s->handle,id,&stroke,1);Check(i.Env());return Boolean::New(i.Env(),sent==1);
}
Value Receive(const CallbackInfo& i){auto s=Open(i);auto id=DeviceArg(i);alignas(InterceptionMouseStroke) InterceptionStroke stroke{};auto count=interception_receive(s->handle,id,&stroke,1);Check(i.Env());if(!count)return i.Env().Null();auto o=Object::New(i.Env());if(id<=10){auto& k=*reinterpret_cast<InterceptionKeyStroke*>(stroke);o.Set("type","keyboard");o.Set("code",k.code);o.Set("state",k.state);o.Set("information",k.information);}else{auto& m=*reinterpret_cast<InterceptionMouseStroke*>(stroke);o.Set("type","mouse");o.Set("state",m.state);o.Set("flags",m.flags);o.Set("rolling",m.rolling);o.Set("x",m.x);o.Set("y",m.y);o.Set("information",m.information);}return o;}
Value Hardware(const CallbackInfo& i){auto s=Open(i);auto id=DeviceArg(i);char16_t text[512]{};auto bytes=interception_get_hardware_id(s->handle,id,text,sizeof(text));Check(i.Env());return bytes?String::New(i.Env(),text,(bytes/2)-1).As<Value>():i.Env().Null();}
Value Keyboard(const CallbackInfo& i){return Boolean::New(i.Env(),interception_is_keyboard(static_cast<int>(Arg(i,0,INT32_MIN,INT32_MAX,"Expected an integer device ID"))));}
Value Mouse(const CallbackInfo& i){return Boolean::New(i.Env(),interception_is_mouse(static_cast<int>(Arg(i,0,INT32_MIN,INT32_MAX,"Expected an integer device ID"))));}
Value Invalid(const CallbackInfo& i){return Boolean::New(i.Env(),interception_is_invalid(static_cast<int>(Arg(i,0,INT32_MIN,INT32_MAX,"Expected an integer device ID"))));}
Value Installed(const CallbackInfo& i){auto o=Object::New(i.Env());o.Set("installed",interceptor_installed()!=0);o.Set("error",Number::New(i.Env(),interceptor_error()));o.Set("message",String::New(i.Env(),interceptor_error_message()));return o;}
Object Init(Env env,Object exports){auto* e=new Environment;env.SetInstanceData(e);env.AddCleanupHook([](Environment* e){e->Close();},e);
#ifdef INTERCEPTOR_TEST_BACKEND
    exports.Set("testPushKey",Function::New(env,[](const CallbackInfo& i){FakePushKey(static_cast<unsigned>(Arg(i,0,1,1000,"device")),static_cast<unsigned>(Arg(i,1,0,65535,"code")),static_cast<unsigned>(Arg(i,2,0,65535,"flags")));}));
    exports.Set("testPushMouse",Function::New(env,[](const CallbackInfo& i){FakePushMouse(static_cast<unsigned>(Arg(i,0,1,1000,"device")),static_cast<int>(Arg(i,1,INT32_MIN,INT32_MAX,"x")),static_cast<int>(Arg(i,2,INT32_MIN,INT32_MAX,"y")));}));
    exports.Set("testReset",Function::New(env,[](const CallbackInfo&){FakeReset();}));
#endif
#define EXPORT(name,fn) exports.Set(name,Function::New(env,fn,name))
    EXPORT("createContext",Create);EXPORT("destroyContext",Destroy);EXPORT("isContextDestroyed",Destroyed);EXPORT("setFilter",SetFilter);EXPORT("getFilter",GetFilter);EXPORT("getPrecedence",GetPrecedence);EXPORT("setPrecedence",SetPrecedence);EXPORT("wait",Wait);EXPORT("waitWithTimeout",WaitTimeout);EXPORT("waitAsync",AsyncWait);EXPORT("waitWithTimeoutAsync",AsyncTimeout);EXPORT("send",Send);EXPORT("receive",Receive);EXPORT("getHardwareId",Hardware);EXPORT("isKeyboard",Keyboard);EXPORT("isMouse",Mouse);EXPORT("isInvalid",Invalid);EXPORT("installationStatus",Installed);
#undef EXPORT
    return exports;}
}
NODE_API_MODULE(node_interceptor,Init)


#define IC_SDK_INTERNAL
#include "SDK/Interceptor.h"
#include "interception.h"
#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstring>
#include <deque>
#include <limits>
#include <thread>
#include <unordered_map>
#include <system_error>

namespace {
thread_local DWORD lastError=0;
thread_local std::string lastMessage;
void Error(DWORD code,const char* action="Interceptor") noexcept {
    lastError=code;
    if(!code){lastMessage.clear();SetLastError(0);return;}
    try {
        char text[1024]{};
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,nullptr,code,
            MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),text,sizeof(text),nullptr);
        lastMessage=std::string(action)+": "+text+" (Windows error "+std::to_string(code)+")";
    } catch(...) {lastMessage.clear();}
    SetLastError(code);
}
void Exception() noexcept {
    try {throw;} catch(const std::system_error& e){Error(static_cast<DWORD>(e.code().value()),e.what());}
    catch(const std::bad_alloc&){Error(ERROR_NOT_ENOUGH_MEMORY);}
    catch(const std::exception& e){Error(ERROR_INVALID_PARAMETER,e.what());}
    catch(...){Error(ERROR_GEN_FAILURE);}
}
struct Handle {
    HANDLE value=nullptr;
    ~Handle(){if(value && value!=INVALID_HANDLE_VALUE)CloseHandle(value);}
    Handle()=default;Handle(const Handle&)=delete;Handle& operator=(const Handle&)=delete;
};
struct Packet {
    int device=0;std::uint64_t order=0;
    InterceptionKeyStroke key{};InterceptionMouseStroke mouse{};
};
struct Slot {
    Interceptor::Device_Info info{};bool present=false;
    std::deque<Packet> packets;
};
struct Session {
    Handle ownership,stop,ready;
    Interceptor::Context_Result capture;
    std::mutex state,native;
    std::condition_variable changed;
    Slot slots[21];unsigned short filters[21]{};
    std::thread worker;
    bool closed=false,interrupted=false;DWORD fault=0;
    std::size_t queued=0;
    std::uint64_t order=0;
    Session() {
        // This edition has one capture owner. Never silently steal another bridge.
        ownership.value=CreateMutexW(nullptr,FALSE,L"Global\\TitanCSA.InterceptorKeyboardMouse.LanguageBridge");
        if(!ownership.value)throw std::system_error(GetLastError(),std::system_category(),"Create capture ownership marker");
        if(GetLastError()==ERROR_ALREADY_EXISTS)throw std::system_error(ERROR_BUSY,std::system_category(),"Another AutoHotkey/Node bridge is running; close it first");
        stop.value=CreateEventW(nullptr,TRUE,FALSE,nullptr);
        if(!stop.value)throw std::system_error(GetLastError(),std::system_category(),"Create cancellation event");
        ready.value=CreateEventW(nullptr,TRUE,FALSE,nullptr);
        if(!ready.value)throw std::system_error(GetLastError(),std::system_category(),"Create readiness event");
        auto init=Interceptor::Init_AllDevices();
        if(!init.ready || init.error || init.reboot_required)
            throw std::system_error(init.error?init.error:init.reboot_required?ERROR_SUCCESS_REBOOT_REQUIRED:ERROR_NOT_READY,
                std::system_category(),"Driver initialization failed; run Setup and restart Windows if requested");
        capture=Interceptor::Create_Context();
        Refresh();
        worker=std::thread([this]{Pump();});
    }
    ~Session(){Stop();}
    void Stop() noexcept {
        {std::lock_guard<std::mutex> lock(state);closed=true;}
        SetEvent(stop.value);SetEvent(ready.value);changed.notify_all();
        if(worker.joinable())worker.join();
        std::lock_guard<std::mutex> lock(native);
        Interceptor::Destroy_Context(capture);
    }
    void Refresh() {
        std::lock_guard<std::mutex> io(native);
        auto devices=Interceptor::Get_Devices(*capture);
        std::lock_guard<std::mutex> lock(state);
        for(auto& slot:slots)slot.present=false;
        for(const auto& d:devices) {
            if(d.type!=IC_TYPE_KEYBOARD && d.type!=IC_TYPE_MOUSE)continue;
            const int start=d.type==IC_TYPE_KEYBOARD?1:11;
            int found=0,empty=0;
            for(int i=start;i<start+10;++i) {
                if(slots[i].info.id==d.id && slots[i].info.instance_token==d.instance_token){found=i;break;}
                if(!slots[i].info.id && !empty)empty=i;
            }
            if(!found)found=empty;
            if(!found)throw std::system_error(ERROR_TOO_MANY_NAMES,std::system_category(),"Compatibility IDs exhausted; recreate the context (10 devices per family)");
            slots[found].info=d;slots[found].present=true;
        }
    }
    int Map(unsigned type,unsigned id) {
        std::lock_guard<std::mutex> lock(state);
        for(int i=1;i<=20;++i)if(slots[i].present && slots[i].info.type==type && slots[i].info.id==id)return i;
        return 0;
    }
    bool Matches(int id,const Packet& packet) const noexcept {
        const auto filter=filters[id];
        if(id<=10) {
            const unsigned flags=packet.key.state;
            return (filter&((flags&1)?2:1)) || (filter&((flags&~1u)<<1));
        }
        return (filter&packet.mouse.state) || ((filter&0x1000) &&
            (packet.mouse.x || packet.mouse.y || (packet.mouse.flags&1)));
    }
    template<class T> void Route(std::vector<T>& batch) {
        constexpr bool key=std::is_same_v<T,Interceptor::Keyboard_Input>;
        for(auto& input:batch) {
            int id=Map(key?IC_TYPE_KEYBOARD:IC_TYPE_MOUSE,input.device);
            if(!id){Refresh();id=Map(key?IC_TYPE_KEYBOARD:IC_TYPE_MOUSE,input.device);}
            bool selected=false;
            if(id) {
                Packet packet{};packet.device=id;
                if constexpr(key)packet.key={input.stroke.make_code,input.stroke.flags,input.stroke.extra_information};
                else packet.mouse={input.stroke.button_flags,input.stroke.flags,static_cast<short>(input.stroke.button_data),input.stroke.x,input.stroke.y,input.stroke.extra_information};
                std::lock_guard<std::mutex> lock(state);
                if(!closed && Matches(id,packet)) {
                    if(queued>=8192)throw std::system_error(ERROR_BUFFER_OVERFLOW,std::system_category(),"Script did not consume its input queue; capture released");
                    packet.order=++order;slots[id].packets.push_back(packet);++queued;selected=true;SetEvent(ready.value);
                }
            }
            if(selected)changed.notify_all();
            else {
                std::lock_guard<std::mutex> io(native);
                auto sent=Interceptor::Send_Input(*capture,&input,1);
                if(sent.error || sent.accepted!=1)throw std::system_error(sent.error?sent.error:ERROR_WRITE_FAULT,std::system_category(),"Forward unfiltered input");
            }
        }
    }
    void Pump() noexcept {
        try {
            std::vector<Interceptor::Keyboard_Input> keys;keys.reserve(256);
            std::vector<Interceptor::Mouse_Input> mice;mice.reserve(256);
            while(WaitForSingleObject(stop.value,0)!=WAIT_OBJECT_0) {
                const auto streams=Interceptor::Wait_For_Input(*capture,INFINITE,stop.value);
                // Read/send share a lock; waiting never holds it, so injection
                // remains possible while the physical devices are idle.
                if(streams&1) {
                    {std::lock_guard<std::mutex> io(native);Interceptor::Get_Keyboard_Input(*capture,keys,256);}
                    Route(keys);
                }
                if(streams&2) {
                    {std::lock_guard<std::mutex> io(native);Interceptor::Get_Mouse_Input(*capture,mice,256);}
                    Route(mice);
                }
            }
        } catch(const std::system_error& e){std::lock_guard<std::mutex> lock(state);fault=static_cast<DWORD>(e.code().value());}
        catch(const std::bad_alloc&){std::lock_guard<std::mutex> lock(state);fault=ERROR_NOT_ENOUGH_MEMORY;}
        catch(...){std::lock_guard<std::mutex> lock(state);fault=ERROR_GEN_FAILURE;}
        {std::lock_guard<std::mutex> lock(state);closed=true;}
        SetEvent(ready.value);
        {std::lock_guard<std::mutex> io(native);Interceptor::Destroy_Context(capture);}
        changed.notify_all();
    }
    int Ready() const noexcept {
        int id=0;std::uint64_t first=UINT64_MAX;
        for(int i=1;i<=20;++i)if(!slots[i].packets.empty() && slots[i].packets.front().order<first) {
            id=i;first=slots[i].packets.front().order;
        }
        return id;
    }
};
struct Registry {std::mutex mutex;std::unordered_map<std::uintptr_t,std::shared_ptr<Session>> sessions;std::uintptr_t next=1;};
// Lifetime is explicitly managed by destroy_context. Avoid joining threads from
// DLL_PROCESS_DETACH under the Windows loader lock. The OS releases process state.
Registry& RegistryState(){static auto* registry=new Registry;return *registry;}
std::shared_ptr<Session> Lookup(InterceptionContext handle) {
    auto& r=RegistryState();std::lock_guard<std::mutex> lock(r.mutex);
    auto found=r.sessions.find(reinterpret_cast<std::uintptr_t>(handle));
    if(found==r.sessions.end())throw std::system_error(ERROR_INVALID_HANDLE,std::system_category(),"Capture is closed");
    return found->second;
}
bool Device(int id){return id>=1 && id<=20;}
void Check(Session& s) {if(s.closed)throw std::system_error(s.fault?s.fault:ERROR_OPERATION_ABORTED,std::system_category(),"Capture stopped; close/reopen after resolving the error");}
}
extern "C" {
unsigned long interceptor_error(){return lastError;}
const char* interceptor_error_message(){return lastMessage.c_str();}
int interceptor_installed(){try{auto c=Interceptor::Get_Capabilities();Error(0);return c.abi_version==IC_VERSION;}catch(...){Exception();return 0;}}
InterceptionContext interception_create_context(){try {
    auto session=std::make_shared<Session>();auto& r=RegistryState();std::lock_guard<std::mutex> lock(r.mutex);
    const auto id=r.next++;r.sessions.emplace(id,std::move(session));Error(0);return reinterpret_cast<void*>(id);
}catch(...){Exception();return nullptr;}}
void interception_destroy_context(InterceptionContext h){try {
    std::shared_ptr<Session> session;auto& r=RegistryState();
    {std::lock_guard<std::mutex> lock(r.mutex);auto it=r.sessions.find(reinterpret_cast<std::uintptr_t>(h));if(it==r.sessions.end()){Error(ERROR_INVALID_HANDLE);return;}session=std::move(it->second);r.sessions.erase(it);}
    session->Stop();Error(0);
}catch(...){Exception();}}
void interceptor_cancel(InterceptionContext h){try{auto s=Lookup(h);SetEvent(s->stop.value);{std::lock_guard<std::mutex> lock(s->state);s->closed=true;}s->changed.notify_all();Error(0);}catch(...){Exception();}}
void interceptor_wake(InterceptionContext h){try{auto s=Lookup(h);{std::lock_guard<std::mutex> lock(s->state);s->interrupted=true;}s->changed.notify_all();Error(0);}catch(...){Exception();}}
void* interceptor_duplicate_ready_event(InterceptionContext h){try{auto s=Lookup(h);HANDLE duplicate=nullptr;if(!DuplicateHandle(GetCurrentProcess(),s->ready.value,GetCurrentProcess(),&duplicate,SYNCHRONIZE,FALSE,0))throw std::system_error(GetLastError(),std::system_category(),"Duplicate readiness event");Error(0);return duplicate;}catch(...){Exception();return nullptr;}}
int interception_is_keyboard(int id){return id>=1&&id<=10;}
int interception_is_mouse(int id){return id>=11&&id<=20;}
int interception_is_invalid(int id){return !Device(id);}
int interception_get_precedence(InterceptionContext h,int id){try{auto s=Lookup(h);if(!Device(id))throw std::invalid_argument("Invalid device ID");Error(0);return 0;}catch(...){Exception();return 0;}}
void interception_set_precedence(InterceptionContext h,int id,int value){try{auto s=Lookup(h);if(!Device(id))throw std::invalid_argument("Invalid device ID");if(value)throw std::system_error(ERROR_NOT_SUPPORTED,std::system_category(),"This driver supports one capture owner; cross-context precedence is unavailable");Error(0);}catch(...){Exception();}}
unsigned short interception_get_filter(InterceptionContext h,int id){try{auto s=Lookup(h);if(!Device(id))throw std::invalid_argument("Invalid device ID");std::lock_guard<std::mutex> lock(s->state);Check(*s);Error(0);return s->filters[id];}catch(...){Exception();return 0;}}
void interception_set_filter(InterceptionContext h,InterceptionPredicate predicate,unsigned short filter){try {
    if(!predicate)throw std::invalid_argument("Missing device predicate");auto s=Lookup(h);
    bool selected[21]{};for(int i=1;i<=20;++i)selected[i]=predicate(i)!=0;
    std::lock_guard<std::mutex> lock(s->state);Check(*s);
    for(int i=1;i<=20;++i)if(selected[i])s->filters[i]=filter;
    Error(0);
}catch(...){Exception();}}
int interception_wait_with_timeout(InterceptionContext h,unsigned long ms){try {
    auto s=Lookup(h);std::unique_lock<std::mutex> lock(s->state);
    auto predicate=[&]{return s->closed || s->Ready() || s->interrupted;};
    if(ms==INFINITE)s->changed.wait(lock,predicate);
    else if(!s->changed.wait_for(lock,std::chrono::milliseconds(ms),predicate)){Error(0);return 0;}
    Check(*s);s->interrupted=false;Error(0);return s->Ready();
}catch(...){Exception();return 0;}}
int interception_wait(InterceptionContext h){return interception_wait_with_timeout(h,INFINITE);}
int interception_receive(InterceptionContext h,int id,InterceptionStroke* data,unsigned int count){try {
    if(!Device(id) || (count&&!data) || count>8192)throw std::invalid_argument("Invalid receive arguments");
    auto s=Lookup(h);std::lock_guard<std::mutex> lock(s->state);Check(*s);
    unsigned int done=0;auto* bytes=reinterpret_cast<unsigned char*>(data);
    while(done<count && !s->slots[id].packets.empty()) {
        const auto& p=s->slots[id].packets.front();
        if(id<=10)std::memcpy(bytes+done*sizeof(p.key),&p.key,sizeof(p.key));
        else std::memcpy(bytes+done*sizeof(p.mouse),&p.mouse,sizeof(p.mouse));
        s->slots[id].packets.pop_front();--s->queued;++done;
    }
    if(!s->queued&&!s->closed)ResetEvent(s->ready.value);
    Error(0);return done;
}catch(...){Exception();return 0;}}
int interception_send(InterceptionContext h,int id,const InterceptionStroke* data,unsigned int count){try {
    if(!Device(id) || (count&&!data) || count>8192)throw std::invalid_argument("Invalid send arguments");
    auto s=Lookup(h);std::lock_guard<std::mutex> io(s->native);Interceptor::Device_Info source;
    {std::lock_guard<std::mutex> lock(s->state);Check(*s);if(!s->slots[id].present)throw std::system_error(ERROR_DEVICE_NOT_CONNECTED,std::system_category(),"Device disconnected");source=s->slots[id].info;}
    Interceptor::Send_Result sent{};
    if(id<=10){std::vector<Interceptor::Keyboard_Input> inputs(count);const auto* keys=reinterpret_cast<const InterceptionKeyStroke*>(data);
        for(unsigned i=0;i<count;++i){inputs[i].device=source.id;inputs[i].stroke.make_code=keys[i].code;inputs[i].stroke.flags=keys[i].state;inputs[i].stroke.extra_information=keys[i].information;}
        sent=Interceptor::Send_Input(*s->capture,inputs.data(),inputs.size());
    }else{std::vector<Interceptor::Mouse_Input> inputs(count);const auto* mice=reinterpret_cast<const InterceptionMouseStroke*>(data);
        for(unsigned i=0;i<count;++i){auto& stroke=inputs[i].stroke;inputs[i].device=source.id;stroke.flags=mice[i].flags;stroke.button_flags=mice[i].state;stroke.button_data=static_cast<unsigned short>(mice[i].rolling);stroke.x=mice[i].x;stroke.y=mice[i].y;stroke.extra_information=mice[i].information;}
        sent=Interceptor::Send_Input(*s->capture,inputs.data(),inputs.size());
    }
    Error(sent.error);return static_cast<int>(sent.accepted);
}catch(...){Exception();return 0;}}
unsigned int interception_get_hardware_id(InterceptionContext h,int id,void* buffer,unsigned int capacity){try {
    if(!Device(id)||(!buffer&&capacity))throw std::invalid_argument("Invalid hardware ID arguments");auto s=Lookup(h);s->Refresh();
    std::lock_guard<std::mutex> lock(s->state);Check(*s);const auto& slot=s->slots[id];if(!slot.present){Error(0);return 0;}
    std::wstring value(reinterpret_cast<const wchar_t*>(slot.info.hardware_id),wcsnlen(reinterpret_cast<const wchar_t*>(slot.info.hardware_id),256));
    if(value.empty())value=L"INTERCEPTOR\\SOURCE_"+std::to_wstring(slot.info.id);
    const auto bytes=static_cast<unsigned int>((value.size()+1)*sizeof(wchar_t));
    if(capacity<bytes){Error(ERROR_INSUFFICIENT_BUFFER);return bytes;}
    std::memcpy(buffer,value.c_str(),bytes);Error(0);return bytes;
}catch(...){Exception();return 0;}}
}

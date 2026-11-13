#define IC_SDK_INTERNAL
#include "../../native/SDK/Interceptor.h"
#include "FakeSdk.h"
#include <condition_variable>
#include <deque>
#include <atomic>
namespace {
std::mutex mutex;std::condition_variable changed;
std::deque<Interceptor::Keyboard_Input> keys;
std::deque<Interceptor::Mouse_Input> mice;
unsigned sentKeys=0,sentMice=0;std::uint64_t sequence=0;
HANDLE keyEvent=CreateEventW(nullptr,TRUE,FALSE,nullptr),mouseEvent=CreateEventW(nullptr,TRUE,FALSE,nullptr);
unsigned removed=0;
}
namespace Interceptor {
class Context{};
void Context_Deleter::operator()(Context* c)const noexcept {delete c;}
Init_Result Init_AllDevices(){Init_Result result;result.ready=true;return result;}
std::vector<Device_Info> Get_Devices(Context&){std::vector<Device_Info> result;std::lock_guard<std::mutex> lock(mutex);
    for(unsigned type=1;type<=2;++type)for(unsigned instance=1;instance<=2;++instance){Device_Info d{};d.type=type;d.id=type*100+instance;if(d.id==removed)continue;d.instance_token=d.id;d.vid=0x1234;d.pid=0x1000+static_cast<unsigned short>(instance);swprintf_s(reinterpret_cast<wchar_t*>(d.hardware_id),256,L"HID\\VID_1234&PID_%04X",d.pid);result.push_back(d);}return result;}
Context_Result Create_Context(){Context_Result result;result.context.reset(new Context);result.devices=Get_Devices(*result.context);return result;}
void Destroy_Context(Context_Ptr& c)noexcept {c.reset();}
void Destroy_Context(Context_Result& c)noexcept {c.context.reset();}
IC_CAPABILITIES Get_Capabilities(){IC_CAPABILITIES result{};result.abi_version=IC_VERSION;return result;}
std::uint8_t Wait_For_Input(Context&,DWORD timeout,HANDLE stop){HANDLE events[]={stop,keyEvent,mouseEvent};auto result=WaitForMultipleObjects(3,events,FALSE,timeout);if(result==WAIT_OBJECT_0||result==WAIT_TIMEOUT)return 0;if(result==WAIT_OBJECT_0+1)return 1;if(result==WAIT_OBJECT_0+2)return 2;throw std::system_error(GetLastError(),std::system_category());}
void Get_Keyboard_Input(Context&,std::vector<Keyboard_Input>& out,std::uint32_t count){std::lock_guard<std::mutex> lock(mutex);out.clear();while(!keys.empty()&&out.size()<count){out.push_back(keys.front());keys.pop_front();}if(keys.empty())ResetEvent(keyEvent);}
void Get_Mouse_Input(Context&,std::vector<Mouse_Input>& out,std::uint32_t count){std::lock_guard<std::mutex> lock(mutex);out.clear();while(!mice.empty()&&out.size()<count){out.push_back(mice.front());mice.pop_front();}if(mice.empty())ResetEvent(mouseEvent);}
Send_Result Send_Input(Context&,const Keyboard_Input*,std::size_t n){std::lock_guard<std::mutex> lock(mutex);sentKeys+=static_cast<unsigned>(n);changed.notify_all();return {n,0};}
Send_Result Send_Input(Context&,const Mouse_Input*,std::size_t n){std::lock_guard<std::mutex> lock(mutex);sentMice+=static_cast<unsigned>(n);changed.notify_all();return {n,0};}
}
void FakeReset(){std::lock_guard<std::mutex> lock(mutex);keys.clear();mice.clear();sentKeys=sentMice=removed=0;sequence=0;ResetEvent(keyEvent);ResetEvent(mouseEvent);}
void FakePushKey(unsigned device,unsigned code,unsigned flags){std::lock_guard<std::mutex> lock(mutex);Interceptor::Keyboard_Input input{};input.device=device;input.sequence=++sequence;input.stroke.make_code=static_cast<unsigned short>(code);input.stroke.flags=static_cast<unsigned short>(flags);keys.push_back(input);SetEvent(keyEvent);}
void FakePushMouse(unsigned device,int x,int y,unsigned buttons,short wheel,unsigned flags){std::lock_guard<std::mutex> lock(mutex);Interceptor::Mouse_Input input{};input.device=device;input.sequence=++sequence;input.stroke.x=x;input.stroke.y=y;input.stroke.button_flags=static_cast<unsigned short>(buttons);input.stroke.button_data=static_cast<unsigned short>(wheel);input.stroke.flags=static_cast<unsigned short>(flags);mice.push_back(input);SetEvent(mouseEvent);}
unsigned FakeSentKeys(){std::lock_guard<std::mutex> lock(mutex);return sentKeys;}
unsigned FakeSentMice(){std::lock_guard<std::mutex> lock(mutex);return sentMice;}
bool FakeAwaitSent(unsigned k,unsigned m){std::unique_lock<std::mutex> lock(mutex);return changed.wait_for(lock,std::chrono::seconds(5),[&]{return sentKeys>=k&&sentMice>=m;});}
void FakeRemove(unsigned id){std::lock_guard<std::mutex> lock(mutex);removed=id;}


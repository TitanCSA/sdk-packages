#include "InterceptorBridge.h"
#include <windows.h>
#include <stdexcept>
#include <cstring>
#include <thread>
#include <iostream>
extern "C" __declspec(dllimport) void autohotinterceptor_test_mode(unsigned);
extern "C" __declspec(dllimport) long long autohotinterceptor_test_seen(unsigned,unsigned);
void Check(bool value) {if(!value)throw std::runtime_error("Product C ABI contract failed");}
bool Allowed(unsigned kind) {return kind<=2 ? (TEST_INPUT_MASK&(1u<<(kind-1)))!=0 : (TEST_CONTROLLER_MASK&(1u<<(kind-3)))!=0;}
int main() {
 try {
  Check(!std::strcmp(autohotinterceptor_product_name(),TEST_PRODUCT));
  Check(autohotinterceptor_input_mask()==TEST_INPUT_MASK && autohotinterceptor_controller_mask()==TEST_CONTROLLER_MASK);
  Check(autohotinterceptor_version()==1 && autohotinterceptor_sizeof(1)==sizeof(autohotinterceptor_field) && autohotinterceptor_sizeof(6)==sizeof(autohotinterceptor_info));
  unsigned expected=0;for(unsigned k=1;k<=6;++k)if(Allowed(k))++expected;
  autohotinterceptor_error error{};uint64_t session=0,other=0;
  Check(!autohotinterceptor_open(&session,&error));Check(autohotinterceptor_open(&other,&error)==ERROR_BUSY && !other);
  autohotinterceptor_init init{};Check(!autohotinterceptor_initialize(session,&init,&error)&&init.ready);
  autohotinterceptor_info info{};Check(!autohotinterceptor_info_get(session,&info,&error));
  Check(info.input_mask==TEST_INPUT_MASK && info.controller_mask==TEST_CONTROLLER_MASK);
  const autohotinterceptor_devices* devices=nullptr;Check(!autohotinterceptor_devices_get(session,&devices,&error)&&devices->count==expected);
  for(unsigned n=0;n<devices->count;++n)Check(Allowed(devices->items[n].kind));
  const autohotinterceptor_batch* batch=nullptr;Check(!autohotinterceptor_receive(session,0,&batch,&error)&&batch->count==expected);
  auto ticket=batch->id;const autohotinterceptor_batch* unused=nullptr;
  Check(autohotinterceptor_receive(session,0,&unused,&error)==ERROR_BUSY && !unused);
  for(unsigned n=0;n<batch->count;++n) {
   auto& event=batch->events[n];Check(Allowed(event.kind));Check(event.sequence>0x7fffffffffffffffull);
   for(unsigned f=0;f<event.field_count;++f) {
    auto& field=event.fields[f];
    if(event.kind==1 && !std::strcmp(field.name,"scan_code"))field.value=0x30;
    if(event.kind==2 && !std::strcmp(field.name,"y"))field.value=-7;
    if((event.kind==3 || event.kind==5) && field.group==AUTOHOTINTERCEPTOR_AXIS && field.index==0){field.value=50;field.normalized=.5;}
    if(event.kind==4 && !std::strcmp(field.name,"left_x"))field.value=-100;
   }
  }
  Check(!autohotinterceptor_send(session,ticket,&error)&&error.accepted==expected);
  if(Allowed(1))Check(autohotinterceptor_test_seen(1,1)==0x30);
  if(Allowed(2))Check(autohotinterceptor_test_seen(2,1)==-7);
  if(Allowed(3))Check(autohotinterceptor_test_seen(3,1)==50 && autohotinterceptor_test_seen(3,2)==500);
  Check(!autohotinterceptor_send(session,ticket,&error)&&!error.accepted);
  Check(!autohotinterceptor_receive(session,0,&batch,&error));Check(autohotinterceptor_send(session,ticket,&error)==ERROR_INVALID_HANDLE);
  batch->events[0].blocked=1;Check(!autohotinterceptor_send(session,batch->id,&error)&&error.accepted==expected-1);
  Check(!autohotinterceptor_receive(session,0,&batch,&error));autohotinterceptor_test_mode(2);
  Check(autohotinterceptor_send(session,batch->id,&error)==ERROR_BUSY && error.accepted==1);
  Check(!autohotinterceptor_send(session,batch->id,&error)&&error.accepted==expected-1);autohotinterceptor_test_mode(0);
  Check(!autohotinterceptor_receive(session,0,&batch,&error));Check(!autohotinterceptor_discard(session,batch->id,&error));
  for(unsigned kind=1;kind<=6;++kind)if(!Allowed(kind)) {
   autohotinterceptor_event input{};input.kind=kind;Check(autohotinterceptor_inject(session,&input,1,&error)==ERROR_NOT_SUPPORTED);
  }
  if(Allowed(2)) {
   autohotinterceptor_field x{};x.name="x";x.value=123;autohotinterceptor_event input{};input.kind=2;input.fields=&x;input.field_count=1;
   Check(!autohotinterceptor_inject(session,&input,1,&error)&&error.accepted==1);
  }
  autohotinterceptor_blob blob{};Check(!autohotinterceptor_descriptor(session,devices->items[0].token,1,&blob,&error));
  autohotinterceptor_test_mode(1);uint32_t result=999;
  std::thread reader([&]{autohotinterceptor_error e{};const autohotinterceptor_batch* empty=nullptr;result=autohotinterceptor_receive(session,5000,&empty,&e);});
  Sleep(30);Check(!autohotinterceptor_cancel(session,&error));reader.join();Check(!result);
  Check(!autohotinterceptor_close(session,&error));Check(autohotinterceptor_close(session,&error)==ERROR_INVALID_HANDLE);
  autohotinterceptor_test_mode(0);Check(!autohotinterceptor_open(&session,&error));Check(!autohotinterceptor_close(session,&error));
  std::cout<<TEST_PRODUCT<<": C ABI, product masks, edits, blocking, retries, uint64, injection and cancellation passed\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}
}

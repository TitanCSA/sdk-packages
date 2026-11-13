#include "FyraDeviceJsonSDK.h"
#include <string.h>
int main(void){uint64_t session=0;if(fyradevice_json_open(&session))return 1;char *reply=fyradevice_json_call(session,"{",1);if(!reply){fyradevice_json_close(session);return 2;}int failed=strstr(reply,"\"ok\":false")==0;fyradevice_json_free(reply);if(fyradevice_json_close(session))return 3;return failed;}

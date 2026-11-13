#include "nodeinterceptorJsonSDK.h"
#include <string.h>
int main(void){uint64_t session=0;if(node_interceptor_json_open(&session))return 1;char *reply=node_interceptor_json_call(session,"{",1);if(!reply){node_interceptor_json_close(session);return 2;}int failed=strstr(reply,"\"ok\":false")==0;node_interceptor_json_free(reply);if(node_interceptor_json_close(session))return 3;return failed;}

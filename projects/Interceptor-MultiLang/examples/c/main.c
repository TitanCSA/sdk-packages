#include "InterceptorBridge.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    puts("Invert mouse Y. Close the program to restore normal input.");
    uint64_t io=0;
    icb_error error={0};
    if (icb_open(&io,&error)) goto failed;
    for (;;) {
        const icb_batch* inputs=NULL;
        if (icb_receive(io,UINT32_MAX,&inputs,&error)) break;
        for (uint32_t n=0;n<inputs->count;++n) {
            icb_event* input=&inputs->events[n];
            if (input->kind!=ICB_MOUSE) continue;
            int absolute=0;
            for(uint32_t f=0;f<input->field_count;++f)
                if(!strcmp(input->fields[f].name,"flags"))
                    absolute=(input->fields[f].value&1)!=0;
            if(absolute) continue;
            for (uint32_t f=0;f<input->field_count;++f)
                if (!strcmp(input->fields[f].name,"y"))
                    input->fields[f].value=-input->fields[f].value;
        }
        if (icb_send(io,inputs->id,&error)) break;
    }
failed:
    if (error.code) fprintf(stderr,"%s\n",error.message);
    { icb_error cleanup={0}; if(io) icb_close(io,&cleanup); }
    return error.code?1:0;
}

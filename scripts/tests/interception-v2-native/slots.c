#include "interception.h"

#include <stdio.h>

int main(void)
{
    unsigned keyboards = 0;
    unsigned mice = 0;
    for (InterceptionDevice slot = 1; slot <= INTERCEPTION_MAX_DEVICE; ++slot) {
        keyboards += interception_is_keyboard(slot) != 0;
        mice += interception_is_mouse(slot) != 0;
    }
    printf("Keyboard slots: %u; mouse slots: %u.\n", keyboards, mice);
    return keyboards == 10 && mice == 10 ? 0 : 1;
}

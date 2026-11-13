#include "Interceptor.h"
#include <iostream>

int main() {
    std::cout << "Invert mouse Y. Close the program to restore normal input.\n";
    for (;;) {
        auto inputs=Interceptor::Get_Input_With_Timeout();
        for (auto& input:inputs)
            if (auto* mouse=input.Get<Interceptor::Mouse_Input>(); mouse && !(mouse->stroke.flags & Interceptor::MouseAbsolute))
                mouse->stroke.y = mouse->stroke.y == INT32_MIN ? INT32_MAX : -mouse->stroke.y;
        if (!Interceptor::Send_Input(inputs)) return 1;
    }
}

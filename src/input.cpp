#include "include/input.h"
#include <windows.h>

namespace Input {

    bool isKeyDown( int key ) {
        if (GetAsyncKeyState(key) & 0x8000)
            return true;
        return false;
    }

}
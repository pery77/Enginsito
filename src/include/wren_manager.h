#pragma once

#include "wren.h"
#include "../../libs/raylib/include/raylib.h"

class WrenManager{
    public:
        WrenConfiguration wConfig;

        WrenManager();
        ~WrenManager();

    private:
        static void writeFn(WrenVM* vm, const char* text);
        static void errorFn(WrenVM* vm, WrenErrorType errorType,
                    const char* module, const int line,
                    const char* msg);

};


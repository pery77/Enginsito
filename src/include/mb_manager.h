#pragma once

#include "my_basic.h"
#include "../../libs/raylib/include/raylib.h"
#include <sstream>
#include <assert.h>

class mbManager{

    public:
        struct mb_interpreter_t* bas = NULL;
        void* context = NULL;

        mbManager();
        ~mbManager();

        static int drawtext(struct mb_interpreter_t* s, void** l);
};
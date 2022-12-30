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

        void init();
        void tick();
        void draw();
        void end();

        void OpenBas();
        void CloseBas();

    private:
        mb_value_t nullArg[1];
        mb_value_t initRoutine;
        mb_value_t tickRoutine;
        mb_value_t drawRoutine;
        mb_value_t endRoutine;

        void doRoutine(char* routineName, mb_value_t routine);

        static int drawtext(struct mb_interpreter_t* s, void** l);
};
#pragma once

#include "my_basic.h"
#include "../../libs/raylib/include/raylib.h"
#include <sstream>
#include <assert.h>
#include "tools.h"

class MBManager{

    public:
        struct mb_interpreter_t* bas = NULL;
        void* context = NULL;

        MBManager(Tools* toolsref);
        ~MBManager();

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
        //Draw
        static int cls(struct mb_interpreter_t* s, void** l);
        //Shapes
        static int drawpixel(struct mb_interpreter_t* s, void** l);
        static int drawline(struct mb_interpreter_t* s, void** l);
        static int drawcircle(struct mb_interpreter_t* s, void** l);
        static int drawellipse(struct mb_interpreter_t* s, void** l);
        static int drawtriangle(struct mb_interpreter_t* s, void** l);
        static int drawrect(struct mb_interpreter_t* s, void** l);
        //Text
        static int drawtext(struct mb_interpreter_t* s, void** l);

        //Tools
        static int textformat(struct mb_interpreter_t* s, void** l);
        static int delta(struct mb_interpreter_t* s, void** l);
        //Input
        static int getkeydown(struct mb_interpreter_t* s, void** l);
        static int mouseX(struct mb_interpreter_t* s, void** l);
        static int mouseY(struct mb_interpreter_t* s, void** l);
};
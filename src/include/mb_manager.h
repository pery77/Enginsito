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
        const char* basFile;

        MBManager();
        ~MBManager();

        void init();
        void tick();
        void draw();
        void end();

        int OpenBas(const char *file);
        void Run();
        void CloseBas();

        void UpdateAudio();

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
        static int drawPixel(struct mb_interpreter_t* s, void** l);
        static int drawLine(struct mb_interpreter_t* s, void** l);
        static int drawCircle(struct mb_interpreter_t* s, void** l);
        static int drawRing(struct mb_interpreter_t* s, void** l);
        static int drawEllipse(struct mb_interpreter_t* s, void** l);
        static int drawTriangle(struct mb_interpreter_t* s, void** l);
        static int drawRect(struct mb_interpreter_t* s, void** l);
        static int drawPoly(struct mb_interpreter_t* s, void** l);

        //Text
        static int drawText(struct mb_interpreter_t* s, void** l);
        static int drawFont(struct mb_interpreter_t* s, void** l);
        static int measureText(struct mb_interpreter_t* s, void** l);

        //Tools
        static int intToText(struct mb_interpreter_t* s, void** l);
        static int floatToText(struct mb_interpreter_t* s, void** l);
        static int delta(struct mb_interpreter_t* s, void** l);
        static int setColor(struct mb_interpreter_t* s, void** l);
        static int restorePalette(struct mb_interpreter_t* s, void** l);

        //Input
        static int keyPressed(struct mb_interpreter_t* s, void** l);
        static int keyDown(struct mb_interpreter_t* s, void** l);
        static int keyReleased(struct mb_interpreter_t* s, void** l);
        static int keyUp(struct mb_interpreter_t* s, void** l);
        
        static int getKey(struct mb_interpreter_t* s, void** l);
        static int getKeyChar(struct mb_interpreter_t* s, void** l);

        static int mouseX(struct mb_interpreter_t* s, void** l);
        static int mouseY(struct mb_interpreter_t* s, void** l);
        static int mouseWheel(struct mb_interpreter_t* s, void** l);  
        static int setMousePosition(struct mb_interpreter_t* s, void** l);

        static int mousePressed(struct mb_interpreter_t* s, void** l);
        static int mouseDown(struct mb_interpreter_t* s, void** l);
        static int mouseReleased(struct mb_interpreter_t* s, void** l);
        static int mouseUp(struct mb_interpreter_t* s, void** l);  

        //Sound
        static int setSequence(struct mb_interpreter_t* s, void** l); 
        static int playNote(struct mb_interpreter_t* s, void** l);
        static int stopNote(struct mb_interpreter_t* s, void** l);
        static int sfxSet(struct mb_interpreter_t* s, void** l);
        static int sfxPlay(struct mb_interpreter_t* s, void** l);

        //Sprites
        static int sprite(struct mb_interpreter_t* s, void** l);
};
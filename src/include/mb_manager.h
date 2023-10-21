#pragma once

#include "mybasic/my_basic.h"
#include "../../libs/raylib/include/raylib.h"
#include <sstream>
#include <assert.h>
#include "tools.h"

class Engine;

class MBManager{

    public:
        struct mb_interpreter_t* bas = NULL;
        void* context = NULL;
        const char* basFile;

        MBManager(Engine* _engine);
        ~MBManager();

        void init();
        void tick();
        void pause();
        void draw();
        void close();

        int OpenBas(const char *file);
        void Run();
        void Reset();
        void CloseBas();

        void UpdateAudio();
        void UpdateVars();

        uint32_t GetCurrentFrame();

    private:
        mb_value_t nullArg[1];
        mb_value_t initRoutine;
        mb_value_t tickRoutine;
        mb_value_t pauseRoutine;
        mb_value_t drawRoutine;
        mb_value_t closeRoutine;

        void managerError(int state);

        void doRoutine(char* routineName, mb_value_t routine);
        static int updateVars(struct mb_interpreter_t* s, void** l);

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
        static int drawRectRound(struct mb_interpreter_t* s, void** l);
        static int drawPoly(struct mb_interpreter_t* s, void** l);

        //Text
        static int drawText(struct mb_interpreter_t* s, void** l);
        static int measureText(struct mb_interpreter_t* s, void** l);
        static int getChar(struct mb_interpreter_t* s, void** l);
        static int setFontSpacing(struct mb_interpreter_t* s, void** l);

        //Tools
        static int formatText(struct mb_interpreter_t* s, void** l);

        static int peek(struct mb_interpreter_t* s, void** l);
        static int poke(struct mb_interpreter_t* s, void** l);
    
        static int savedata(struct mb_interpreter_t* s, void** l);
        static int loaddata(struct mb_interpreter_t* s, void** l);
        static int quit(struct mb_interpreter_t* s, void** l);

        //Input
        //Keys
        static int keyPressed(struct mb_interpreter_t* s, void** l);
        static int keyDown(struct mb_interpreter_t* s, void** l);
        static int keyReleased(struct mb_interpreter_t* s, void** l);
        static int keyUp(struct mb_interpreter_t* s, void** l);
        
        static int getKey(struct mb_interpreter_t* s, void** l);
        static int getKeyChar(struct mb_interpreter_t* s, void** l);
        
        //Mouse
        static int mouseWheel(struct mb_interpreter_t* s, void** l);  
        static int setMousePosition(struct mb_interpreter_t* s, void** l);

        static int mousePressed(struct mb_interpreter_t* s, void** l);
        static int mouseDown(struct mb_interpreter_t* s, void** l);
        static int mouseReleased(struct mb_interpreter_t* s, void** l);
        static int mouseUp(struct mb_interpreter_t* s, void** l);  

        //Gamepads
        static int isGamepadAvailable(struct mb_interpreter_t* s, void** l);
        static int getGamepadName(struct mb_interpreter_t* s, void** l);
        
        static int buttonPressed(struct mb_interpreter_t* s, void** l);
        static int buttonDown(struct mb_interpreter_t* s, void** l);
        static int buttonReleased(struct mb_interpreter_t* s, void** l);
        static int buttonUp(struct mb_interpreter_t* s, void** l);
        
        static int getButton(struct mb_interpreter_t* s, void** l);
        static int getAxisCount(struct mb_interpreter_t* s, void** l);
        static int axisValue(struct mb_interpreter_t* s, void** l);


        //Sound
        static int getFrameAverage(struct mb_interpreter_t* s, void** l); 
        static int getNoteName(struct mb_interpreter_t* s, void** l); 
        static int setPreset(struct mb_interpreter_t* s, void** l); 
        static int setSequence(struct mb_interpreter_t* s, void** l); 
        static int playNote(struct mb_interpreter_t* s, void** l);
        static int stopNote(struct mb_interpreter_t* s, void** l);
        static int musicPlay(struct mb_interpreter_t* s, void** l);
        static int musicStop(struct mb_interpreter_t* s, void** l);
        static int getMusicPosition(struct mb_interpreter_t* s, void** l);
        static int getMusicSize(struct mb_interpreter_t* s, void** l);
        
        //Sprites
        static int drawSprite(struct mb_interpreter_t* s, void** l);
        static int drawMetaSprite(struct mb_interpreter_t* s, void** l);

        static int my_print(struct mb_interpreter_t* s, const char* fmt, ...);
        static int my_input(struct mb_interpreter_t* s, const char* pmt, char* buf, int n);
};
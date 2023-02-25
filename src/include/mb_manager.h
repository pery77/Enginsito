#pragma once

#include "my_basic.h"
#include "../../libs/raylib/include/raylib.h"
#include <sstream>
#include <assert.h>
#include "tools.h"
#include "postProcessing.h"

class MBManager{

    public:
        struct mb_interpreter_t* bas = NULL;
        void* context = NULL;
        const char* basFile;

        MBManager(PostProcessing* postProcessing);
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
        static int drawRectRound(struct mb_interpreter_t* s, void** l);
        static int drawPoly(struct mb_interpreter_t* s, void** l);

        //Text
        static int drawText(struct mb_interpreter_t* s, void** l);
        static int measureText(struct mb_interpreter_t* s, void** l);
        static int getChar(struct mb_interpreter_t* s, void** l);
        static int setFontSpacing(struct mb_interpreter_t* s, void** l);
        static int updateFont(struct mb_interpreter_t* s, void** l);

        //Tools
        static int intToText(struct mb_interpreter_t* s, void** l);
        static int floatToText(struct mb_interpreter_t* s, void** l);
        static int delta(struct mb_interpreter_t* s, void** l);
        static int setColor(struct mb_interpreter_t* s, void** l);
        static int getColor(struct mb_interpreter_t* s, void** l);
        static int getFiles(struct mb_interpreter_t* s, void** l);
        static int getFolders(struct mb_interpreter_t* s, void** l);
        static int peek(struct mb_interpreter_t* s, void** l);
        static int poke(struct mb_interpreter_t* s, void** l);
        static int dumpMemory(struct mb_interpreter_t* s, void** l);
        static int loadMemory(struct mb_interpreter_t* s, void** l);

        static int saveFile(struct mb_interpreter_t* s, void** l);
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
        
        static int sfxRender(struct mb_interpreter_t* s, void** l);
        static int sfxEnv(struct mb_interpreter_t* s, void** l);
        static int sfxFreq(struct mb_interpreter_t* s, void** l);
        static int sfxTone(struct mb_interpreter_t* s, void** l);
        static int sfxRepeat(struct mb_interpreter_t* s, void** l);
        static int sfxFilter(struct mb_interpreter_t* s, void** l);
        static int sfxPlay(struct mb_interpreter_t* s, void** l);

        //Sprites
        static int drawSprite(struct mb_interpreter_t* s, void** l);
        static int drawMetaSprite(struct mb_interpreter_t* s, void** l);
        static int addMetaSprite(struct mb_interpreter_t* s, void** l);
        static int getMetaSprite(struct mb_interpreter_t* s, void** l);
        static int setSprite(struct mb_interpreter_t* s, void** l);
        static int renderSprites(struct mb_interpreter_t* s, void** l);
        static int getSpriteByte(struct mb_interpreter_t* s, void** l);

        //CRT
        static int crtEnabled(struct mb_interpreter_t* s, void** l);
};
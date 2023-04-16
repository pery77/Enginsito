#pragma once

#include "../../libs/raylib/include/raylib.h"
#include "../../libs/raylib/include/raymath.h"

#include "tools.h"



#define GAME_SCREEN_W       320
#define GAME_SCREEN_H       200
#define GAME_FPS            60
#define GAME_RATIO          1.6
#define ASSETS_FOLDER       "assets"
#define PROGRAM_EXTENSION   ".bas"
#define BOOT_FILE           "assets/boot.txt"

#define PE_VERSION          257

enum GameState{
    Off,
    Running,
    Paused
};

class MBManager;
class PostProcessing;

class Engine
{
    public:

        Engine();
        ~Engine();

        MBManager* basicIntepreter;
        PostProcessing* postProcessing;

        void DropFileUpdate();

        

    private:

};
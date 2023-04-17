#pragma once

#include "../../libs/raylib/include/raylib.h"
#include "../../libs/raylib/include/raymath.h"

#include "tools.h"

enum GameState{
    Off,
    Running,
    Paused
};

class MBManager;
class PostProcessing;
class Bios;
class Editor;

class Engine
{
    public:

        Engine();
        ~Engine();

        void Init();

        void DropFileUpdate();

        MBManager* basicIntepreter;
        PostProcessing* postProcessing;
        Bios* bios;
        Editor* editor;

        static const char *GetEngineName();
        void UpdateFileWatcher();

        bool FileWatcherEnabled = false;

    private:

};
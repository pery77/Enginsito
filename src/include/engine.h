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
class SpriteManager;
class AudioManager;

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
        SpriteManager* spriteManager;
        AudioManager* audioManager;

        static const char *GetEngineName();
        void UpdateFileWatcher();

        bool FileWatcherEnabled = false;
        GameState currentState = Off;

        unsigned char MainMemory[4096]{};
        unsigned char *GetMemory();
        void DumpMemory(const char *path);
        void LoadMemory(const char *path);
        void LoadDefaultMemory();
        void ReadHexFile(const char* filePath);

        unsigned char Peek(unsigned short dir);
        void Poke(unsigned short dir,unsigned char value);

        int GetVersion();
        void SetVersion();
        void SetVirtualMouse(int x, int y);

        int VirtualMouseX;
        int VirtualMouseY;

        bool isInImGui;
};
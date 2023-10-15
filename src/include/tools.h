#pragma once
#include "../../libs/raylib/include/raylib.h"
#include "../../libs/raylib/include/raymath.h"
#include <vector>
#include <fstream>
#include <algorithm>

#include "audio_manager.h"

#include <filesystem>
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <array>
#include "console.h"

#define GAME_SCREEN_W       320
#define GAME_SCREEN_H       200
#define GAME_FPS            60
#define GAME_RATIO          1.6
#define ASSETS_FOLDER       "assets"
#define CONFIG_FOLDER       "config"
#define BOOT_FILE           "config/boot.txt"
#define PROGRAM_EXTENSION   ".bas"
#define MEM_EXTENSION       ".data"

#define PE_VERSION          260

struct Vector2i {
	int x;
	int y;
};

struct Command {
    std::string command;
    std::string args[10];
};

typedef struct DataBytes{
    unsigned char bytes[8];
}DataBytes;

typedef struct SpriteData{
    unsigned char bytes[5];// id, offset_X, offset_y, color, flags
}SpriteData;

typedef struct MetaSprites{
    SpriteData sprites[8];
}MetaSprites;

class Tools{
    
    public:

        static int GetVirtualMouse(bool isXAxis);
        static bool CompareFloats(float x, float y, float epsilon);
        static int IntClamp(int value, int min, int max);

        static void Trim(char * s);
        static char* ToUpper(char* s);
        static std::string GetCharFromCodepoint(int ch);
        static std::vector<std::string> Split(const std::string& str, const char sep);
        static std::stringstream GetFiles(const char *path);
        static std::stringstream GetFolders(const char *path);
        static std::stringstream GetDir(const char *path);
        static bool CheckAssetsFolder();
        static bool DirExist(std::string path);
        static bool FileExist(std::string path, std::string file);
        static bool FileMemoryExist(std::string memoryFile);

        static float Min(float a, float b);

        static int8_t ToSigned(unsigned char value);
        static unsigned char ToUnsigned(int8_t value);

        static Console* console;
        static Texture TextureFromCode(int format, int height, int width, void* data, int mips);
        
        static const char* GetCurrentLayout(int currentLayout);
};
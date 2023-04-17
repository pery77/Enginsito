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
#include "editor.h"


#define GAME_SCREEN_W       320
#define GAME_SCREEN_H       200
#define GAME_FPS            60
#define GAME_RATIO          1.6
#define ASSETS_FOLDER       "assets"
#define PROGRAM_EXTENSION   ".bas"
#define BOOT_FILE           "assets/boot.txt"

#define PE_VERSION          258

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

        static Color GetColor(int color);
        static Color GetBiosColor(int color);
        static Color SetColor(int color, int r, int g, int b);

        static void InitFont();
        static Font GetFont();

        static void SetSprite(unsigned int id, unsigned char b0, unsigned char b1, unsigned char b2, 
                                unsigned char b3, unsigned char b4, unsigned char b5, unsigned char b6,
                                unsigned char b7);
        static void DrawSprite(int id, int x, int y, int col, int flag);
        static void DrawMetaSprite(int id, int x, int y);
        static void RenderSprites();
        static Texture GetSpriteTexture();
        static unsigned char GetSpriteByte(unsigned int id, unsigned char byte);

        static void AddMetaSprite(unsigned char id, unsigned char postition, unsigned char sprite_id, unsigned char offset_x, unsigned char offset_y, 
                                    unsigned char color, unsigned char flags);
        static std::array<int,20> GetMetaSprite(unsigned char id);

        static int GetVirtualMouse(bool isXAxis);
        static void SetVirtualMouse(int x,int y);
        static bool CompareFloats(float x, float y, float epsilon);
        static int IntClamp(int value, int min, int max);

        static void Trim(char * s);
        static char* ToUpper(char* s);
        static std::string GetCharFromCodepoint(int ch);
        static std::vector<std::string> Split(const std::string& str, const char sep);
        static std::stringstream GetFiles(const char *path);
        static std::stringstream GetFolders(const char *path);
        static std::stringstream GetDir(const char *path);
        static bool DirExist(std::string path);
        static bool FileExist(std::string path, std::string file);

        static float Min(float a, float b);

        static void SetFontSpacing(int spacing);
        static float GetFontSpacing();

        static void DumpMemory(const char *path);
        static void LoadMemory(const char *path);
        static unsigned char Peek(unsigned short dir);
        static void Poke(unsigned short dir,unsigned char value);

        static int GetVersion();
        static void SetVersion();

        static int8_t ToSigned(unsigned char value);
        static unsigned char ToUnsigned(int8_t value);

        static Console* console;
        static Editor* editor;
};
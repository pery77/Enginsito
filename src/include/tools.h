#pragma once
#include "../../libs/raylib/include/raylib.h"
#include "../../libs/raylib/include/raymath.h"

#include "audio_manager.h"
#include "ini_manager.h"

#include <filesystem>
#include <iostream>
#include <dirent.h>
#include <sys/types.h>

//#define max(a, b) ((a)>(b)? (a) : (b))
//#define min(a, b) ((a)<(b)? (a) : (b))

#define GAME_SCREEN_W 320
#define GAME_SCREEN_H 200
#define GAME_FPS      60
#define GAME_RATIO    1.6
#define ASSETS_FOLDER     "assets"
#define PROGRAM_EXTENSION ".BAS"
#define BOOT_FILE         "assets/boot.txt"

struct Vector2i {
	int x;
	int y;
};

struct Command {
    std::string command;
    std::string args[10];
};

enum GameState{
    Off,
    Running,
    Paused
};

typedef struct FontDataBytes{
    unsigned char bytes[8];
}FontDataBytes;

class Tools{
    public:
        static Color GetColor(int color);
        static Color GetFixedColor(int color);
        static Color SetColor(int color, int r, int g, int b);
        static void CopyPalette();

        static void SetFontChar(unsigned int id, unsigned char b0, unsigned char b1, unsigned char b2, 
                                unsigned char b3, unsigned char b4, unsigned char b5, unsigned char b6,
                                unsigned char b7);
        static unsigned char GetFontByte(unsigned int id, unsigned char byte);
        static void SetFont();
        static Font GetFont();
        static void SaveFont();
     

        static int GetVirtualMouse(bool isXAxis);
        static void SetVirtualMouse(int x,int y);
        static bool CompareFloats(float x, float y, float epsilon);
        static int IntClamp(int value, int min, int max);

        static void Trim(char * s);
        static char* ToUpper(char* s);
        static std::string GetCharFromCodepoint(int ch);
        static std::vector<std::string> Split(const std::string& str, const char sep);
        static std::stringstream GetFiles(const char *path);
        static bool DirExist(std::string path);
        static bool FileExist(std::string path, std::string file);

        static float Min(float a, float b);
};
#pragma once
#include "../../libs/raylib/include/raylib.h"
#include "../../libs/raylib/include/raymath.h"

#include "audio_manager.h"
#include "ini_manager.h"

#include <filesystem>
#include <iostream>
#include <dirent.h>
#include <sys/types.h>

#define max(a, b) ((a)>(b)? (a) : (b))
#define min(a, b) ((a)<(b)? (a) : (b))

#define GAME_SCREEN_W 320
#define GAME_SCREEN_H 200
#define GAME_RATIO 1.6

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

class Tools{
    public:
        static Color GetColor(int col);
        static int GetVirtualMouse(bool isXAxis);
        static bool CompareFloats(float x, float y, float epsilon);

        static void Trim(char * s);
        static std::stringstream GetFiles(const char *path);
};
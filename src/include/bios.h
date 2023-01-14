#pragma once
#include "../../libs/raylib/include/raylib.h"
#include <stdio.h>
#include <string>
#include <string.h>

class Bios{
    public:
        Bios();
        ~Bios();

        const char* text;

        void Update();
        void ProcessCommand();
    private:
        float delta;
        std::string currentLine = "";
        std::string screenLines = "";
        const char* cursor = "";
};
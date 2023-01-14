#pragma once
#include "../../libs/raylib/include/raylib.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>

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
        std::string screenLines = "*****************\n* peryEngine v0.5 *\n*                      *\n* Type HELP       *\n*****************\n\n";
        const char* cursor = "";
};
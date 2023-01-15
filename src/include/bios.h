#pragma once
#include "../../libs/raylib/include/raylib.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>

#include "tools.h"

class Bios{
    public:
        Bios();
        ~Bios();

        void Update();
        void ProcessCommand();

        int backColor = 0;
        int frontColor = 9;
    private:
        float delta;
        std::string currentLine = "";
        std::string screenLines = "*****************\n* peryEngine v0.5 *\n*                      *\n* Type HELP       *\n*****************\n\n";
        const char* cursor = "";

};
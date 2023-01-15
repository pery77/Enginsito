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

        bool ShouldClose = false;
        bool ShouldRun= false;

    private:
        float delta;
        std::string currentLine = "";
        std::string screenLines = "*****************\n* peryEngine v0.5 *\n*                      *\n* Type HELP       *\n*****************\n\n";

        int backColor = 39;
        int frontColor = 7;
        const char* cursor = "";

        Command lastCommand;
};
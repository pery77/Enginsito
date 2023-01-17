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

        std::string HelpInfo = "CLS -> clear screen.\nEXIT -> close program.\nCOLOR Back Front -> change console colors.\nRUN ->\nLIST ->\nCD ->\n";
        std::string CurrentPath = "";

    private:
        float delta;
        std::string currentLine = "";
        std::string screenLines = "*****************\n* peryEngine v0.5 *\n*                      *\n* Type HELP       *\n*****************\n\n";

        int backColor = 51;
        int frontColor = 7;
        const char* cursor = "";
        

        Command lastCommand;
};
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

        std::string GetFile();

        std::string HelpInfo = "CLS - clear screen.\nEXIT - close program.\nCOLOR b f - change console colors.\nRUN - run current program.\nLIST - list current directory.\nCD - change directory\n";
        std::string CurrentPath = "";
        std::string CurrentProgram = "";

    private:
        float delta;
        std::string currentLine = "";
        std::string screenLines = "*****************\n* peryEngine v0.5 *\n*                      *\n* Type HELP       *\n*****************\n\n";

        int backColor = 51;
        int frontColor = 7;
        const char* cursor = "";
        
        Command lastCommand;
        void addSubPath(std::string current);
        void removeSubPath();

};
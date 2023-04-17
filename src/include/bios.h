#pragma once
#include "../../libs/raylib/include/raylib.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>

#include "tools.h"
#include "imgui/imgui.h"

class Engine;

class Bios {

    public:

        Bios(Engine* _engine);
        ~Bios();
        
        void Update();
        void ProcessCommand();

        bool ShouldClose = false;
        bool ShouldRun= false;

        std::string GetFile();
        void SetFile(std::string file);
        void LoadBoot();

        
        std::string CurrentPath = "";
        std::string CurrentProgram = "";

        void DrawExplorer();
        void DrawImGui();

        void AddSubPath(std::string current);
        void RemoveSubPath();

    private:
        float delta;
        std::string currentLine = "";
        std::string screenLines = 
R"|(
****************************************
*  Wecolome to PeryEngine              *
*  Press F1 to open tools or Type HELP *
****************************************
)|";

        int backColor = 0;
        int frontColor = 3;
        const char* cursor = "";
        
        Command lastCommand;
        bool checkCommand(std::string command, std::string value);

        const char* helpText = 
R"|(
HELP - show this. 
CLS  - clear screen.
EXIT - close program.
COLOR b f - change console colors.
RUN - run current program.
DIR - list current directory.
CD  - change directory.
CD. - directory up.
PRINT p - print p in screen.
MEM  - print loaded program in memory.
LOAD - load program in memory.
FILEWATCHER 0|1 - on off filewatcher system.
)|";

};
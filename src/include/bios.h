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
class Editor;

typedef struct 
{
    std::string name;
    std::string programFile;
    std::string memoryFile;
} Project;

class Bios {

    public:

        Bios(Engine* _engine);
        ~Bios();
        
        void Update();
        void ProcessCommand();
        void Print(char* line);

        bool ShouldClose = false;
        bool ShouldRun = false;

        void SetFile(std::string file);
        void LoadBoot();

        std::string CurrentPath = "";

        Project CurrentProject;

        void TryToSaveMemory();
        void TryToLoadMemory();
        void SetProgram(std::string file);
        void AddSubPath(std::string current);
        void RemoveSubPath();

    private:
        float delta;
        std::string currentLine = "";
        std::string screenLines = 
R"|(
****************************************
*        Welcome to Enginsito          *
* Press F1 to open editor or type HELP *
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
FILEWATCHER 0|1 - on off filewatcher 
system.
)|";

};
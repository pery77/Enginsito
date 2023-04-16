#pragma once
#include "../../libs/raylib/include/raylib.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>

#include "tools.h"
#include "imgui/imgui.h"

#include "postProcessing.h"
#include "mb_manager.h"

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

        std::string HelpInfo = "CLS - clear screen.\nEXIT - close program.\nCOLOR b f - change console colors.\nRUN - run current program.\nDIR - list current directory.\nCD - change directory\n";
        std::string CurrentPath = "";
        std::string CurrentProgram = "";

        void DrawExplorer();
        void DrawImGui();

    private:
        float delta;
        std::string currentLine = "";
        std::string screenLines = "Wecolome to PeryEngine\nPress F1 to open tools or Type HELP\n";

        int backColor = 0;
        int frontColor = 3;
        const char* cursor = "";
        
        Command lastCommand;
        void addSubPath(std::string current);
        void removeSubPath();
        bool checkCommand(std::string command, std::string value);
        std::string editorFile = "";

};
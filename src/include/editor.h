#pragma once

#include <iostream>
#include <map>

#include "imgui/imgui.h"
#include "imgui/rlImGui.h"
#include "imgui/imgui_memory_editor.h"
#include "imgui-knobs.h"

#include "engine.h"
#include "bios.h"
#include "TextEditor.h"
#include "postprocessing.h"
#include "mb_manager.h"
#include "sprite_manager.h"


class Editor
{
    #define WHITE_KEY_WIDTH 40
    #define WHITE_KEY_HEIGHT 120
    #define BLACK_KEY_WIDTH 30
    #define BLACK_KEY_HEIGHT 60

    public:

    int keyboardOctave = 4;
    int keyboardBaseKey = 96;
    int pressedKey = -1;

    Texture hackTexture;

    std::map<char, int> keyCharToKey {
	{'Z', 0},
	{'S', 1},
	{'X', 2},
	{'D', 3},
	{'C', 4},
	{'V', 5},
	{'G', 6},
	{'B', 7},
	{'H', 8},
	{'N', 9},
	{'J', 10},
	{'M', 11},
	{',', 12},
	{'L', 13},
	{'.', 14},
	{';', 15},
	{'/', 16},
	{'Q', 12},
	{'2', 13},
	{'W', 14},
	{'3', 15},
	{'E', 16},
	{'R', 17},
	{'5', 18},
	{'T', 19},
	{'6', 20},
	{'Y', 21},
	{'7', 22},
	{'U', 23},
	{'I', 24},
	{'9', 25},
	{'O', 26},
	{'0', 27},
	{'P', 28}
};

    Engine* editorEngineRef;
    TextEditor codeEditor;
    TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::Basic();
    std::string editorFile = "";

    MemoryEditor mem_edit; 

    bool HasFocus = false;
    bool Paused = false;
    bool DoStep = false;

    Editor(Engine* _engine);
    ~Editor();

    void HighLightMemory(uint16_t address, uint16_t size);

    void inline DrawFPS();
    
    void CenterWindow();
   
    void OpenFile();
    void DrawshowFileBrowser();
    
    void SetError(int line, int column, std::string dsc);
    void ClearError();

    void DrawCode();
    void DrawPalette();
    void DrawSprites();
    void DrawCRT();
    
    void DrawPlayer();
    void DrawMemory();
    
    void PianoKey(ImVec2 pos, ImVec2 size, int note, bool isBlack, bool pressed) ;
    bool IsBlack(int note);

    void DrawSFX();
    void Draw();
};
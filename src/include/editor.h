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
	{',', 12}
};

    Engine* editorEngineRef;
    TextEditor codeEditor;
    TextEditor docs;
    TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::Basic();
    std::string editorFile = "";

    MemoryEditor mem_edit; 

    bool ScreenWindowHasFocus = false;
    bool MouseInsideScreenWindow = false;
    bool Paused = false;
    bool DoStep = false;

    int window_position_x;
    int window_position_y;

    Editor(Engine* _engine);
    ~Editor();

    void SetMainWindow();

    void HighLightMemory(uint16_t address, uint16_t size);

    void inline DrawFPS();
    
    void Credits();
   
    void OpenFile();
    void DrawshowFileBrowser();
    
    void SetError(int line, int column, std::string dsc);
    void ClearError();

    int GetColorPopup();
    void GetSpritePopup();
    void MetaSpriteRect(int id, ImVec2 pos, ImVec2 size, int x, int y, ImTextureID my_tex_id);

    void DrawCode();
    void DrawDocs();
    void DrawPalette();
    void SpriteRect(int id, ImVec2 pos, ImVec2 size, int x, int y, ImTextureID my_tex_id);
    void DrawSprites();
    void MakeSprite(int spriteId);
    void DrawMetaLine(int id);
    void DrawMetaSprites(int metaId);
    void PixelRect(int dir, uint8_t bit, ImVec2 pos, ImVec2 size, bool state);
    void DrawCRT();
    
    void DrawPlayer();
    void DrawMemory();
    void DrawMetaExample();
    
    void PianoKey(ImVec2 pos, ImVec2 size, int note, bool isBlack, bool pressed) ;
    bool IsBlack(int note);

    void DrawChannel(uint8_t channel, ImVec2 pos);

    void DrawSFX();
    void Draw();
    void SaveCurrentFile();
};
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

#include "documentation.h"

class Editor
{
    public:
    #define IM_COL32Vec4(c)    (((ImU32)(255)<<IM_COL32_A_SHIFT) | ((ImU32)(c.z * 255)<<IM_COL32_B_SHIFT) | ((ImU32)(c.y * 255)<<IM_COL32_G_SHIFT) | ((ImU32)(c.x * 255)<<IM_COL32_R_SHIFT))
    #define IM_COL32Col(c)    (((ImU32)(255)<<IM_COL32_A_SHIFT) | ((ImU32)(c.b)<<IM_COL32_B_SHIFT) | ((ImU32)(c.g)<<IM_COL32_G_SHIFT) | ((ImU32)(c.r)<<IM_COL32_R_SHIFT))

    int keyboardOctave = 4;
    int pressedKey = -1;
    int currentNote = -1;
    int pianoVolume = 127;

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
    Documentation docs;
    TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::Basic();
    std::string editorFile = "";

    MemoryEditor mem_edit; 
    Texture iconTexture;

    bool ScreenWindowHasFocus = false;
    bool MouseInsideScreenWindow = false;
    bool Enabled = true;
    GameState PlayerState = Off;
   
    bool DoStep  = false;

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

    void DrawCode(bool* p_open);
    void DrawDocs(bool* p_open);
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
    
    void PianoKey(ImVec2 size, int note, bool isBlack);
    bool IsBlack(int note);

    void DrawChannel(uint8_t channel, ImVec2 pos);

    void DrawSFX();
    void DrawToolsPiano();
    void DrawToolsSequence();
    void Draw();
    void SaveCurrentFile();
    void SetMousePosInEditor(int x, int y);

    void Link(const char* text, const char* link, float size);
    void ChangeLayout(uint8_t layout);
    void LoadUIJson();
    void SaveUIJson();
    void CloseAll();

    void Package();
    bool CheckJsonFile();

    void LoadEditorPalette(std::string colors[17]);
    void SetImGuiColors();
    
    //Text
    ImVec4 Col_T_1 = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    ImVec4 Col_T_2 = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    //Primary
    ImVec4 Col_P_M  = ImVec4(0.08f, 0.18f, 0.22f, 1.00f);
    ImVec4 Col_P_B1 = ImVec4(0.23f, 0.35f, 0.38f, 1.00f);
    ImVec4 Col_P_B2 = ImVec4(0.14f, 0.23f, 0.27f, 1.00f);
    ImVec4 Col_P_D1 = ImVec4(0.04f, 0.13f, 0.16f, 1.00f);
    ImVec4 Col_P_D2 = ImVec4(0.01f, 0.07f, 0.09f, 1.00f);

    //Secondary 1
    ImVec4 Col_S1_M  = ImVec4(0.35f, 0.28f, 0.13f, 1.00f);
    ImVec4 Col_S1_B1 = ImVec4(0.61f, 0.53f, 0.36f, 1.00f);
    ImVec4 Col_S1_B2 = ImVec4(0.42f, 0.36f, 0.21f, 1.00f);
    ImVec4 Col_S1_D1 = ImVec4(0.25f, 0.19f, 0.05f, 1.00f);
    ImVec4 Col_S1_D2 = ImVec4(0.15f, 0.10f, 0.01f, 1.00f);

    //Secondary 2
    ImVec4 Col_S2_M  = ImVec4(0.35f, 0.18f, 0.13f, 1.00f);
    ImVec4 Col_S2_B1 = ImVec4(0.61f, 0.42f, 0.36f, 1.00f);
    ImVec4 Col_S2_B2 = ImVec4(0.42f, 0.26f, 0.21f, 1.00f);
    ImVec4 Col_S2_D1 = ImVec4(0.25f, 0.10f, 0.05f, 1.00f);
    ImVec4 Col_S2_D2 = ImVec4(0.15f, 0.04f, 0.01f, 1.00f);

};
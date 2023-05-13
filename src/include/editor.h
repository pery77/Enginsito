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


struct Editor
{
    #define WHITE_KEY_WIDTH 40
    #define WHITE_KEY_HEIGHT 120
    #define BLACK_KEY_WIDTH 30
    #define BLACK_KEY_HEIGHT 60

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


    Editor(Engine* _engine)
    {
        editorEngineRef = _engine;

        codeEditor.SetLanguageDefinition(lang);
        codeEditor.SetPalette(TextEditor::GetBasicPalette()); 
        mem_edit.HighlightColor = IM_COL32(22, 110, 162, 255);

        Image hackImage = GenImageColor(1,1,(Color){0,0,0,0});
        hackTexture = LoadTextureFromImage(hackImage);
    }

    ~Editor()
    {

    }

    void HighLightMemory(uint16_t address, uint16_t size)
    {
        mem_edit.HighlightMin = address;
        mem_edit.HighlightMax = address + size;
        mem_edit.GotoAddr = address;
        mem_edit.DataEditingTakeFocus = false;
        mem_edit.DataEditingAddr = -1;
        mem_edit.DataPreviewAddr = -1;
    }

    void inline DrawFPS()
    {
        ImGui::PushStyleColor(ImGuiCol_PlotLines, IM_COL32(40,255,0,255));
        static float values[90] = {};
        static int values_offset = 0;
        static double refresh_time = 0.0;
        if (refresh_time == 0.0)
            refresh_time = ImGui::GetTime();
        while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
        {
            static float phase = 0.0f;
            values[values_offset] = GetFPS();
            values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
            phase += 0.10f * values_offset;
            refresh_time += 1.0f / 60.0f;
        }
        float average = 0.0f;
        for (int n = 0; n < IM_ARRAYSIZE(values); n++)
            average += values[n];
        average /= (float)IM_ARRAYSIZE(values);
        char overlay[32];
        sprintf(overlay, "avg: %.2f fps", average);
        ImGui::PlotLines("FPS", values, IM_ARRAYSIZE(values), values_offset,overlay ,0.0f, 60.0f, ImVec2(0, 80.0f));
        ImGui::PopStyleColor();  
    }

    void CenterWindow()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 max_size;
        ImVec2 min_size = ImVec2(500,300);
        max_size.x = io.DisplaySize.x;
        max_size.y = io.DisplaySize.y;
        ImGui::SetNextWindowSizeConstraints(min_size, max_size);
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Appearing, ImVec2(0.5f,0.5f));
        ImGui::SetNextWindowSize(ImVec2(std::max<float>(600, min_size.x), std::max<float>(400, min_size.y)), ImGuiCond_Appearing);
    }

    void OpenFile()
    {
        Tools::console->AddLog("Open: [ %s ]\n", editorEngineRef->bios->CurrentProgram.c_str());
        std::ifstream inFile;
        inFile.open(editorEngineRef->bios->GetFile().c_str());
        std::stringstream strStream;
        strStream << inFile.rdbuf();
        std::string str = strStream.str();
        std::cout << str << "\n";
        codeEditor.SetText(str);
        editorFile = editorEngineRef->bios->CurrentProgram;
        Paused = false;
        DoStep = false;
    }

    void DrawshowFileBrowser()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec2 pw_size = ImGui::GetWindowSize();
        float list_item_height = ImGui::CalcTextSize("").y + (style.ItemSpacing.y * 4.0f);
        float input_bar_ypos = pw_size.y - ImGui::GetFrameHeightWithSpacing() * 2.5f - style.WindowPadding.y;
        float window_height = input_bar_ypos - ImGui::GetCursorPosY() - style.ItemSpacing.y;
        float window_content_height = window_height - style.WindowPadding.y * 6.0f;
        float min_content_size = pw_size.x - style.WindowPadding.x * 4.0f;

        static bool memoryMode = false;

        ImGui::BeginChild("#Head",ImVec2(0, list_item_height), true, ImGuiWindowFlags_NoScrollWithMouse);
            ImGui::Text("Current path: %s", editorEngineRef->bios->CurrentPath.c_str());
            ImGui::SameLine();
            ImGui::Checkbox("Memory mode", &memoryMode);
        ImGui::EndChild();

        if(memoryMode) ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.192f, 0.106, 0.216f, 1.0f));
        ImGui::BeginChild("#Files", ImVec2(0, window_content_height), true);
        if(memoryMode) ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.882f, 0.745f, 0.078f, 1.0f));

            std::stringstream ss = Tools::GetFolders(editorEngineRef->bios->CurrentPath.c_str());
            std::string temp;

            if(editorEngineRef->bios->CurrentPath.length() != 0)
            {
                if(ImGui::Selectable("..", false))
                {
                    editorEngineRef->bios->RemoveSubPath();  
                }   
            }
            while (std::getline(ss, temp))
            {
                if(ImGui::Selectable(temp.c_str(), false))
                {
                    editorEngineRef->bios->AddSubPath(temp);
                    break;
                }
            }
            ImGui::PopStyleColor(1);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.827f, 0.871f, 0.98f, 1.0f));
   
        ss = Tools::GetFiles(editorEngineRef->bios->CurrentPath.c_str(), memoryMode);
        while (std::getline(ss, temp))
        {
            if(ImGui::Selectable(temp.c_str(), false))
            {
                if (memoryMode)
                {
                    std::stringstream m;
                    m << ASSETS_FOLDER << "/" << editorEngineRef->bios->CurrentPath << "/" << temp << MEM_EXTENSION;
                    Tools::console->AddLog("[MEMORY] Loaded:");
                    Tools::console->AddLog(m.str().c_str());
                    editorEngineRef->LoadMemory(m.str().c_str());
                }
                else
                {
                    editorEngineRef->bios->CurrentProgram = temp;
                    OpenFile();
                }
            }
        }
            
        ImGui::PopStyleColor(1);
        ImGui::EndChild();

        ImGui::BeginChild("#Foot",ImVec2(0, list_item_height), true, ImGuiWindowFlags_NoScrollWithMouse);
            ImGui::Text("Current program: %s", editorEngineRef->bios->CurrentProgram.c_str());
        ImGui::EndChild();

        ImGui::BeginChild("#Foot2",ImVec2(0, list_item_height*2), true, ImGuiWindowFlags_NoScrollWithMouse);
            static char str0[128] = "new";
            if(ImGui::SmallButton("New file"))
            {
                editorEngineRef->bios->CurrentProgram = str0;
            }
            ImGui::SameLine();
            ImGui::InputText("New File", str0, IM_ARRAYSIZE(str0));
        ImGui::EndChild(); 
    }

    void SetError(int line, int column, std::string dsc)
    {
        TextEditor::ErrorMarkers markers;
        markers.insert(std::make_pair(line, dsc));

        codeEditor.SetErrorMarkers(markers);
        codeEditor.SetCursorPosition(TextEditor::Coordinates{line, column});
    }

    void ClearError()
    {
        TextEditor::ErrorMarkers markers;
        codeEditor.SetErrorMarkers(markers);
    }

    void DrawCode()
    {
        auto cpos = codeEditor.GetCursorPosition();

        ImGui::Begin("Code Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

	    if (ImGui::BeginMenuBar())
	    {
	    	if (ImGui::BeginMenu("Edit"))
	    	{
	    		bool ro = codeEditor.IsReadOnly();
	    		if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
	    			codeEditor.SetReadOnly(ro);
	    		ImGui::Separator();
	    		if (ImGui::MenuItem("Undo", "Ctrl-Z", nullptr, !ro && codeEditor.CanUndo()))
	    			codeEditor.Undo();
	    		if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && codeEditor.CanRedo()))
	    			codeEditor.Redo();
	    		ImGui::Separator();
	    		if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, codeEditor.HasSelection()))
	    			codeEditor.Copy();
	    		if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && codeEditor.HasSelection()))
	    			codeEditor.Cut();
	    		if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && codeEditor.HasSelection()))
	    			codeEditor.Delete();
	    		if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
	    			codeEditor.Paste();
	    		ImGui::Separator();
	    		if (ImGui::MenuItem("Select all", nullptr, nullptr))
	    			codeEditor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(codeEditor.GetTotalLines(), 0));
	    		ImGui::EndMenu();
	    	}
 
	    		if (ImGui::BeginMenu("View"))
	    		{
	    			if (ImGui::MenuItem("Dark palette"))
	    				codeEditor.SetPalette(TextEditor::GetDarkPalette());
	    			if (ImGui::MenuItem("Light palette"))
	    				codeEditor.SetPalette(TextEditor::GetLightPalette());
	    			if (ImGui::MenuItem("Retro blue palette"))
	    				codeEditor.SetPalette(TextEditor::GetRetroBluePalette());
                    if (ImGui::MenuItem("Basic palette"))
	    				codeEditor.SetPalette(TextEditor::GetBasicPalette());
                                           
	    			ImGui::EndMenu();
	    		}
             
	    	ImGui::EndMenuBar();
	    	}

	    	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, codeEditor.GetTotalLines(),
	    		codeEditor.IsOverwrite() ? "Ovr" : "Ins",
	    		codeEditor.CanUndo() ? "*" : " ",
                editorEngineRef->bios->CurrentProgram.c_str());

            codeEditor.Render("TextEditor");

        ImGui::End();
    }

    void DrawPalette()
    {
        for (char c = 0; c<16; c++)
        {
           char buffer [50];
           sprintf (buffer, "[%i]", c);

           Color col = editorEngineRef->spriteManager->GetColor(c);
           ImVec4 color = ImVec4(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, 1.0f);
           ImGui::ColorEdit3(buffer, (float*)&color, 0);

           editorEngineRef->spriteManager->SetColor(c, color.x * 255, color.y * 255, color.z * 255);

           if (ImGui::IsItemHovered())
           {
                HighLightMemory(c*3,3);
           }
        }
    }

    void DrawGraphics()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImTextureID my_tex_id = &editorEngineRef->spriteManager->spriteTexture.id;
        ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImVec2 size = ImVec2(32.0f * io.FontGlobalScale, 32.0f * io.FontGlobalScale);
        
        for (int j = 0; j < 16; j++)
        {
            for (int i = 0; i < 16; i++)
            {
                int id = i + j * 16;
                ImGui::PushID(id);

                ImVec2 uv0 = ImVec2(i * 0.0625f, j * 0.0625f);
                ImVec2 uv1 = ImVec2(i * 0.0625f + 0.0625f, j * 0.0625f + 0.0625f);
                if (ImGui::ImageButton("", my_tex_id, size, uv0, uv1, bg_col, tint_col))
                {
                    HighLightMemory(id*8+48,8);
                }

                ImGui::PopID();
                ImGui::SameLine();
            }
            ImGui::NewLine();
        }
    }

    void DrawCRT()
    {
        bool ppState = editorEngineRef->postProcessing->enabled;
        ImGui::Checkbox("Enabled", &ppState);
        editorEngineRef->postProcessing->SetState(ppState);
        
        int blurPower    = editorEngineRef->Peek(4080);
        int blurFactor   = editorEngineRef->Peek(4081);
        int chromatic    = editorEngineRef->Peek(4082);
        int curvature    = editorEngineRef->Peek(4083);
        int vignetting   = editorEngineRef->Peek(4084);
        int scanLine     = editorEngineRef->Peek(4085);
        int verticalLine = editorEngineRef->Peek(4086);
        int grilleForce  = editorEngineRef->Peek(4087);
        int noise        = editorEngineRef->Peek(4088);
        int fliker       = editorEngineRef->Peek(4089);
        int grille       = editorEngineRef->Peek(4090);

        ImGuiKnobs::KnobInt("Blur", &blurPower, 0, 255, 1, "%03i", ImGuiKnobVariant_Wiper);
        ImGui::SameLine();
        ImGuiKnobs::KnobInt("Factor", &blurFactor, 0, 255, 1, "%03i", ImGuiKnobVariant_Wiper);
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        ImGuiKnobs::KnobInt("Chromatic", &chromatic, 0, 255, 1, "%03i", ImGuiKnobVariant_Wiper);
        ImGui::SameLine();
        ImGuiKnobs::KnobInt("Curvature", &curvature, 0, 255, 1, "%03i", ImGuiKnobVariant_Wiper);
        ImGui::SameLine();
        ImGuiKnobs::KnobInt("Vignetting", &vignetting, 0, 255, 1, "%03i", ImGuiKnobVariant_Wiper);
        ImGui::Text("  ");
        ImGuiKnobs::KnobInt("Scanline", &scanLine, 0, 255, 1, "%03i", ImGuiKnobVariant_Wiper);
        ImGui::SameLine();
        ImGuiKnobs::KnobInt("Vertical", &verticalLine, 0, 255, 1, "%03i", ImGuiKnobVariant_Wiper);
        ImGui::SameLine();
        ImGuiKnobs::KnobInt("Grille F", &grilleForce, 0, 255, 1, "%03i", ImGuiKnobVariant_Wiper);
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        ImGuiKnobs::KnobInt("Noise", &noise, 0, 255, 1, "%03i", ImGuiKnobVariant_Wiper);
        ImGui::SameLine();
        ImGuiKnobs::KnobInt("Flicker", &fliker, 0, 255, 1, "%03i", ImGuiKnobVariant_Wiper);
        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::Text("Grille Mode");
        const char* items[] = { "Dots", "TV", "LCD"};
        ImGui::Combo("#grillemode", &grille, items, IM_ARRAYSIZE(items));
        ImGui::EndGroup();

        if(ImGui::IsWindowFocused())
        {
            editorEngineRef->postProcessing->SetCRTValue(CRTProperty::BlurPower, blurPower);
            editorEngineRef->postProcessing->SetCRTValue(CRTProperty::BlurFactor, blurFactor);
            editorEngineRef->postProcessing->SetCRTValue(CRTProperty::Chromatic, chromatic);
            editorEngineRef->postProcessing->SetCRTValue(CRTProperty::Curvature, curvature);
            editorEngineRef->postProcessing->SetCRTValue(CRTProperty::Vignetting, vignetting);
            editorEngineRef->postProcessing->SetCRTValue(CRTProperty::ScanLine, scanLine);
            editorEngineRef->postProcessing->SetCRTValue(CRTProperty::VerticalLine, verticalLine);
            editorEngineRef->postProcessing->SetCRTValue(CRTProperty::GrilleForce, grilleForce);
            editorEngineRef->postProcessing->SetCRTValue(CRTProperty::Noise, noise);
            editorEngineRef->postProcessing->SetCRTValue(CRTProperty::Fliker, fliker);
            editorEngineRef->postProcessing->SetGrilleTexture(grille);

            HighLightMemory(4080,11);
        }
    }

    void DrawPlayer()
    {
        ImVec2 playerSize = ImGui::GetWindowSize();
        ImVec2 buttonSize(30, 30);

        float buttonPosX = (playerSize.x - buttonSize.x * 4) / 2;
        float buttonPosY = (playerSize.y - buttonSize.y) / 2;

        ImGui::SetCursorPosX(buttonPosX);
        ImGui::SetCursorPosY(buttonPosY);

        if(ImGui::Button(ICON_FA_PLAY, buttonSize))
        {
            if (!Paused)
            {
                auto textToSave = codeEditor.GetText();
                SaveFileText(editorEngineRef->bios->GetFile().c_str(), (char *)textToSave.c_str());

                editorEngineRef->bios->ShouldRun = true;
            }

            Paused = false;
            DoStep = false;
        }
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_PAUSE, buttonSize))
        {
            Paused = !Paused;
        }
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_FORWARD_STEP, buttonSize))
        {
            Paused = true;
            DoStep = true;
        }
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_STOP, buttonSize))
        {
            Paused = false;
            DoStep = false;
            editorEngineRef->basicIntepreter->close();
            editorEngineRef->currentState = Off;
            editorEngineRef->basicIntepreter->CloseBas();
        }
    }

    void DrawMemory()
    {
        static char str0[128] = "memory";
        if (ImGui::Button("Load default"))
        {
            editorEngineRef->LoadDefaultMemory();
        }
        ImGui::InputText("New", str0, IM_ARRAYSIZE(str0));
        if (ImGui::Button("Save"))
        {
            std::stringstream ss;
            ss << ASSETS_FOLDER << "/" << editorEngineRef->bios->CurrentPath << "/" << str0 << MEM_EXTENSION;
            Tools::console->AddLog("[MEMORY] Saved:");
            Tools::console->AddLog(ss.str().c_str());
            editorEngineRef->DumpMemory(ss.str().c_str());
        }
        
        mem_edit.DrawContents(editorEngineRef->GetMemory(), 4096);
    }

    void PianoKey(ImVec2 pos, ImVec2 size, int note, bool isBlack, bool pressed) 
    {
        ImVec4 color = pressed ? ImVec4(.9f, .2f, .2f, 1.0f) : ImVec4(.9f, .9f, .9f, 1.0f);
        ImVec4 colorText = ImVec4(.1f, .1f, .1f, 1.0f);
        const char* formatText = "\n\n\n%d";

        if (isBlack)
        {
            color = pressed ? ImVec4(.6f, .2f, .2f, 1.0f) : ImVec4(.1f, .1f, .1f, 1.0f);
            colorText = ImVec4(.9f, .9f, .9f, 1.0f);
            formatText = "\n%d";
        }
        else
        {
            ImGui::SetItemAllowOverlap();
        }

        char button_label[32];
        sprintf(button_label, formatText , note + ((keyboardOctave - 4) * 12));
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        //draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), pressed ? IM_COL32(255, 20, 20, 255) : IM_COL32(255, 255, 255, 255), 2.0f);
        draw_list->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y - 2), ImGui::IsItemHovered() ?  IM_COL32(100, 100, 100, 255) : IM_COL32(20, 20, 20, 255), 2.0f, 0, 10);

        ImGui::SetCursorScreenPos(pos);

        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_Text, colorText);
        if (ImGui::Button(button_label, size)) 
        {
            editorEngineRef->audioManager->LoadSoundData(0);
            editorEngineRef->audioManager->SFXRender(0,note);
            editorEngineRef->audioManager->SFXPlay(0,255);
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }

    bool IsBlack(int note)
    {
        int n = note % 12;
        return (n == 1 || n == 3 || n == 6 || n == 8 || n == 10);
    }

    void DrawSFX()
    {
        static int id = 0;
        unsigned int dir = 3376 + (id * 22);

        int waveType = editorEngineRef->Peek(dir);

        int attackTimeValue   = editorEngineRef->Peek(dir+1);
        int sustainTimeValue  = editorEngineRef->Peek(dir+2);
        int sustainPunchValue = editorEngineRef->Peek(dir+3);
        int decayTimeValue    = editorEngineRef->Peek(dir+4);

        int slideValue        = Tools::ToSigned(editorEngineRef->Peek(dir+5));
        int deltaSlideValue   = Tools::ToSigned(editorEngineRef->Peek(dir+6));
        int vibratoDepthValue = editorEngineRef->Peek(dir+7);
        int vibratoSpeedValue = editorEngineRef->Peek(dir+8);

        int changeAmountValue = Tools::ToSigned(editorEngineRef->Peek(dir+9));
        int changeSpeedValue  = Tools::ToSigned(editorEngineRef->Peek(dir+10));
        int squareDutyValue   = editorEngineRef->Peek(dir+11);
        int dutySweepValue    = editorEngineRef->Peek(dir+12);

        int repeatSpeedValue  = editorEngineRef->Peek(dir+13);
        int phaserOffsetValue = Tools::ToSigned(editorEngineRef->Peek(dir+14));
        int phaserSweepValue  = Tools::ToSigned(editorEngineRef->Peek(dir+15));;    

        int lpfCutoffValue      = editorEngineRef->Peek(dir+16);
        int lpfCutoffSweepValue = Tools::ToSigned(editorEngineRef->Peek(dir+17));
        int lpfResonanceValue   = editorEngineRef->Peek(dir+18);
        int hpfCutoffValue      = editorEngineRef->Peek(dir+19);
        int hpfCutoffSweepValue = Tools::ToSigned(editorEngineRef->Peek(dir+20));

       

        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
        {
                for (auto& key : keyCharToKey)
                {
                    char thisChar = key.first;
                    int thisKey = key.second + keyboardOctave * 12;

                    if (IsKeyPressed(thisChar)) 
                    {
                        editorEngineRef->audioManager->LoadSoundData(id);
                        editorEngineRef->audioManager->SFXRender(id, thisKey);
                        editorEngineRef->audioManager->SFXPlay(id, 255);
                        pressedKey = thisKey;

                    }
                    if (IsKeyReleased(thisChar)) 
                    {
                        editorEngineRef->audioManager->SFXStop(id);
                        pressedKey = -1;
                    }
                }
                
                // key input - octave control
                if (IsKeyPressed('=') && keyboardOctave < 8) 
                {
                    keyboardOctave++;
                } 
                else if (IsKeyPressed('-') && keyboardOctave > 0) 
                {
                    keyboardOctave--;
                }
        }      
        ImVec2 white_key_pos = ImGui::GetCursorScreenPos();
        ImVec2 black_key_pos = white_key_pos;
       
        ImGui::BeginGroup();
            for (int i = 48; i <= 84; i++) 
            {
                if (!IsBlack(i))
                {
                    PianoKey(white_key_pos, ImVec2(WHITE_KEY_WIDTH, WHITE_KEY_HEIGHT), i, false, i == pressedKey);
                    white_key_pos.x += WHITE_KEY_WIDTH;
                } 
            }
        ImGui::EndGroup();        
 
        ImGui::BeginGroup();
            for (int i = 48; i <= 84; i++) 
            {
                if (IsBlack(i))
                {
                    PianoKey(ImVec2(black_key_pos.x - BLACK_KEY_WIDTH / 2, black_key_pos.y), 
                    ImVec2(BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT), i, true, i == pressedKey);
                    black_key_pos.x -= WHITE_KEY_WIDTH;
                }

                black_key_pos.x += WHITE_KEY_WIDTH;
            }
        ImGui::EndGroup();

        ImGui::SetCursorPos(ImVec2(10,WHITE_KEY_HEIGHT + 50));

ImGui::BeginChild("##scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);

        ImGui::BeginGroup();
            ImGui::Text("Envelope");
            if (ImGuiKnobs::KnobInt("Attack", &attackTimeValue, 0, 255, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+1, attackTimeValue);
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("Sustain", &sustainTimeValue, 0, 255, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+2, sustainTimeValue);
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("Punch", &sustainPunchValue, 0, 255, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+3, sustainPunchValue);
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("Decay", &decayTimeValue, 0, 255, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+4, decayTimeValue);
            }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        ImGui::BeginGroup();
            ImGui::Text("Frequence");
            if (ImGuiKnobs::KnobInt("Slide", &slideValue, -127, 127, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+5, Tools::ToSigned(slideValue));
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("Delta", &deltaSlideValue, -127, 127, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+6, Tools::ToSigned(deltaSlideValue));
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("V.Depht", &vibratoDepthValue, 0, 255, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+7, vibratoDepthValue);
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("V.Speed", &vibratoSpeedValue, 0, 255, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+8, vibratoSpeedValue);
            }
        ImGui::EndGroup();
        ImGui::Text("  ");
        ImGui::BeginGroup();
            ImGui::Text("Tone");
            if (ImGuiKnobs::KnobInt("Amount", &changeAmountValue, -127, 127, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+9, Tools::ToSigned(changeAmountValue));
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("C.Speed", &changeSpeedValue, -127, 127, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+10, Tools::ToSigned(changeSpeedValue));
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("S.Duty", &squareDutyValue, 0, 255, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+11, squareDutyValue);
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("D.Sweep", &dutySweepValue, 0, 255, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+12, dutySweepValue);
            }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        ImGui::BeginGroup();
            ImGui::Text("Repeat");
            if (ImGuiKnobs::KnobInt("R.Speed", &repeatSpeedValue, 0, 255, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+13, repeatSpeedValue);
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("R.Offset", &phaserOffsetValue, -127, 127, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+14, Tools::ToSigned(phaserOffsetValue));
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("R.Sweep", &phaserSweepValue, -127, 127, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+15, Tools::ToSigned(phaserSweepValue));
            }
        ImGui::EndGroup();
        ImGui::Text("  ");
        ImGui::BeginGroup();
            ImGui::Text("Filter");
            if (ImGuiKnobs::KnobInt("LPF Cut", &lpfCutoffValue, 0, 255, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+16, lpfCutoffValue);
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("LPF Swp", &lpfCutoffSweepValue, -127, 127, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+17, Tools::ToSigned(lpfCutoffSweepValue));
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("Reso", &lpfResonanceValue, 0, 255, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+18, lpfResonanceValue);
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("HPF Cut", &hpfCutoffValue, 0, 255, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+19, hpfCutoffValue);
            }
            ImGui::SameLine();
            if (ImGuiKnobs::KnobInt("HPF Swp", &hpfCutoffSweepValue, -127, 127, 1, "%03i", ImGuiKnobVariant_Stepped)) 
            {
                editorEngineRef->Poke(dir+20, Tools::ToSigned(hpfCutoffSweepValue));
            }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::Text("  ");
        ImGui::SameLine();
        ImGui::BeginGroup();
            ImGui::Text("Wave Type");
            if (ImGui::RadioButton("Sqr", waveType == 0)) { waveType = 0; editorEngineRef->Poke(dir, waveType);} ImGui::SameLine();
            if (ImGui::RadioButton("Saw", waveType == 1)) { waveType = 1; editorEngineRef->Poke(dir, waveType);} ImGui::SameLine();
            if (ImGui::RadioButton("Sin", waveType == 2)) { waveType = 2; editorEngineRef->Poke(dir, waveType);} ImGui::SameLine();
            if (ImGui::RadioButton("RND", waveType == 3)) { waveType = 3; editorEngineRef->Poke(dir, waveType);}
            ImGui::Text("  ");
            ImGui::DragInt("Sound Id", &id, 1, 0, 15, "%2i", ImGuiSliderFlags_AlwaysClamp);
        ImGui::EndGroup();
ImGui::EndChild();
    }

    void Draw()
    {
        ImGuiIO& io = ImGui::GetIO();
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Main", NULL, window_flags);
        ImGui::PopStyleVar();

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("View"))
                {
                    if (ImGui::MenuItem("XX")) 
                        Tools::console->AddLog("FFF");
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            static bool showDemo = false;

            ImGuiID dockspace_id = ImGui::GetID("DockId");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

            ImGui::Begin("Tools", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
                ImGui::DragFloat("Font", &io.FontGlobalScale, 0.05f, 0.5f, 2.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
                ImGui::SameLine();
                ImGui::Checkbox("Demo", &showDemo);
                if (showDemo)
                {
                    ImGui::ShowDemoWindow(&showDemo);
                }
            ImGui::End(); 
                
            //Console
            Tools::console->Draw();
            
            ImGui::Begin("File browser", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
                DrawshowFileBrowser();
            ImGui::End();    

            ImGui::Begin("FPS", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
                DrawFPS();
            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Screen", NULL, ImGuiWindowFlags_NoCollapse);
            ImGui::PopStyleVar();
                HasFocus = ImGui::IsWindowFocused();
                ImVec2 windowSize = ImGui::GetWindowSize();
                float scale = (windowSize.x/windowSize.y < 1.6f) ? windowSize.x/(float)GAME_SCREEN_W : windowSize.y/(float)GAME_SCREEN_H;
                ImVec2 imageSize = ImVec2(GAME_SCREEN_W * scale, GAME_SCREEN_H * scale); 
                ImVec2 imagePos = ImVec2((windowSize.x - imageSize.x) / 2, (windowSize.y - imageSize.y) / 2);

                ImGui::SetCursorPos(imagePos);

                ImGui::Image(&editorEngineRef->postProcessing->editorRender.texture, imageSize, ImVec2(0,0),
                                ImVec2(editorEngineRef->postProcessing->editorImageFactor,-editorEngineRef->postProcessing->editorImageFactor));
            ImGui::End();

            ImGui::Begin("Palette", NULL, ImGuiWindowFlags_NoCollapse);
                DrawPalette();
            ImGui::End();

            ImGui::Begin("CRT", NULL, ImGuiWindowFlags_NoCollapse); 
                DrawCRT();
            ImGui::End();
            
            ImVec2 minHeight(60, 100);
            ImVec2 maxHeight(2000, 100);
            ImGui::SetNextWindowSizeConstraints(minHeight, maxHeight);
            ImGui::Begin("Player", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
                DrawPlayer();
            ImGui::End();

            ImGui::Begin("Memory", NULL, ImGuiWindowFlags_NoCollapse);
                DrawMemory();
            ImGui::End();

            ImGui::Begin("Sprites", NULL, ImGuiWindowFlags_NoCollapse);
                ImGui::BeginGroup();
                    DrawGraphics();
                ImGui::EndGroup();
                //windowSize = ImGui::GetWindowSize();
                //scale = (windowSize.x/windowSize.y < 1.0f) ? windowSize.x/128.0f : windowSize.y/128.0f;
                //rlImGuiImageRect(&editorEngineRef->spriteManager->spriteTexture, 128 * scale, 128 * scale, (Rectangle){0, 0, 128, 128});
            ImGui::End();

            ImGui::Begin("SFX", NULL, ImGuiWindowFlags_NoCollapse);
                DrawSFX();
            ImGui::End();

            DrawCode();

            //Hack ¿?¿?¿?, if you remove this, ImGui fails. ¯\_(ツ)_/¯
            rlImGuiImageRect(&hackTexture, 1, 1, (Rectangle){0, 0, 1, 1});

       ImGui::End();
    }

};
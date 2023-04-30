#pragma once
#include "imgui/imgui.h"
#include "imgui/rlImGui.h"
#include "imgui/imgui_memory_editor.h"

#include "engine.h"
#include "bios.h"
#include "TextEditor.h"
#include "postprocessing.h"
#include "mb_manager.h"
#include "sprite_manager.h"

struct Editor
{
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
    }

    ~Editor()
    {

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
        }
    }

    void DrawCRT()
    {
        bool ppState = editorEngineRef->postProcessing->enabled;
        ImGui::Checkbox("Enabled", &ppState);
        editorEngineRef->postProcessing->SetState(ppState);
        
        static int blurPower    = editorEngineRef->Peek(4080);
        static int blurFactor   = editorEngineRef->Peek(4081);
        static int chromatic    = editorEngineRef->Peek(4082);
        static int curvature    = editorEngineRef->Peek(4083);
        static int vignetting   = editorEngineRef->Peek(4084);
        static int scanLine     = editorEngineRef->Peek(4085);
        static int verticalLine = editorEngineRef->Peek(4086);
        static int grilleForce  = editorEngineRef->Peek(4087);
        static int noise        = editorEngineRef->Peek(4088);
        static int fliker       = editorEngineRef->Peek(4089);
        static int grille       = editorEngineRef->Peek(4090);

        ImGui::DragInt("Blur Power", &blurPower, 1, 0, 255, "%3i", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragInt("Blur Factor", &blurFactor, 1, 0, 255, "%3i", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragInt("Chromatic", &chromatic, 1, 0, 255, "%3i", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragInt("Curvature", &curvature, 1, 0, 255, "%3i", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragInt("Vignetting", &vignetting, 1, 0, 255, "%3i", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragInt("Scanline", &scanLine, 1, 0, 255, "%3i", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragInt("Vertical Line", &verticalLine, 1, 0, 255, "%3i", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragInt("Grille Force", &grilleForce, 1, 0, 255, "%3i", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragInt("Noise", &noise, 1, 0, 255, "%3i", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragInt("Flicker", &fliker, 1, 0, 255, "%3i", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragInt("Grille", &grille, 1, 0, 2, "%1i", ImGuiSliderFlags_AlwaysClamp);

        editorEngineRef->postProcessing->SetCRTFloat(CRTProperty::BlurPower, blurPower);
        editorEngineRef->postProcessing->SetCRTFloat(CRTProperty::BlurFactor, blurFactor);
        editorEngineRef->postProcessing->SetCRTFloat(CRTProperty::Chromatic, chromatic);
        editorEngineRef->postProcessing->SetCRTFloat(CRTProperty::Curvature, curvature);
        editorEngineRef->postProcessing->SetCRTFloat(CRTProperty::Vignetting, vignetting);
        editorEngineRef->postProcessing->SetCRTFloat(CRTProperty::ScanLine, scanLine);
        editorEngineRef->postProcessing->SetCRTFloat(CRTProperty::VerticalLine, verticalLine);
        editorEngineRef->postProcessing->SetCRTFloat(CRTProperty::GrilleForce, grilleForce);
        editorEngineRef->postProcessing->SetCRTFloat(CRTProperty::Noise, noise);
        editorEngineRef->postProcessing->SetCRTFloat(CRTProperty::Fliker, fliker);
        editorEngineRef->postProcessing->SetGrilleTexture(grille);
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

    void Draw()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags window_flags;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus ;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin(editorEngineRef->GetEngineName(), NULL, window_flags);
        ImGui::PopStyleVar();

            static bool showDemo = false;

            ImGuiID dockspace_id = ImGui::GetID("DockId");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

            ImGui::Begin("Tools", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
                ImGui::DragFloat("Font", &io.FontGlobalScale, 0.01f, 0.5f, 2.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
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

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Graphics", NULL, ImGuiWindowFlags_NoCollapse);
            ImGui::PopStyleVar();
                windowSize = ImGui::GetWindowSize();
                scale = (windowSize.x/windowSize.y < 1.0f) ? windowSize.x/128.0f : windowSize.y/128.0f;
                rlImGuiImageRect(&editorEngineRef->spriteManager->spriteTexture, 128 * scale, 128 * scale, (Rectangle){0, 0, 128, 128});
            ImGui::End();

            DrawCode();

        ImGui::End();
    }

};
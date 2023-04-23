#pragma once
#include "imgui/imgui.h"
#include "imgui/rlImGui.h"

#include "engine.h"
#include "bios.h"
#include "TextEditor.h"

struct Editor
{
    Engine* editorEngineRef;
    TextEditor codeEditor;
    TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::Basic();
    std::string editorFile = "";

    Editor(Engine* _engine)
    {
        editorEngineRef = _engine;

        codeEditor.SetLanguageDefinition(lang);
        codeEditor.SetPalette(TextEditor::GetBasicPalette());
    }

    ~Editor()
    {

    }

void inline DrawFPS(bool * showInfo)
{
    
    ImGui::Begin("Info", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::SeparatorText("FPS");

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

        ImGui::SeparatorText("Program");
        ImGui::Text("Current Program: [ %s ]\n", editorEngineRef->bios->CurrentProgram.c_str());
        ImGui::SeparatorText("Memory");
        ImGui::Text("Current Memory:");

    ImGui::End();
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

void DrawshowFileBrowser(bool * show)
{
        //CenterWindow();
        ImGui::Begin("File browser", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
            
            ImGuiStyle& style = ImGui::GetStyle();
            ImVec2 pw_size = ImGui::GetWindowSize();
            float list_item_height = ImGui::CalcTextSize("").y + (style.ItemSpacing.y * 4.0f);
            float input_bar_ypos = pw_size.y - ImGui::GetFrameHeightWithSpacing() * 2.5f - style.WindowPadding.y;
            float window_height = input_bar_ypos - ImGui::GetCursorPosY() - style.ItemSpacing.y;
            float window_content_height = window_height - style.WindowPadding.y * 2.0f;
            float min_content_size = pw_size.x - style.WindowPadding.x * 4.0f;

            ImGui::BeginChild("#Head",ImVec2(0, list_item_height), true, ImGuiWindowFlags_NoScrollWithMouse);
                ImGui::Text("Current path: %s", editorEngineRef->bios->CurrentPath.c_str());
            ImGui::EndChild();

            ImGui::BeginChild("#Files", ImVec2(0, window_content_height), true);

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

                ss = Tools::GetFiles(editorEngineRef->bios->CurrentPath.c_str());
                while (std::getline(ss, temp))
                {
                    if(ImGui::Selectable(temp.c_str(), false))
                    {
                        editorEngineRef->bios->CurrentProgram = temp;
                    }
                }
            
            ImGui::EndChild();

            ImGui::BeginChild("#Foot",ImVec2(0, list_item_height), true, ImGuiWindowFlags_NoScrollWithMouse);
                ImGui::Text("Current program: %s", editorEngineRef->bios->CurrentProgram.c_str());
            ImGui::EndChild();

        ImGui::End();
        
}

void Draw()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |ImGuiWindowFlags_NoBackground ;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(editorEngineRef->GetEngineName(), NULL, window_flags);
    ImGui::PopStyleVar();

    static bool showConsole = true;
    static bool showDemo = true;
    static bool showFileBrowser = true;
    static bool showInfo = true;
    static bool showPalette = true;
    static bool showCode = true;

    ImGuiID dockspace_id = ImGui::GetID("DockId");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New"))
            {
                editorEngineRef->bios->CurrentProgram = "new";
            }

            ImGui::MenuItem("Save");
            ImGui::MenuItem("Save as");
            ImGui::EndMenu();
        }
    
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("DemoOpen", "", &showDemo);
            if (ImGui::MenuItem("FileBrowser", "", &showFileBrowser));
            if (ImGui::MenuItem("Info", "", &showInfo));
            ImGui::MenuItem("Console", "", &showConsole);
            ImGui::EndMenu();
        }

        if(ImGui::SmallButton("RUN"))
        {
            editorEngineRef->bios->ShouldRun = true;
        }

        ImGui::DragFloat("Font scale", &io.FontGlobalScale, 0.01f, 0.5f, 2.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

        ImGui::EndMenuBar();
    }

    if (showConsole)
    {
        Tools::console->Draw("Console", &showConsole);
    }

    if (showDemo)
    {
        ImGui::ShowDemoWindow(&showDemo);
    }

    if (showFileBrowser)
    {
        DrawshowFileBrowser(&showFileBrowser);
    }
     
    if (showInfo)
    {
        DrawFPS(&showInfo);
    }

ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
ImGui::Begin("Screen");
ImGui::PopStyleVar();
    ImVec2 windowSize = ImGui::GetWindowSize();
    float scale = (windowSize.x/windowSize.y < 1.6f) ? windowSize.x/320.0f : windowSize.y/200.0f;
    rlImGuiImageRect(&editorEngineRef->postProcessing->mainRender.texture, 320 * scale, 200 * scale, (Rectangle){0, 200, 320, 200});
ImGui::End();

ImGui::Begin("Palette");
    for (char c = 0; c<16; c++)
    {
        char buffer [50];
        sprintf (buffer, "[%i]", c);
        Color col = Tools::GetColor(c);
        ImVec4 color = ImVec4(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, 1.0f);
        ImGui::ColorEdit3(buffer, (float*)&color, 0);
        Tools::SetColor(c, color.x * 255, color.y * 255, color.z * 255);
    }
ImGui::End();

ImGui::Begin("CRT");
    bool ppState = editorEngineRef->postProcessing->enabled;
    ImGui::Checkbox("Enabled", &ppState);
    editorEngineRef->postProcessing->SetState(ppState);
ImGui::End();

    //if (ImGui::CollapsingHeader("Editor"))
    {
        if (editorEngineRef->bios->CurrentProgram != editorFile)
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
            
        }

		auto cpos = codeEditor.GetCursorPosition();

		ImGui::Begin("Code Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
                if (ImGui::MenuItem("Test ERROR"))
				{
					TextEditor::ErrorMarkers markers;
                    markers.insert(std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
                    markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
                    codeEditor.SetErrorMarkers(markers);
                    TextEditor::Coordinates coord;
                    coord.mLine = 6;
                    coord.mColumn = 0;
                    codeEditor.SetCursorPosition(coord);
				}
				if (ImGui::MenuItem("Save"))
				{
					auto textToSave = codeEditor.GetText();
                    SaveFileText(editorEngineRef->bios->GetFile().c_str(), (char *)textToSave.c_str());
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				bool ro = codeEditor.IsReadOnly();
				if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
					codeEditor.SetReadOnly(ro);
				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && codeEditor.CanUndo()))
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

		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s  | %s", cpos.mLine + 1, cpos.mColumn + 1, codeEditor.GetTotalLines(),
			codeEditor.IsOverwrite() ? "Ovr" : "Ins",
			codeEditor.CanUndo() ? "*" : " ",
			codeEditor.GetLanguageDefinition().mName.c_str(),
            editorEngineRef->bios->CurrentProgram.c_str());

        codeEditor.Render("TextEditor");

        ImGui::End();
    }
 
    ImGui::End();
}
};
#pragma once
#include "imgui/imgui.h"
#include "imgui/rlImGui.h"

#include "engine.h"
#include "TextEditor.h"

struct Editor
{
    Engine* editorEngineRef;
    TextEditor editor;
    TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::Basic();
    std::string editorFile = "";

    Editor(Engine* _engine)
    {
        editorEngineRef = _engine;

        editor.SetLanguageDefinition(lang);
        editor.SetPalette(TextEditor::GetBasicPalette());
    }

    ~Editor()
    {

    }

    void Draw()
    {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        ImGui::Begin("XXX");
        ImGui::End();
    }

//ImGui

void inline drawFPS()
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
    sprintf(overlay, "average framerate: %.3f fps", average);
    ImGui::PlotLines("FPS", values, IM_ARRAYSIZE(values), values_offset,overlay ,0.0f, 60.0f, ImVec2(0, 80.0f));
    ImGui::PopStyleColor();    
}

void ShowFontSelector(const char* label)
{
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font_current = ImGui::GetFont();
    if (ImGui::BeginCombo(label, font_current->GetDebugName()))
    {
        for (int n = 0; n < io.Fonts->Fonts.Size; n++)
        {
            ImFont* font = io.Fonts->Fonts[n];
            ImGui::PushID((void*)font);
            if (ImGui::Selectable(font->GetDebugName(), font == font_current))
                io.FontDefault = font;
            ImGui::PopID();
        }
        ImGui::EndCombo();
    }
}

void DrawExplorer()
{
        ImGui::SeparatorText("Program");
        ImGui::Text("Current Paht:[ %s ]\n", editorEngineRef->bios->CurrentPath.c_str());
        ImGui::Text("Current Program: [ %s ]\n", editorEngineRef->bios->CurrentProgram.c_str());
        if(ImGui::Button("RUN"))
        {
            editorEngineRef->bios->ShouldRun = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("NEW"))
        {
            editorEngineRef->bios->CurrentProgram = "new";
        }
        if(ImGui::Button("Up"))
        {
            editorEngineRef->bios->RemoveSubPath();
        }

        std::stringstream ss = Tools::GetFolders(editorEngineRef->bios->CurrentPath.c_str());
        std::string temp;
        while (std::getline(ss, temp)){
            if(ImGui::Button(temp.c_str())){
                editorEngineRef->bios->AddSubPath(temp);
            }
        }
        ss = Tools::GetFiles(editorEngineRef->bios->CurrentPath.c_str());
        while (std::getline(ss, temp)){
            if(ImGui::Button(temp.c_str())){
                editorEngineRef->bios->CurrentProgram=temp;
            }
        }
}
void DrawImGui()
{

    ImGui::SetNextWindowSize(ImVec2(250, 500),ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(20, 20),ImGuiCond_FirstUseEver);


    ImGui::Begin(editorEngineRef->GetEngineName());
    ShowFontSelector("Font");
    
    static bool showConsole = true;
    ImGui::Checkbox("Console", &showConsole);

    if (showConsole){
        Tools::console->Draw("Console", &showConsole);
    }    

    if (ImGui::CollapsingHeader("Explorer"))
    {
        DrawExplorer();
    }

    if (ImGui::CollapsingHeader("Info")){
        ImGui::SeparatorText("FPS");
        drawFPS();
        ImGui::SeparatorText("Program");
        ImGui::Text("Current Program: [ %s ]\n", editorEngineRef->bios->CurrentProgram.c_str());
        ImGui::SeparatorText("Memory");
        ImGui::Text("Current Memory:");
    }

    if (ImGui::CollapsingHeader("Palette"))
    {
        for (char c = 0; c<16; c++)
        {
            char buffer [50];
            sprintf (buffer, "[%i]", c);
            Color col = Tools::GetColor(c);
            ImVec4 color = ImVec4(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, 1.0f);
            ImGui::ColorEdit3(buffer, (float*)&color, 0);
            Tools::SetColor(c, color.x * 255, color.y * 255, color.z * 255);
        }
    }
    if (ImGui::CollapsingHeader("Graphics"))
    {
    }
    if (ImGui::CollapsingHeader("Inputs"))
    {
    }
    if (ImGui::CollapsingHeader("CRT filter"))
    {
        bool ppState = editorEngineRef->postProcessing->enabled;
        ImGui::Checkbox("Enabled", &ppState);
        editorEngineRef->postProcessing->SetState(ppState);
    }
    if (ImGui::CollapsingHeader("SFX synth"))
    {
    }
    if (ImGui::CollapsingHeader("Music"))
    {
    }
    if (ImGui::CollapsingHeader("Online"))
    {
    }


    if (ImGui::CollapsingHeader("Editor"))
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
            editor.SetText(str);
            editorFile = editorEngineRef->bios->CurrentProgram;
            
        }

		auto cpos = editor.GetCursorPosition();

        ImGui::SetNextWindowSize(ImVec2(400, 500),ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(80, 80),ImGuiCond_FirstUseEver);
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
                    editor.SetErrorMarkers(markers);
                    TextEditor::Coordinates coord;
                    coord.mLine = 6;
                    coord.mColumn = 0;
                    editor.SetCursorPosition(coord);
				}
				if (ImGui::MenuItem("Save"))
				{
					auto textToSave = editor.GetText();
                    SaveFileText(editorEngineRef->bios->GetFile().c_str(), (char *)textToSave.c_str());
				}
				if (ImGui::MenuItem("pushFont"))
				{
					
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				bool ro = editor.IsReadOnly();
				if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
					editor.SetReadOnly(ro);
				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
					editor.Undo();
				if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
					editor.Redo();

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
					editor.Copy();
				if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
					editor.Cut();
				if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
					editor.Delete();
				if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
					editor.Paste();

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", nullptr, nullptr))
					editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark palette"))
					editor.SetPalette(TextEditor::GetDarkPalette());
				if (ImGui::MenuItem("Light palette"))
					editor.SetPalette(TextEditor::GetLightPalette());
				if (ImGui::MenuItem("Retro blue palette"))
					editor.SetPalette(TextEditor::GetRetroBluePalette());
                if (ImGui::MenuItem("Basic palette"))
					editor.SetPalette(TextEditor::GetBasicPalette());
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s  | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
			editor.IsOverwrite() ? "Ovr" : "Ins",
			editor.CanUndo() ? "*" : " ",
			editor.GetLanguageDefinition().mName.c_str(),
            editorEngineRef->bios->CurrentProgram.c_str());

        editor.Render("TextEditor");

        ImGui::End();
    }
    ImGui::End();
}
};
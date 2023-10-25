#include "editor.h"
#include "nlohmann/json.hpp"
#include <cstdlib>
#include <iostream>
#include "iconTexture.h"

static bool show_tools = false;
static bool show_demo = false;
static bool show_FPS = false;
static bool show_filebrowser = false;
static bool show_code = false;
static bool show_palette = false;
static bool show_crt = false;
static bool show_console = false;
static bool show_sfx = false;
static bool show_sprites = false;
static bool show_makeSprite = false;
static bool show_metaSprite = false;
static bool show_screen = false;
static bool show_player = false;
static bool show_memory = false;
static bool show_credits = false;
static bool show_docs = false;
static bool codeHelperOpen = false;

static unsigned char spriteCopy[8]{};
static unsigned char metaSpriteCopy[20]{};
static unsigned char metaLineCopy[5]{};

int currentSprite = 0;
int currentMetaSprite = 0;
nlohmann::json data;

static float mouseEditorPositionX;
static float mouseEditorPositionY;
static float mouseEditorOffset;
static float iScale;

static Color drawMetaExampleColor;
int currentLayout = 0;

Documentation docs;

bool Editor::CheckJsonFile()
{
	std::stringstream ss;
	ss << CONFIG_FOLDER << "/ui.json";

    std::ifstream f(ss.str().c_str());
    if (FileExists(ss.str().c_str()))
    {
        data = nlohmann::json::parse(f);
        f.close();
        return true;
    }
    Enabled = false;
    return false;
}

void Editor::LoadUIJson()
{
    if(!Enabled) return;
    if (!CheckJsonFile()) return;

    static const char* layouts[5] = {"layout_1", "layout_2", "layout_3", "layout_4", "layout_5"};
    const char* layout = layouts[currentLayout];

    show_player         = data[layout]["show_player"].get<bool>();
    show_FPS            = data[layout]["show_fps"].get<bool>();
    show_tools          = data[layout]["show_tools"].get<bool>();
    show_screen         = data[layout]["show_screen"].get<bool>();
    show_code           = data[layout]["show_code"].get<bool>();
    show_console        = data[layout]["show_console"].get<bool>();
    show_palette        = data[layout]["show_palette"].get<bool>();
    show_crt            = data[layout]["show_crt"].get<bool>();
    show_memory         = data[layout]["show_memory"].get<bool>();
    show_filebrowser    = data[layout]["show_filebrowser"].get<bool>();
    show_sprites        = data[layout]["show_sprites"].get<bool>();
    show_makeSprite     = data[layout]["show_makeSprite"].get<bool>();
    show_metaSprite     = data[layout]["show_metaSprite"].get<bool>();
    show_sfx            = data[layout]["show_sfx"].get<bool>();
    show_docs           = data[layout]["show_docs"].get<bool>();
    
    ImGui::LoadIniSettingsFromDisk(Tools::GetCurrentLayout(currentLayout));
}

void Editor::SaveUIJson()
{
    if(!Enabled) return;
    if (!CheckJsonFile()) return;

    ImGuiIO& io = ImGui::GetIO();
    const char* layoutFilename = Tools::GetCurrentLayout(currentLayout);
    ImGui::SaveIniSettingsToDisk(layoutFilename);
    io.WantSaveIniSettings = false;
    
    static const char* layouts[5] = {"layout_1", "layout_2", "layout_3", "layout_4", "layout_5"};
    const char* layout = layouts[currentLayout];

    data["window_width"]        = GetScreenWidth();
    data["window_height"]       = GetScreenHeight();
    data["window_position_x"]   = window_position_x;
    data["window_position_y"]   = window_position_y;
    data["currentLayout"]       = currentLayout;

    data[layout]["show_player"]         = show_player;
    data[layout]["show_fps"]            = show_FPS;
    data[layout]["show_tools"]          = show_tools;
    data[layout]["show_screen"]         = show_screen;
    data[layout]["show_code"]           = show_code;
    data[layout]["show_console"]        = show_console;
    data[layout]["show_palette"]        = show_palette;
    data[layout]["show_crt"]            = show_crt;
    data[layout]["show_memory"]         = show_memory;
    data[layout]["show_filebrowser"]    = show_filebrowser;
    data[layout]["show_sprites"]        = show_sprites;
    data[layout]["show_makeSprite"]     = show_makeSprite;
    data[layout]["show_metaSprite"]     = show_metaSprite;
    data[layout]["show_sfx"]            = show_sfx;
    data[layout]["show_docs"]           = show_docs;

    std::stringstream ss;
	ss << CONFIG_FOLDER << "/ui.json";
    std::ofstream o(ss.str().c_str());
    o << std::setw(4) << data << std::endl;
    o.close();
}

void Editor::CloseAll()
{
    show_tools = false;
    show_demo = false;
    show_FPS = false;
    show_filebrowser = false;
    show_code = false;
    show_palette = false;
    show_crt = false;
    show_console = false;
    show_sfx = false;
    show_sprites = false;
    show_makeSprite = false;
    show_metaSprite = false;
    show_screen = false;
    show_player = false;
    show_memory = false;
    show_credits = false;
    show_docs = false;
}

Editor::Editor(Engine* _engine)
{
    editorEngineRef = _engine;
    codeEditor.SetLanguageDefinition(lang);
    codeEditor.SetTabSize(4);

    mem_edit.HighlightColor = IM_COL32Vec4(Col_P_B1);
    mem_edit.OptShowAscii = false;

    Image hackImage = GenImageColor(1,1,(Color){0,0,0,0});
    hackTexture = LoadTextureFromImage(hackImage);
    UnloadImage(hackImage);

    if (CheckJsonFile()) 
    {
        currentLayout = data["currentLayout"].get<int>();
    }

    //ExportImageAsCode(LoadImage("icon.png"), "iconTexture.h");
    iconTexture = Tools::TextureFromCode(ICONTEXTURE_FORMAT, ICONTEXTURE_HEIGHT, ICONTEXTURE_WIDTH, ICONTEXTURE_DATA, 1); 
    SetTextureFilter(iconTexture, TEXTURE_FILTER_POINT);
    SetMainWindow();
    
    LoadUIJson();

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 6.0f;
    style.WindowRounding = 6.0f;
    style.ChildRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.TabRounding = 12.0f;
    style.GrabRounding = 3.0f;
    style.GrabMinSize = 20.0f;
    style.ScrollbarSize = 20.0f;
    style.WindowMenuButtonPosition = ImGuiDir_Right;
    style.SelectableTextAlign = ImVec2(0.02f, 0.0f);

    SetImGuiColors();
    
}

void Editor::SetImGuiColors()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                   = Col_T_1;
    colors[ImGuiCol_TextDisabled]           = Col_T_2;
    colors[ImGuiCol_WindowBg]               = Col_P_D1;
    colors[ImGuiCol_ChildBg]                = Col_P_D2;
    colors[ImGuiCol_PopupBg]                = Col_P_D2;
    colors[ImGuiCol_Border]                 = Col_P_M;
    colors[ImGuiCol_FrameBg]                = Col_P_M;
    colors[ImGuiCol_FrameBgHovered]         = Col_P_B2;
    colors[ImGuiCol_FrameBgActive]          = Col_P_B1;
    colors[ImGuiCol_TitleBg]                = Col_S2_M;
    colors[ImGuiCol_TitleBgActive]          = Col_S2_B2;
    colors[ImGuiCol_TitleBgCollapsed]       = Col_P_D2;
    colors[ImGuiCol_MenuBarBg]              = Col_S2_D2;
    colors[ImGuiCol_ScrollbarBg]            = Col_S1_D2;
    colors[ImGuiCol_ScrollbarGrab]          = Col_S1_M;
    colors[ImGuiCol_ScrollbarGrabHovered]   = Col_S1_B2;
    colors[ImGuiCol_ScrollbarGrabActive]    = Col_S1_B1;
    colors[ImGuiCol_CheckMark]              = Col_S2_B1;
    colors[ImGuiCol_SliderGrab]             = Col_S2_M;
    colors[ImGuiCol_SliderGrabActive]       = Col_S2_B1;
    colors[ImGuiCol_Button]                 = Col_S2_M;
    colors[ImGuiCol_ButtonHovered]          = Col_S2_B2;
    colors[ImGuiCol_ButtonActive]           = Col_S2_B1;
    colors[ImGuiCol_Header]                 = Col_S1_M;
    colors[ImGuiCol_HeaderHovered]          = Col_S1_B2;
    colors[ImGuiCol_HeaderActive]           = Col_S1_B1;
    colors[ImGuiCol_Separator]              = Col_P_D2;
    colors[ImGuiCol_SeparatorHovered]       = Col_P_B2;
    colors[ImGuiCol_SeparatorActive]        = Col_P_B1;
    colors[ImGuiCol_ResizeGrip]             = Col_S1_M;
    colors[ImGuiCol_ResizeGripHovered]      = Col_S1_B1;
    colors[ImGuiCol_ResizeGripActive]       = Col_S1_B1;
    colors[ImGuiCol_Tab]                    = Col_S1_M;
    colors[ImGuiCol_TabHovered]             = Col_S1_B2;
    colors[ImGuiCol_TabActive]              = Col_S1_B1;
    colors[ImGuiCol_TabUnfocused]           = Col_S1_D2;
    colors[ImGuiCol_TabUnfocusedActive]     = Col_S1_D1;
    colors[ImGuiCol_DockingPreview]         = Col_P_B1;
    colors[ImGuiCol_PlotLines]              = Col_S1_B2;
    colors[ImGuiCol_PlotLinesHovered]       = Col_S1_B1;
    colors[ImGuiCol_DockingEmptyBg]         = Col_P_D2;
    colors[ImGuiCol_TextSelectedBg]         = Col_P_B1;
}

inline ImVec4 hexToVec3(const std::string& hex) 
{     
    std::string redStr = hex.substr(0, 2);
    std::string greenStr = hex.substr(2, 2);
    std::string blueStr = hex.substr(4, 2);

    // Convierte los componentes hexadecimales en valores de punto flotante
    float r = std::stoul(redStr, nullptr, 16) / 255.0f;
    float g = std::stoul(greenStr, nullptr, 16) / 255.0f;
    float b = std::stoul(blueStr, nullptr, 16) / 255.0f;

    return ImVec4(r, g, b, 1.0f);
}

void Editor::LoadEditorPalette(std::string colors[17])
{
    //Text
    Col_T_1 = hexToVec3(colors[0]);
    Col_T_2 = hexToVec3(colors[1]);
    //Primary
    Col_P_B1 = hexToVec3(colors[2]);
    Col_P_B2 = hexToVec3(colors[3]);
    Col_P_M  = hexToVec3(colors[4]);
    Col_P_D1 = hexToVec3(colors[5]);
    Col_P_D2 = hexToVec3(colors[6]);

    //Secondary 1
    Col_S1_B1 = hexToVec3(colors[7]);
    Col_S1_B2 = hexToVec3(colors[8]);
    Col_S1_M  = hexToVec3(colors[9]);
    Col_S1_D1 = hexToVec3(colors[10]);
    Col_S1_D2 = hexToVec3(colors[11]);

    //Secondary 2
    Col_S2_B1 = hexToVec3(colors[12]);
    Col_S2_B2 = hexToVec3(colors[13]);
    Col_S2_M  = hexToVec3(colors[14]);
    Col_S2_D1 = hexToVec3(colors[15]);
    Col_S2_D2 = hexToVec3(colors[16]);

    SetImGuiColors();
}

Editor::~Editor()
{
    ChangeLayout(0);
    SaveCurrentFile();
    UnloadTexture(iconTexture);
}

void Editor::SaveCurrentFile()
{
    if (editorEngineRef->bios->CurrentProject.name.size() > 0)
    {
        Tools::console->AddLog("Autosaving: [%s]", editorEngineRef->bios->CurrentProject.programFile.c_str());
        auto textToSave = codeEditor.GetText();

        size_t lastChar = textToSave.find_last_not_of('\n');

        if (lastChar != std::string::npos)
        {
            textToSave = textToSave.substr(0, lastChar + 1);
        }

        SaveFileText(editorEngineRef->bios->CurrentProject.programFile.c_str(), (char *)textToSave.c_str());
        editorEngineRef->bios->TryToSaveMemory();
    }
}

void Editor::SetMainWindow()
{
    BeginDrawing();
        ClearBackground(BLACK);
    EndDrawing();
    if(Enabled)
    {
        SetWindowPosition(data["window_position_x"].get<int>(), data["window_position_y"].get<int>());
        SetWindowSize(data["window_width"].get<int>(), data["window_height"].get<int>());
    }
    else
    {
        SetWindowSize(GAME_SCREEN_W*4, GAME_SCREEN_H*4);
        int monitor = GetCurrentMonitor();
        SetWindowPosition(GetMonitorWidth(monitor) / 2 - GetScreenWidth() / 2, 
            GetMonitorHeight(monitor) / 2 - GetScreenHeight() / 2 );
    }
}

void Editor::HighLightMemory(uint16_t address, uint16_t size)
{
    mem_edit.GotoAddr = address;
    mem_edit.HighlightMin = address;
    mem_edit.HighlightMax = address + size;
    mem_edit.DataEditingAddr = mem_edit.DataPreviewAddr = -1;
    mem_edit.DataEditingTakeFocus = false;
}

void Editor::DrawFPS()
{
    ImVec2 pw_size = ImGui::GetWindowSize();
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
    ImGui::PushStyleColor(ImGuiCol_FrameBg, Col_P_D2);
    ImGui::PlotLines("", values, IM_ARRAYSIZE(values), values_offset,overlay ,
                        0.0f, 60.0f, ImVec2(pw_size.x, pw_size.y-20)); 
    ImGui::PopStyleColor();
}

void Editor::Link(const char* text, const char* link, float size)
{ 
    if (ImGui::Button(text, ImVec2(size, 0.0f)))
    {
        std::system((std::string("start ") + link).c_str());
    }
}

void Editor::Credits()
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    

    if (!ImGui::IsPopupOpen("Credits"))
        ImGui::OpenPopup("Credits");
    float c;
    if(ImGui::BeginPopupModal("Credits", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {    
        c = ImGui::GetWindowWidth() / 2.0f;

        ImGui::SetCursorPosX(c - 40);
        ImGui::Image(&iconTexture, ImVec2(80,80));

        const char * pbp = "Programmed by Pery - 2023";
        ImGui::SetCursorPosX(c - ImGui::CalcTextSize(pbp).x / 2.0f);
        ImGui::TextColored(Col_S2_B1, pbp);
        ImGui::SetCursorPosX(c - 80);
        Link("Site", "https://zaroa.net/Enginsito", 80);
        ImGui::SameLine();
        Link("  X  ", "https://x.com/pery36", 80);
        ImGui::Separator();

        ImGui::Text("This application utilizes the following open-source libraries:");
        ImGui::Spacing();
        c = 700;
        Link("Raylib", "https://github.com/raysan5/raylib", c);
        Link("My-Basic", "https://github.com/paladin-t/my_basic/", c);
        Link("ImGui", "https://github.com/ocornut/imgui", c);
        Link("Json", "https://github.com/nlohmann/json", c);
        Link("Memory", "https://github.com/ocornut/imgui_club", c);
        Link("Text Editor", "https://github.com/BalazsJako/ImGuiColorTextEdit", c);
        Link("Imgui Knobs", "https://github.com/altschuler/imgui-knobs", c);
        Link("rlImGui", "https://github.com/raylib-extras/rlImGui", c);
        Link("MML_Parser", "https://github.com/vcraftjp/MML-Parser", c);
        Link("FontAwesome", "https://fontawesome.com/", c);
        Link("Font", "https://www.fontspace.com/mozart-nbp-font-f18977", c);
        Link("Palettes", "https://lospec.com/palette-list", c);
        Link("MML Music Library", "https://archeagemmllibrary.com/", c);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Thanks to:");
        ImGui::Text("Deebrol, David, Alberto, Maza, ChatGPT,\nand all the people who ask and answer things on the internet.");

        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x + ImGui::GetWindowContentRegionMin().x - 120) * 0.5f);
        if (ImGui::Button("OK", ImVec2(120, 0))) 
        { 
            ImGui::CloseCurrentPopup();
            show_credits = false;
        }

        ImGui::SetItemDefaultFocus();

        ImGui::EndPopup();
    }
}

void Editor::OpenFile()
{
    Tools::console->AddLog("Open: [ %s ]\n", editorEngineRef->bios->CurrentProject.name.c_str());

    std::ifstream inFile;
    inFile.open(editorEngineRef->bios->CurrentProject.programFile.c_str());
    std::stringstream strStream;
    strStream << inFile.rdbuf();
    std::string str = strStream.str();
    codeEditor.SetText(str);
    editorFile = editorEngineRef->bios->CurrentProject.name;
    DoStep = false;
    inFile.close();
}

void Editor::DrawshowFileBrowser()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 pw_size = ImGui::GetWindowSize();
    float list_item_height = ImGui::CalcTextSize("").y + (style.ItemSpacing.y * 4.0f);
    float input_bar_ypos = pw_size.y - ImGui::GetFrameHeightWithSpacing() * 3.5f - style.WindowPadding.y;
    float window_height = input_bar_ypos - ImGui::GetCursorPosY() - style.ItemSpacing.y;
    float window_content_height = window_height - style.WindowPadding.y * 6.0f;
    float min_content_size = pw_size.x - style.WindowPadding.x * 4.0f;

    ImGui::BeginChild("#Head",ImVec2(0, list_item_height), true, ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::Text(" Current path: %s", editorEngineRef->bios->CurrentPath.c_str());
        ImGui::SameLine();

    ImGui::EndChild();

    ImGui::BeginChild("#Files", ImVec2(0, window_content_height), true);

        ImGui::PushStyleColor(ImGuiCol_Text, Col_S1_B1);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Col_P_B1);
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
        ImGui::PopStyleColor(2);
    ImGui::PushStyleColor(ImGuiCol_Text, Col_T_1);
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Col_P_B2);
    ss = Tools::GetFiles(editorEngineRef->bios->CurrentPath.c_str());
    while (std::getline(ss, temp))
    {
        if(ImGui::Selectable(temp.c_str(), false))
        {
            ClearError();
            SaveCurrentFile();
            editorEngineRef->bios->SetProgram(temp);  
            codeEditor.Deselect();
            OpenFile();
        }
    }
        
    ImGui::PopStyleColor(2);
    ImGui::EndChild();

    ImGui::BeginChild("#Foot",ImVec2(0, list_item_height), true, ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::Text(" Current program: %s", editorEngineRef->bios->CurrentProject.name.c_str());
    ImGui::EndChild();

    ImGui::BeginChild("#Foot2",ImVec2(0, list_item_height*2), true, ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::Text(" Create");
        ImGui::Text(" ");
        ImGui::SameLine();
        static char str0[128] = "new";
        if(ImGui::Button(" File "))
        {
            SaveCurrentFile();
            editorEngineRef->bios->SetProgram(str0);
            codeEditor.SetText("");
            SaveCurrentFile();
        }
        
        ImGui::SameLine();
        if(ImGui::Button(" Folder "))
        {
            std::string path = "assets/" + editorEngineRef->bios->CurrentPath + "/" + str0;
            int status = mkdir(path.c_str());

            if (status == 0) 
            {
                Tools::console->AddLog("[Info] Folder %s created successfully.", path.c_str());
            } 
            else 
            {
                Tools::console->AddLog("[Error] Failed to create %s", path.c_str());
            }
        }
        
        ImGui::SameLine();
        ImGui::PushItemWidth(200);
        ImGui::InputText("###inputName", str0, IM_ARRAYSIZE(str0));
        ImGui::PopItemWidth();
    ImGui::EndChild(); 
}

void Editor::SetError(int line, int column, std::string dsc)
{
    TextEditor::ErrorMarkers markers;
    markers.insert(std::make_pair(line, dsc));
    codeEditor.SetErrorMarkers(markers);
    codeEditor.SetCursorPosition(TextEditor::Coordinates{line, column});
}

void Editor::ClearError()
{
    TextEditor::ErrorMarkers markers;
    codeEditor.SetErrorMarkers(markers);
}



void Editor::DrawCode(bool* p_open)
{	
    ImGuiIO& io = ImGui::GetIO();
	auto shift = io.KeyShift;
	auto ctrl = io.ConfigMacOSXBehaviors ? io.KeySuper : io.KeyCtrl;
	auto alt = io.ConfigMacOSXBehaviors ? io.KeyCtrl : io.KeyAlt;

    bool stupidsenseFocus;
    ImVec2 codeHelperPos = ImVec2(0,0);

    if (ImGui::IsKeyReleased(ImGuiKey_LeftCtrl))
    {
        if (!stupidsenseFocus)
        {
            stupidsenseFocus = true;
        }
        else
        {
            ImGui::SetWindowFocus("Code Editor"); //TODO: Fix this.
            stupidsenseFocus = false;
        }
    }

    ImGui::Begin("Code Editor", p_open, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);    
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
                    if (ImGui::MenuItem("Select all", "Ctrl-A", nullptr, true))
                        codeEditor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(codeEditor.GetTotalLines(), 0));
                    ImGui::Separator();
                    if (ImGui::MenuItem("Duplicate line", "Ctrl-D", nullptr, !ro))
                        codeEditor.DuplicateLine();
                    if (ImGui::MenuItem("Move line up", "Ctrl-Up", nullptr, !ro))
                        codeEditor.MoveLine(ImGuiDir_Up);
                    if (ImGui::MenuItem("Move line down", "Ctrl-Down", nullptr, !ro))
                        codeEditor.MoveLine(ImGuiDir_Down);
                    ImGui::EndMenu();
                }
                if (docs.ProgramName != "")
                {
                    if (ImGui::BeginMenu("Load example"))
                    {
                        if (ImGui::MenuItem(docs.ProgramName.c_str(), nullptr, nullptr, true))
                        {
                            ClearError();
                            SaveCurrentFile();
                            editorEngineRef->bios->CurrentPath = "docs/";
                            editorEngineRef->bios->SetProgram(docs.ProgramName);
                            codeEditor.Deselect();
                            OpenFile();
                        }
                        ImGui::EndMenu();
                    }
                }
            ImGui::EndMenuBar();
        }

        static char str0[128] = "";
        if (ctrl && !shift && !alt && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F)))
        {
            const char* findWord = codeEditor.GetFindWord().c_str();
            strncpy(str0, findWord, sizeof(str0));
            str0[sizeof(str0) - 1] = '\0';
            codeEditor.Find(str0);
        }
        auto cpos = codeEditor.GetCursorPosition();
		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, codeEditor.GetTotalLines(),
			        codeEditor.IsOverwrite() ? "Ovr" : "Ins", codeEditor.CanUndo() ? "*" : " ",
                    editorEngineRef->bios->CurrentProject.name.c_str());
                    
        ImGui::Text("Find");
        ImGui::SameLine();
        if (ImGui::InputText("###find", str0, IM_ARRAYSIZE(str0)))
        {
            codeEditor.Find(str0);
        }
        ImGui::SameLine();

        if(ImGui::Button(ICON_FA_ARROW_UP))
        {
            codeEditor.FindPrev();
        }
        ImGui::SameLine();

        if(ImGui::Button(ICON_FA_ARROW_DOWN))
        {
            codeEditor.FindNext();
        }
        ImGui::SameLine();

        if(codeEditor.findWordsPositions.size() > 0)
            ImGui::Text("%i/%i",codeEditor.findWordIndex + 1, codeEditor.findWordsPositions.size());
        else
            ImGui::Text("?/0");
        
        ImGui::SameLine();
        if (ImGui::Checkbox("Code Helper##chcbox", &codeHelperOpen))
        {
            codeHelperPos = ImGui::GetCursorScreenPos();
            ImGui::SetWindowPos("Code Helper", codeHelperPos, ImGuiCond_Always);
        }
        ImGui::Separator();

        if (editorEngineRef->bios->CurrentProject.name != "")
        {
            codeEditor.Render("TextEditor");
        }
        else
        {
            ImGui::Text("Load or create a program in File Browser");
        }

    if (codeHelperOpen)
    {   
        ImGui::PushStyleColor(ImGuiCol_WindowBg, Col_P_D2);
        ImGui::Begin("Code Helper", &codeHelperOpen, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
            bool firstElement = true;
        	for (std::string& k : codeEditor.CodeHelperGetList())
	    	{
                if (firstElement && stupidsenseFocus)
                {
                    ImGui::SetKeyboardFocusHere();
                    firstElement = false;
                }
                if (ImGui::Selectable(k.c_str()))
                {
                    codeEditor.InsertKeyword(k);
                }
	    	}

        ImGui::End();
        ImGui::PopStyleColor();
    }

    ImGui::End();
}

void Editor::DrawDocs(bool* p_open)
{
    ImGui::Begin("Documentation", p_open, ImGuiWindowFlags_HorizontalScrollbar);
        docs.Draw(p_open);
    ImGui::End();
}

static ImVec4 selectedColor;
static ImVec4 oldColor;
static int selectedId = -1;
void Editor::DrawPalette()
{
    for (char c = 0; c<16; c++)
    {
        char buffer [5];
        sprintf (buffer, "%i", c);

        Color col = editorEngineRef->spriteManager->GetColor(c);
        ImVec4 color = ImVec4(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, 1.0f);
        ImGui::BeginGroup();

        if (ImGui::ColorButton(buffer, color, ImGuiColorEditFlags_NoTooltip, ImVec2(30, c == selectedId ? 25 : 30)))
        {
            selectedColor = ImVec4(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, 1.0f);
            oldColor = selectedColor;
            selectedId = c;
        }
        if (c == selectedId)
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 pos = ImGui::GetCursorScreenPos();
            draw_list->AddRectFilled(ImVec2(pos.x+2, pos.y-5), ImVec2(pos.x+28 , pos.y+1.5f), IM_COL32Vec4(Col_S1_B1));
        }
        ImGui::EndGroup();
        if((c+1) % 8 != 0) ImGui::SameLine();

        if (ImGui::IsItemHovered())
        {
            HighLightMemory(c*3,3);
        }    
    }
    if (selectedId < 0)
    {
        selectedId = 0;
        Color col = editorEngineRef->spriteManager->GetColor(selectedId);
        ImVec4 color = ImVec4(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, 1.0f);
        selectedColor = ImVec4(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, 1.0f);
    };
    char buffer [11];
    sprintf (buffer, "Color: %i", selectedId);
    ImGui::Text(buffer);
    ImGui::PushItemWidth(300);
    if(ImGui::ColorPicker3("", (float*)&selectedColor, ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_NoSidePreview 
        | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex
        | ImGuiColorEditFlags_NoSmallPreview))
    {
        editorEngineRef->spriteManager->SetColor(selectedId, selectedColor.x * 255, selectedColor.y * 255, selectedColor.z * 255);
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::ColorButton("Previus", oldColor, 0, ImVec2(40,40)))
    {
        selectedColor = oldColor;
        editorEngineRef->spriteManager->SetColor(selectedId, selectedColor.x * 255, selectedColor.y * 255, selectedColor.z * 255);
    }
    ImGui::BeginGroup();

        std::string name = "default";
        if (editorEngineRef->bios->CurrentProject.name != "")
                name = editorEngineRef->bios->CurrentProject.name;
    
        ImGui::Text(TextFormat("Preset %s", name.c_str()));
        ImGui::SameLine();
        if(ImGui::Button("Export"))
        {
            name += ".colPreset";
            Tools::console->AddLog("Saving Color preset");
            editorEngineRef->DumpMemory(name.c_str(), 0, 48);
        }
    ImGui::EndGroup();
}

void Editor::SpriteRect(int id, ImVec2 pos, ImVec2 size, int x, int y, ImTextureID my_tex_id) 
{
    ImVec4 bg_col = Col_P_D2;
    ImVec4 bg_col_selected = Col_S1_B1;       

    ImVec2 uv0 = ImVec2(x * 0.0625f, y * 0.0625f);
    ImVec2 uv1 = ImVec2(x * 0.0625f + 0.0625f, y * 0.0625f + 0.0625f);
    ImVec4 col = currentSprite == id ? bg_col_selected : bg_col;

    ImGui::SetCursorScreenPos(pos);

    ImGui::PushStyleColor(ImGuiCol_Button, Col_P_B2);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Col_S2_B1);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
    if (ImGui::ImageButton("###", my_tex_id, size, uv0, uv1, col, Col_T_1))
    {
        HighLightMemory(id * 8 + 48, 8);
        currentSprite = id;
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);

}

static int metaSpriteRectSelected = -1;
void Editor::MetaSpriteRect(int id, ImVec2 pos, ImVec2 size, int x, int y, ImTextureID my_tex_id) 
{

    ImVec4 col = Col_S2_D2;
  
    ImVec2 uv0 = ImVec2(x * 0.0625f, y * 0.0625f);
    ImVec2 uv1 = ImVec2(x * 0.0625f + 0.0625f, y * 0.0625f + 0.0625f);

    ImGui::SetCursorScreenPos(pos);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 1.0f));

    if (ImGui::ImageButton("###", my_tex_id, size, uv0, uv1, col, Col_T_1))
    {
        metaSpriteRectSelected = id;
    }
    ImGui::PopStyleVar();
}

void Editor::DrawSprites()
{
    ImGuiIO& io = ImGui::GetIO();
    ImTextureID my_tex_id = &editorEngineRef->spriteManager->spriteTexture.id;
    ImVec2 size = ImVec2(32.0f * io.FontGlobalScale, 32.0f * io.FontGlobalScale);
    
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 p = pos;
    
    float my_tex_w = 128;
    float my_tex_h = 128;
    float padding = 6;
    ImGui::BeginGroup();
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            int id = x + y * 16;
            ImGui::PushID(id);
            SpriteRect(id, pos, size, x, y, my_tex_id);
            pos.x += size.x + padding; 
            ImGui::PopID();
        }
        pos.x = p.x;
        pos.y += size.y + padding; 
    }
    ImGui::EndGroup();
    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::Text("ID: %03i", currentSprite);
    if(ImGui::Button("Copy",ImVec2(size.x*2.5,size.y)))
    { 
        for (int c = 0; c < 8; c++)
            spriteCopy[c] = editorEngineRef->Peek(currentSprite * 8 + 48 + c);
    }
    if(ImGui::Button("Paste",ImVec2(size.x*2.5,size.y)))
    {
        for (int c = 0; c < 8; c++)
             editorEngineRef->Poke(currentSprite * 8 + 48 + c, spriteCopy[c]);
    }
    ImGui::EndGroup();
    
}

void Editor::DrawCRT()
{
    bool ppState = editorEngineRef->Peek(4091);
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

    ImGui::BeginGroup();
    ImGui::Text("Grille Mode  ");
    ImGui::RadioButton("Dots", &grille, 0);
    ImGui::SameLine();
    ImGui::RadioButton("TV", &grille, 1); 
    ImGui::SameLine();
    ImGui::RadioButton("LCD", &grille, 2);
    ImGui::SameLine(); 
    ImGui::Text("         ");
    ImGui::SameLine(); 

    ImGui::BeginGroup();
    std::string name = "default";
    if (editorEngineRef->bios->CurrentProject.name != "")
            name = editorEngineRef->bios->CurrentProject.name;
    
    ImGui::Text(TextFormat("Preset %s", name.c_str()));
    if (ImGui::Button("Export"))
    {
        name += ".crtPreset";
        Tools::console->AddLog("Saving Crt preset");
        editorEngineRef->DumpMemory(name.c_str(), 4080, 11);
    }
    ImGui::EndGroup();

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

void Editor::DrawPlayer()
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 playerSize = ImGui::GetWindowSize();
    ImVec2 buttonSize = ImVec2(32.0f * io.FontGlobalScale, 32.0f * io.FontGlobalScale);
   
    float totalButtonWidth = buttonSize.x * 4;

    float buttonPosX = (playerSize.x - totalButtonWidth) / 2;
    float buttonPosY = (playerSize.y - buttonSize.y) / 2;

    ImGui::Text(TextFormat("Frame: %07i", editorEngineRef->basicIntepreter->GetCurrentFrame()));
    ImGui::SameLine();
    ImGui::SetCursorPosX(buttonPosX);

    ImGui::BeginDisabled(PlayerState != Off || editorEngineRef->bios->CurrentProject.name == "");
    //PLAY
    if(ImGui::Button(ICON_FA_PLAY, buttonSize))
    {
        if (!editorEngineRef->FileWatcherEnabled) SaveCurrentFile();
        editorEngineRef->bios->ShouldRun = true;
        PlayerState = Running;
        DoStep = false;
    }
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!PlayerState == Running);
    ImGui::SameLine();
    //STOP
    if(ImGui::Button(ICON_FA_STOP, buttonSize))
    {
        PlayerState = Off;
        DoStep = false;
        editorEngineRef->basicIntepreter->close();
        editorEngineRef->currentState = Off;
        editorEngineRef->basicIntepreter->CloseBas();
        editorEngineRef->audioManager->StopAll();
    }

    //PAUSE
    ImGui::SameLine();
    bool popColor = false;
    if (PlayerState == Paused)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(194,40,40,255));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(210,80,80,255));
        popColor = true;
    }
    if(ImGui::Button(ICON_FA_PAUSE, buttonSize))
    {
        if (PlayerState == Paused) PlayerState = Running;
        else if (PlayerState == Running) PlayerState = Paused;
    }
    if (popColor)
    {
        ImGui::PopStyleColor(2);
    }

    ImGui::BeginDisabled(PlayerState != Paused);
    ImGui::SameLine();
    //STEP
    if(ImGui::Button(ICON_FA_FORWARD_STEP, buttonSize))
    {
        DoStep = true;
    }
    ImGui::EndDisabled();
    ImGui::EndDisabled();
    
}

void Editor::DrawMemory()
{
    /*
    static char str0[128] = "memory";
    if (ImGui::Button("Load default"))
    {
        editorEngineRef->LoadDefaultMemory();
    }
    ImGui::SameLine();
    ImGui::InputText("New", str0, IM_ARRAYSIZE(str0));
    ImGui::SameLine();
    if (ImGui::Button("Save"))
    {
        std::stringstream ss;
        ss << ASSETS_FOLDER << "/" << editorEngineRef->bios->CurrentPath << "/" << str0 << MEM_EXTENSION;
        Tools::console->AddLog("[MEMORY] Saved:");
        Tools::console->AddLog(ss.str().c_str());
        editorEngineRef->DumpMemory(ss.str().c_str());
    }
    */
    mem_edit.DrawContents(editorEngineRef->GetMemory(), 4096);
}


void Editor::PianoKey(ImVec2 size, int note, bool isBlack) 
{
    ImVec4 color = Col_S2_B1;
    ImVec4 colorHover = Col_S2_B2;
    ImVec4 colorActive = Col_P_B1;
    char c;

    for (const auto& kv : keyCharToKey)
        if (kv.second == note) { c = kv.first; break; }
        
    const char* formatText = "\n\n\n%c";

    if (isBlack)
    {
        color = Col_S1_D2;
        colorHover = Col_S1_D1;
        formatText = "\n%c";
    }

    ImGui::SetItemAllowOverlap();

    char button_label[32];
    sprintf(button_label, formatText , c);

    int finalNote = note + keyboardOctave * 12;

    if (finalNote == pressedKey)
    {
        color = colorActive;
    }

    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorHover);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorActive);

    ImGui::Button(button_label, size);
        
    ImGui::PopStyleColor(3);
    ImGui::SameLine();

    if (ImGui::IsItemHovered() && IsMouseButtonDown(0) && currentNote == -1)
    {
        currentNote = finalNote;
        editorEngineRef->audioManager->PlayNote(3, currentNote, pianoVolume);
    }
    if (IsMouseButtonReleased(0) && currentNote != -1)
    {
        currentNote = -1;
        editorEngineRef->audioManager->StopNote(3);
    }
}

bool Editor::IsBlack(int note)
{
    int n = note % 12;
    return (n == 1 || n == 3 || n == 6 || n == 8 || n == 10);
}

void Editor::DrawChannel(uint8_t channel, ImVec2 pos)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float step = 2.0f;
    ImVec2 init = pos, end;

    draw_list->AddRectFilled(pos, ImVec2((pos.x + (63 * step)), pos.y + 128), 
                                IM_COL32Vec4(Col_P_D2), 12.0f);
    float s = 8*step;
    for (int c = 1; c < 8; c++)
    {
        draw_list->AddLine(ImVec2(pos.x, pos.y + c*s), ImVec2(pos.x + 63 * step, pos.y + c * s), IM_COL32Vec4(Col_P_D1));
        draw_list->AddLine(ImVec2(pos.x + c*s, pos.y), ImVec2(pos.x + c * s, pos.y + 128), IM_COL32Vec4(Col_P_D1));
    }

    pos.x += step;
    for (int i = 0; i < 63; i++)
    {
        float h = editorEngineRef->audioManager->GetSynth()->GetFrameAverage(channel ,i)/2;
        if (i==0) init = ImVec2(pos.x, pos.y + h);
        
        if (i>0)
        {
            draw_list->AddLine(init, end, IM_COL32Vec4(Col_S1_B1), 2.0f);
            init = end;
        }

        end = ImVec2((pos.x + (i * step)), pos.y + h);
    }
}

void Editor::DrawToolsPiano()
{  
    float keyWidth = ImGui::CalcTextSize("    ").x;
    float keyHeight = ImGui::CalcTextSize(" ").y;
    ImVec2 top_pos = ImGui::GetCursorPos();
    ImGui::Text("Octave %i  |  ", keyboardOctave);
    ImGui::SameLine();

    if (currentNote >= 0)
        ImGui::Text("Note %03i - %s", currentNote, editorEngineRef->audioManager->GetNoteName(3));
    else if (pressedKey >= 0)
        ImGui::Text("Note %03i - %s", pressedKey, editorEngineRef->audioManager->GetNoteName(3));
    else
        ImGui::Text("Note --- - ---");

    ImGui::SameLine();
    ImGui::Text("  ");
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_ANGLES_LEFT) && keyboardOctave > 0) 
        keyboardOctave--;
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_ANGLES_RIGHT) && keyboardOctave < 8) 
        keyboardOctave++;
    
    ImVec2 white_key_pos = ImGui::GetCursorScreenPos();
    ImVec2 black_key_pos = ImVec2(white_key_pos.x + keyWidth / 2.f ,white_key_pos.y);
    
    ImGui::BeginGroup();
        for (int i = 0; i <= 12; i++) 
        {
            if (!IsBlack(i))
            {
                PianoKey(ImVec2(keyWidth, keyHeight * 5), i, false);
                white_key_pos.x += keyWidth;
            } 
        }
    ImGui::EndGroup();

    ImGui::SetCursorScreenPos(black_key_pos);       
    
    ImGui::BeginGroup();
        for (int i = 0; i <= 12; i++) 
        {
            if (i == 5) 
            {
                ImGui::Text("    ");
                ImGui::SameLine();
            }
            if (IsBlack(i))
            {
                PianoKey(ImVec2(keyWidth, keyHeight * 3), i, true);
            }
        }
    ImGui::EndGroup();
    ImGui::BeginGroup(); 
    ImGui::SetCursorPos(ImVec2(keyWidth * 10, top_pos.y));  
    ImGuiKnobs::KnobInt("Vol", &pianoVolume, 0, 127, 1.0f, "%03i", ImGuiKnobVariant_Stepped);
    ImGui::EndGroup();

    ImGui::Text("Oscilloscope");
    ImVec2 pos = ImGui::GetCursorScreenPos();
    
    DrawChannel(0, ImVec2(pos.x, pos.y));
    DrawChannel(1, ImVec2(pos.x + 140, pos.y));
    DrawChannel(2, ImVec2(pos.x + 280, pos.y));
    DrawChannel(3, ImVec2(pos.x + 420, pos.y));
            
    for (auto& key : keyCharToKey)
    {
        char thisChar = key.first;
        int thisKey = key.second + keyboardOctave * 12;

        if (IsKeyPressed(thisChar)) 
        {
            editorEngineRef->audioManager->PlayNote(3, thisKey, pianoVolume);
            pressedKey = thisKey;
        }

        if (IsKeyReleased(thisChar)) 
        {
            editorEngineRef->audioManager->StopNote(3);
            pressedKey = -1;
        }
    }
    if (IsKeyPressed('.') && keyboardOctave > 0) 
        keyboardOctave--;
    
    if (IsKeyPressed('/') && keyboardOctave < 8) 
        keyboardOctave++;
}

void Editor::DrawToolsSequence()
{

    static char ex_str0[5][4096] = {
        " ",
        "@0t220L4O4V70t220d4e4f+4g4a4a+4b8b8b8&b32r16r32b8&b32r16r32b2r8g8>e2.d+2.e2.r8<g8a8b8>c8d8e2.d+2f4e2.r2r8<g8>d2.c+2.d2.r8<g8a8b8>c8c+8d2.<g2>f4e2.r2r8<g8>g2.g2.g2.g4a4r8g8f2.f2.f2.f4g4r8f8e2.<a4b4>f4e8e8e4.<b8>c2.",
        "@0t150L4V30>e4<b8>c8d8e16d16c8<b8a4a8>c8e4d8c8<b4.>c8d4e4c4<a4a2&a8>d4f8a4g8f8e4.c8e4d8c8<b4b8>c8d4e4c4<a4a2>e4<b8>c8d8e16d16c8<b8a4a8>c8e4d8c8<b4.>c8d4e4c4<a4a2&a8>d4f8a4g8f8e4.c8e4d8c8<b4b8>c8d4e4c4<a4a2e2c2d2<b2>c2<a2g+2b2>e2c2d2<b2>c4e4a2g+1>e4<b8>c8d8e16d16c8<b8a4a8>c8e4d8c8<b4.>c8d4e4c4<a4a2&a8>d4f8a4g8f8e4.c8e4d8c8<b4b8>c8d4e4c4<a4a2>e4<b8>c8d8e16d16c8<b8a4a8>c8e4d8c8<b4.>c8d4e4c4<a4a2&a8>d4f8a4g8f8e4.c8e4d8c8<b4b8>c8d4e4c4<a4a4",
        "@0t126L16v30a+f>f<f>d+<f>c+<f>c<f>c+<f>c<fa+f>c<f>c+<f>d+<f>c+<f>c<fg+f>c<fa+fa+f>f<f>d+<f>c+<f>c<f>c+<f>c<fa+f>c<f>c+<f>d+<f>c+<f>c<fg+f>c<fa+f>d+8g+f2&fd+8c+8d+8.g+8.f4.d+8c+8d+8g+f2&fd+4f+8.g+4&g+f8.f+4&f+d+8g+f2&fd+8c+8d+8.g+8.f4.d+8c+8d+8g+f2&fd+8f8f+8.g+4&g+f8g8a8>c8<c8.<a+8.>a+8c8.<a+8.>a+8c8.<a+8.>a+8c+>c+<c>c<<a+>a+<g+>g+c8<a+>a+4&a+c8<a+>a+4&a+c8<a+>a+4&a+>c+8d+8cc+8.<<",
        "@1t90l16v30ga+>dd+<ga+>dd+<ga+>dd+<ga+>dd+<f+a>dd+<f+a>dd+<f+a>dd+<f+a>dd+<fg+>dd+<fg+>dd+<fg+>dd+<fg+>dd+<eg>dd+<eg>dd+<eg>dd+<eg>dd+<d+g>cd<d+g>cd<d+g>cd<d+g>cd<dg>cd<dg>cd<dg>cd<dg>cd<cf+ab+f+a>cd+<a>cd+cd+f+d+f+af+ab+g4"
    };
    static char ex_str1[5][4096] = {
        " ",
        "@1L4O4V70t220d4c+4c4<b4>c4c+4d8d8d4e4f2r4g2.f+2.g2.r2r4g2.f+2a4g2.r2r4f2.e2.f2.r2r4f2.<b2>a4g2.r2r4>e2.d2.c+2.r2r4d2.c+2.c2.r2r4<c2.f4g4b4b8b8b4r8f8e2.",
        "@1V30b4g+8a8b4a8g+8e4e8a8>c4<b8a8g+8e8g+8a8b4>c4<a4e4e2&e8f4a8>c8c16c16<b8a8g4.e8g8a16g16f8e8g+8e8g+8a8b8g+8>c8<g+8a8e8e4e2b4g+8a8b4a8g+8e4e8a8>c4<b8a8g+8e8g+8a8b4>c4<a4e4e2&e8f4a8>c8c16c16<b8a8g4.e8g8a16g16f8e8g+8e8g+8a8b8g+8>c8<g+8a8e8e4e2c2<a2b2g+2a2e2e2g+2>c2<a2b2g+2a4>c4e2d1b4g+8a8b4a8g+8e4e8a8>c4<b8a8g+8e8g+8a8b4>c4<a4e4e2&e8f4a8>c8c16c16<b8a8g4.e8g8a16g16f8e8g+8e8g+8a8b8g+8>c8<g+8a8e8e4e2b4g+8a8b4a8g+8e4e8a8>c4<b8a8g+8e8g+8a8b4>c4<a4e4e2&e8f4a8>c8c16c16<b8a8g4.e8g8a16g16f8e8g+8e8g+8a8b8g+8>c8<g+8a8e8e4e4",
        "@1t126L16v30c+<a+>g+<a+>f+<a+>f<a+>d+<a+>f<a+>d+<a+>c+<a+>d+<a+>f<a+>f+<a+>f<a+>d+<a+>c<a+>d+<a+>c+<a+>c+<a+>g+<a+>f+<a+>f<a+>d+<a+>f<a+>d+<a+>c+<a+>d+<a+>f<a+>f+<a+>f<a+>d+<a+>c<a+>d+<a+>c+<a+4.>c<a+>cc+8d+8f2&fc<a+>cc+8d+8f2&fd+c+d+f8f+8g+8a+8a2f8.d+2&d+8c<a+>cc+8d+8f2&fc<a+>cc+8d+8f2&fd+c+d+f8f+8g+8a+8a2f8.d+4&d+g+8.f4&fg+8.f4&fg+8.f4&ff>f<d+>d+<c+>c+<c>c<g+8f>f4&f<g+8f>f4&f<g+8f>f4&ff8f+8d+f8.",
        "@3t90l16v90o5<g2a+4>d4c+4<f+2.f2&fg+4>c+8c4<e2.l16d+dd+4.g8.>d+8.d8<dc+d4.g8.>d8.c+8<df+af+ab+a>cd+cd+f+af+d+cd+c<af+g4;"
    };
    static char ex_str2[5][4096] = {
        " ",
        "@2L4O4V70t220r1r1<g4g2.c4g4>c4<<b4>g4b4c4g4>c4<e4g4>c4<c4g4>c4<<b4>g4b4c4g4>c4<e4g4>c4<d4g4b4c+4f+4a+4d4g4b4<b4>g4b4d4g4b4<b4>g4b4c4g4>c4<<g4>g4>c4<c4g4>e4<<b4>g4>d4<<a+4>g4>c+4<c+4g4>e4<d4a4>f4<c+4a4>f4<c4a4>f4<<b4>g4>f4<<c4>g4>e4<<g4>g4g4f8f8f4r8<b8>c2.",
        "@2V30o3e8>e8<e8>e8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<a8>a8<g+8>g+8<g+8>g+8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<b8>c8d8<d4d4d8a8f8c8>c4c8<c8g8g4b8>b4b4e4g+8<a8>e8<a8>e8<a2e8>e8<e8>e8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<a8>a8<g+8>g+8<g+8>g+8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<b8>c8d8<d4d4d8a8f8c8>c4c8<c8g8g4b8>b4b4e4g+8<a8>e8<a8>e8<a2>a8>e8<a8>e8<a8>e8<a8>e8<g+8>e8<g+8>e8<g+8>e8<g+8>e8<a8>e8<a8>e8<a8>e8<a8>e8<g+8>e8<g+8>e2&e8<a8>e8<a8>e8<a8>e8<a8>e8<g+8>e8<g+8>e8<g+8>e8<g+8>e8<a8>e8<a8>e8<a8>e8<a8>e8<g+8>e8<g+8>e2&e8<<e8>e8<e8>e8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<a8>a8<g+8>g+8<g+8>g+8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<b8>c8d8<d4d4d8a8f8c8>c4c8<c8g8g4b8>b4b4e4g+8<a8>e8<a8>e8<a2e8>e8<e8>e8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<a8>a8<g+8>g+8<g+8>g+8<e8>e8<e8>e8<a8>a8<a8>a8<a8>a8<b8>c8d8<d4d4d8a8f8c8>c4c8<c8g8g4b8>b4b4e4g+8<a8>e8<a8>e8<a4",
        "@2t126L16v60o2a+1>f1a+1>f1<<a+1g+1f+1f1a+1g+1f+1f2f>f<g>g<a>ac>c<<a+2g+2f+2g+2a+2g+2f+2g+2",
        " "
    };
    static char ex_str3[5][4096] = {
        " ",
        "@3L4O4V70t220r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4r4g8r8g4r4g8g8g4",
        " ",
        " ",
        " "
    };

    static char str0[4096] = "";
    static char str1[4096] = "";
    static char str2[4096] = "";
    static char str3[4096] = "";

    if (ImGui::Button("Play")) 
    {
        editorEngineRef->audioManager->ChannelPlay(0, str0);
        editorEngineRef->audioManager->ChannelPlay(1, str1);
        editorEngineRef->audioManager->ChannelPlay(2, str2);
        editorEngineRef->audioManager->ChannelPlay(3, str3);
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop")) 
    {
        editorEngineRef->audioManager->ChannelStop(0);
        editorEngineRef->audioManager->ChannelStop(1);
        editorEngineRef->audioManager->ChannelStop(2);
        editorEngineRef->audioManager->ChannelStop(3);
    }
    ImGui::SameLine();

    ImGui::PushItemWidth(300);
    const char* items[] = {  "Clear", "Mario-Underwater (Vycaon)", "Tetris (Debbi)", "Bloody Tears (Agor)", "Zelda Dungeon (Caymen)" };
    static int item_current_idx =  0;
    const char* combo_preview_value = items[item_current_idx];

    if (ImGui::BeginCombo("Songs Examples", combo_preview_value, ImGuiComboFlags_NoArrowButton))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(items[n], is_selected))
            {
                item_current_idx = n;
                strcpy(str0, ex_str0[n]);
                strcpy(str1, ex_str1[n]);
                strcpy(str2, ex_str2[n]);
                strcpy(str3, ex_str3[n]);
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    ImGui::BeginGroup();
    ImGui::Text(TextFormat("Channel 0 %04i/%04i - %s", 
        editorEngineRef->audioManager->GetChannelPosition(0), 
        editorEngineRef->audioManager->GetChannelSize(0), 
        editorEngineRef->audioManager->GetNoteName(0)));
    ImGui::InputTextMultiline("##channel_00", str0, IM_ARRAYSIZE(str0), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 5));
    
    ImGui::Text(TextFormat("Channel 1 %04i/%04i - %s", 
        editorEngineRef->audioManager->GetChannelPosition(1), 
        editorEngineRef->audioManager->GetChannelSize(1), 
        editorEngineRef->audioManager->GetNoteName(1)));
    ImGui::InputTextMultiline("##channel_01", str1, IM_ARRAYSIZE(str1), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 5));
    
    ImGui::Text(TextFormat("Channel 2 %04i/%04i - %s", 
        editorEngineRef->audioManager->GetChannelPosition(2), 
        editorEngineRef->audioManager->GetChannelSize(2), 
        editorEngineRef->audioManager->GetNoteName(2)));
    ImGui::InputTextMultiline("##channel_02", str2, IM_ARRAYSIZE(str2), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 5));
    
    ImGui::Text(TextFormat("Channel 3 %04i/%04i - %s", 
        editorEngineRef->audioManager->GetChannelPosition(3), 
        editorEngineRef->audioManager->GetChannelSize(3), 
        editorEngineRef->audioManager->GetNoteName(3)));
    ImGui::InputTextMultiline("##channel_03", str3, IM_ARRAYSIZE(str3), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 5));
    ImGui::EndGroup();
    
}

void Editor::DrawSFX()
{
    static int id = 0;
    unsigned int dir = 3376 + (id * 11);

    int osc = editorEngineRef->Peek(dir);

    int attack   = editorEngineRef->Peek(dir+1);
    int decay    = editorEngineRef->Peek(dir+2);
    int sustain  = editorEngineRef->Peek(dir+3);
    int release  = editorEngineRef->Peek(dir+4);

    int lfoSpeed = editorEngineRef->Peek(dir+5);
    int lfoDepth = editorEngineRef->Peek(dir+6);

    int cut = editorEngineRef->Peek(dir+7);
    int res = editorEngineRef->Peek(dir+8);

    int slope = editorEngineRef->Peek(dir+9);
    int curve = editorEngineRef->Peek(dir+10);
       
    if (ImGui::IsWindowFocused())
    {
        HighLightMemory(dir,11);
    }


    ImGui::BeginGroup();
        
        ImGui::Text("Osc");
        ImGui::RadioButton("Sqr", &osc, 0);
        ImGui::SameLine();
        ImGui::RadioButton("P12", &osc, 1); 
        ImGui::SameLine();
        ImGui::RadioButton("P25", &osc, 2);
        ImGui::RadioButton("Tri", &osc, 3);
        ImGui::SameLine();
        ImGui::RadioButton("Saw", &osc, 4);
        ImGui::SameLine();
        ImGui::RadioButton("Noi", &osc, 5);
        ImGui::Text("____________________");
        ImGui::Text("PRESET %i",id);

        editorEngineRef->audioManager->SetOSC(id, osc);
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::Text("  ");
    ImGui::SameLine();

    if(ImGuiKnobs::KnobInt("Bank", &id, 0, 15, 0.1f, "%01i", ImGuiKnobVariant_Stepped))
    {
        editorEngineRef->audioManager->SetChannelPreset(3,id);
    }

    ImGui::SameLine();
    ImGui::Text("  | ");
    ImGui::SameLine();

    ImGui::BeginGroup();
        ImGui::Text("Envelope");
        if (ImGuiKnobs::KnobInt("att", &attack, 0, 255, 1, "%03i", ImGuiKnobVariant_WiperDot)) 
        {
            editorEngineRef->audioManager->SetEnv(id, attack, decay, sustain, release); 
        }
        ImGui::SameLine();
        if (ImGuiKnobs::KnobInt("dec", &decay, 0, 255, 1, "%03i", ImGuiKnobVariant_WiperDot)) 
        {
            editorEngineRef->audioManager->SetEnv(id, attack, decay, sustain, release); 
        }
        ImGui::SameLine();
        if (ImGuiKnobs::KnobInt("sus", &sustain, 0, 255, 1, "%03i", ImGuiKnobVariant_WiperDot)) 
        {
            editorEngineRef->audioManager->SetEnv(id, attack, decay, sustain, release); 
        }
        ImGui::SameLine();
        if (ImGuiKnobs::KnobInt("rel", &release, 0, 255, 1, "%03i", ImGuiKnobVariant_WiperDot)) 
        {
            editorEngineRef->audioManager->SetEnv(id, attack, decay, sustain, release); 
        }
    ImGui::EndGroup();

    ImGui::BeginGroup();
        ImGui::Text("LFO");
        if (ImGuiKnobs::KnobInt("speed", &lfoSpeed, 0, 255, 1.f, "%03i", ImGuiKnobVariant_Stepped)) 
        {
            editorEngineRef->audioManager->SetLFO(id, lfoSpeed, lfoDepth);
        }  
        ImGui::SameLine();
        if (ImGuiKnobs::KnobInt("depth", &lfoDepth, 0, 255, 1.f, "%03i", ImGuiKnobVariant_Stepped)) 
        {
            editorEngineRef->audioManager->SetLFO(id, lfoSpeed, lfoDepth);
        }       
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::Text(" | ");
    ImGui::SameLine();

    ImGui::BeginGroup();
        ImGui::Text("LPF");
        if (ImGuiKnobs::KnobInt("cut", &cut, 0, 255, 1.f, "%03i", ImGuiKnobVariant_Stepped)) 
        {
            editorEngineRef->audioManager->SetFilter(id, cut, res);
        }  
        ImGui::SameLine();
        if (ImGuiKnobs::KnobInt("res", &res, 0, 255, 1.f, "%03i", ImGuiKnobVariant_Stepped)) 
        {
            editorEngineRef->audioManager->SetFilter(id, cut, res);
        }  
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::Text(" | ");
    ImGui::SameLine();

    ImGui::BeginGroup();
        ImGui::Text("Slide");
        if (ImGuiKnobs::KnobInt("slope", &slope, 0, 255, 1.f, "%03i", ImGuiKnobVariant_Tick)) 
        {
            editorEngineRef->audioManager->SetSlide(id, slope, curve);
        }  
        ImGui::SameLine();
        if (ImGuiKnobs::KnobInt("curve", &curve, 0, 255, 1.f, "%03i", ImGuiKnobVariant_Tick)) 
        {
            editorEngineRef->audioManager->SetSlide(id, slope, curve);
        } 
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::Text(" | ");
    ImGui::SameLine();

    ImGui::BeginGroup();
        ImGui::Text(" ");
        ImGui::Text(" ");

        std::string name = "default";
        if (editorEngineRef->bios->CurrentProject.name != "")
                name = editorEngineRef->bios->CurrentProject.name;
    
        ImGui::Text(TextFormat("Preset\n%s", name.c_str()));

        if(ImGui::Button("Export", ImVec2(80,70)))
        {
            name += ".sfxPreset";
            Tools::console->AddLog("Saving Sfx preset");
            editorEngineRef->DumpMemory(name.c_str(), 3376, 176);
        }
    ImGui::EndGroup();
}

inline unsigned char setBit(unsigned char byte, int position, bool newState) {
    unsigned char mask = 1 << position;
    
    if (newState) {
        byte |= mask; // set bit to 1
    } else {
        byte &= ~mask; // set bit to 0
    }
    
    return byte;
}

static int isMakeSpriteHovered;
void Editor::PixelRect(int dir, uint8_t bit, ImVec2 pos, ImVec2 size, bool state) 
{
    bool isMouseOverRect;
    ImVec2 posRect = ImVec2(pos.x + size.x, pos.y + size.y);
    if (ImGui::IsMouseHoveringRect(pos, posRect) && isMakeSpriteHovered) 
    {
        HighLightMemory(dir, 1);
        isMouseOverRect = true;
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) 
            state = true;
        if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) 
            state = false;

        editorEngineRef->Poke(dir,setBit(editorEngineRef->Peek(dir), bit, state));

    }
    else {
        isMouseOverRect = false;
    }
    ImU32 color = state ? IM_COL32Vec4(Col_T_1) : IM_COL32Vec4(Col_P_D2);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(pos, posRect, isMouseOverRect ?  IM_COL32Vec4(Col_S1_B1) : color);
}

void Editor::MakeSprite(int spriteId)
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec4 bg_col = Col_S2_D2;             
    ImVec4 tint_col = Col_T_1;
    ImVec2 size = ImVec2(32.0f * io.FontGlobalScale,32.0f * io.FontGlobalScale);
    uint8_t byte = 0;

    ImGui::Text("Sprite ID: %03i Address:%.3X", currentSprite, currentSprite * 8 + 48);

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 p = pos;

    static unsigned char copyByte;

    ImGui::BeginGroup();
    isMakeSpriteHovered = ImGui::IsWindowHovered();
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int id = x + y * 8;
            byte = editorEngineRef->Peek((currentSprite * 8 + 48) + y);

            ImGui::PushID(TextFormat("pixel_rect_###%i",id));

            unsigned char mascara = 1 << 7-x;

            PixelRect((currentSprite * 8 + 48) + y, 7-x, pos, size, (byte & mascara) != 0);
            pos.x += size.x + 1; 
            
            ImGui::PopID();
        }
        ImGui::SetCursorScreenPos(ImVec2(pos.x + size.x, pos.y + size.y - (size.y)));
        //ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::Text("%.2X", byte);
        ImGui::SameLine();
        ImGui::PushID(TextFormat("pixel_rect_copy###%i",y));
        if(ImGui::Button(TextFormat("%s", ICON_FA_COPY)))
        {
            copyByte = byte;
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Copy");
        }
        ImGui::PopID();
        ImGui::SameLine();
        ImGui::PushID(TextFormat("pixel_rect_paste###%i",y));
        if(ImGui::Button(TextFormat("%s", ICON_FA_PASTE)))
        {
            editorEngineRef->Poke((currentSprite * 8 + 48) + y, copyByte);
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Paste");
        }
        ImGui::PopID();
        ImGui::EndGroup();
        pos.x = p.x;
        pos.y += size.y + 1; 
    }
    ImGui::EndGroup();
    ImGui::Text("");
    if(ImGui::Button(ICON_FA_ARROW_UP))
    {
        uint8_t byte0 = editorEngineRef->Peek((currentSprite * 8 + 48));
        for (int b = 0; b < 7; b++)
        {
            uint8_t byteN = editorEngineRef->Peek((currentSprite * 8 + 48) + b + 1);
            editorEngineRef->Poke((currentSprite * 8 + 48) + b, byteN);
        }
        editorEngineRef->Poke((currentSprite * 8 + 48) + 7, byte0);
    }
    ImGui::SameLine();
    if(ImGui::Button(ICON_FA_ARROW_LEFT))
    {
        for (int b = 0; b < 8; b++)
        {
            uint8_t byteN = editorEngineRef->Peek((currentSprite * 8 + 48) + b);
            editorEngineRef->Poke((currentSprite * 8 + 48) + b, (byteN << 1) | (byteN >> 7));
        }
    }
    ImGui::SameLine();
    if(ImGui::Button(ICON_FA_ARROW_RIGHT))
    {
        for (int b = 0; b < 8; b++)
        {
            uint8_t byteN = editorEngineRef->Peek((currentSprite * 8 + 48) + b);
            editorEngineRef->Poke((currentSprite * 8 + 48) + b, (byteN >> 1) | (byteN << 7));
        }
    }
    ImGui::SameLine();
    if(ImGui::Button(ICON_FA_ARROW_DOWN))
    {
        uint8_t byte7 = editorEngineRef->Peek((currentSprite * 8 + 48) + 7);
        for (int b = 7; b > 0; b--)
        {
            uint8_t byteN = editorEngineRef->Peek((currentSprite * 8 + 48) + b - 1);
            editorEngineRef->Poke((currentSprite * 8 + 48) + b, byteN);
        }
        editorEngineRef->Poke((currentSprite * 8 + 48), byte7);
    }
    ImGui::SameLine();
    if(ImGui::Button(ICON_FA_MASK))
    {
        for (int b = 0; b < 8; b++)
        {
            uint8_t byteN = editorEngineRef->Peek((currentSprite * 8 + 48) + b);
            editorEngineRef->Poke((currentSprite * 8 + 48) + b, ~byteN);
        }
    }

    ImGui::BeginGroup();
    ImGui::EndGroup();
}

int Editor::GetColorPopup()
{
    for (char c = 0; c<16; c++)
    {
        char buffer [5];
        sprintf (buffer, "%i", c);

        Color col = editorEngineRef->spriteManager->GetColor(c);
        ImVec4 color = ImVec4(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, 1.0f);
        ImGui::BeginGroup();
        ImGui::ColorEdit3(buffer, (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoBorder);
        ImGui::Text(buffer);
        ImGui::EndGroup();
        if((c+1) % 4 != 0) ImGui::SameLine();

        if (ImGui::IsItemClicked())
        {
            return c;
        }
    }
    return -1;
}

void Editor::GetSpritePopup()
{
    ImGuiIO& io = ImGui::GetIO();
    ImTextureID my_tex_id = &editorEngineRef->spriteManager->spriteTexture.id;
    ImVec2 size = ImVec2(16.0f * io.FontGlobalScale, 16.0f * io.FontGlobalScale);
    
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 p = pos;
    
    float my_tex_w = 128;
    float my_tex_h = 128;
    float padding = 5;
    static int value = -1;
    ImGui::BeginGroup();
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            int id = x + y * 16;
            ImGui::PushID(id);
            MetaSpriteRect(id, pos, size, x, y, my_tex_id);
            pos.x += size.x + padding; 
            ImGui::PopID();
        }
        pos.x = p.x;
        pos.y += size.y + padding; 
    }
    ImGui::EndGroup();
}

void Editor::DrawMetaLine(int id)
{
    int dir = (currentMetaSprite * 20 + 2096);
    dir += id * 5;

    char idName[16];
    sprintf(idName, "%i" , dir);

    int spID = editorEngineRef->Peek(dir);
    int col = editorEngineRef->Peek(dir + 3);

    const char* spIdName = TextFormat("pop_spID_%04i", dir);
    const char* colIdName = TextFormat("pop_col_%04i", dir + 3);
    const char* posIdName = TextFormat("pos_%04i", dir + 4);

    ImGui::BeginGroup();
        int flag = editorEngineRef->Peek(dir + 4);
        
        if (!((flag >> 7) & 1)) //If not disabled
        {
            if(ImGui::Button(TextFormat("%03i###%03i", spID, dir)))
            {
                ImGui::OpenPopup(spIdName); 
            }

            if (ImGui::BeginPopup(spIdName))
            {
                if (metaSpriteRectSelected == -1)
                {
                    GetSpritePopup();
                }
                else
                {
                    editorEngineRef->Poke(dir, metaSpriteRectSelected);
                    metaSpriteRectSelected = -1;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
            ImGui::SameLine();

            Color bcol = editorEngineRef->spriteManager->GetColor(editorEngineRef->Peek(dir + 3));
            float luminance = 0.299f * bcol.r  + 0.587f * bcol.g  + 0.114f * bcol.b;
            float vCol = luminance < 128 ? 0.9f : 0.1f;

            ImVec4 textColor = ImVec4(vCol, vCol, vCol, 1.0f);
            ImVec4 color = ImVec4(bcol.r / 255.0f, bcol.g / 255.0f, bcol.b / 255.0f, 1.0f);
            ImGui::PushStyleColor(ImGuiCol_Button, color);
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3.0f);
            if(ImGui::Button(TextFormat("%02i###%04i", col, dir + 3)))
            {
                ImGui::OpenPopup(colIdName);
            }
            ImGui::PopStyleVar();
            ImGui::PopStyleColor(2);
            ImGui::SameLine();

            static int selected_color = -1;
            if (ImGui::BeginPopup(colIdName))
            {
                ImGui::SeparatorText("Select Color");
                selected_color = GetColorPopup();
                if (selected_color != -1)
                {
                    editorEngineRef->Poke(dir + 3, selected_color);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
            ImGui::SameLine();

            bool h = (flag & (1 << 3));
            bool v = (flag & (1 << 4));

            if(ImGui::Checkbox(TextFormat("###h%i", id), &h))
            {
                editorEngineRef->Poke(dir + 4, setBit(flag,3,h));
            }
            ImGui::SameLine();

            if (ImGui::Checkbox(TextFormat("###v%i", id), &v))
            {
                editorEngineRef->Poke(dir + 4, setBit(flag,4,v));
            }
            ImGui::SameLine();

            int offsetPos[2] = { editorEngineRef->Peek(dir + 1), editorEngineRef->Peek(dir + 2)};
            ImGui::PushItemWidth(64);
            if (ImGui::InputInt2(TextFormat("###offset_%i", id), offsetPos))
            {
                editorEngineRef->Poke(dir + 1, offsetPos[0]);
                editorEngineRef->Poke(dir + 2, offsetPos[1]);
            }
            ImGui::PopItemWidth();
            ImGui::SameLine();
        }

        ImGui::PushItemWidth(100);
        const char* items[] = {  "Normal", "Rot 90", "Rot 180", "Rot 270", "Disabled" };
        int item_current_idx =  ((flag >> 7) & 1) ? 4 : flag & 0b11;
        const char* combo_preview_value = items[item_current_idx];

        if (ImGui::BeginCombo(TextFormat("###meta_flag_%i", id), combo_preview_value, ImGuiComboFlags_NoArrowButton))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(items[n], is_selected))
                {
                    item_current_idx = n;
                    
                    int currentValue = flag;
                    int firstThreeBits = n & 0x07;
                    int clearedValue = currentValue & ~0b10000111;

                    int combinedValue = clearedValue | firstThreeBits;  
                    editorEngineRef->Poke(dir + 4, n > 3 ? setBit(flag, 7, 1) : combinedValue);
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();
        ImGui::SameLine();
        if(ImGui::Button(TextFormat("%s###copy_%i", ICON_FA_COPY, id)))
        {
            for (int c = 0; c < 5; c++)
                metaLineCopy[c] = editorEngineRef->Peek(dir + c);
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Copy");
        }
        ImGui::SameLine();
        if(ImGui::Button(TextFormat("%s###paste_%i", ICON_FA_PASTE,id)))
        {
            for (int c = 0; c < 5; c++)
                editorEngineRef->Poke(dir + c, metaLineCopy[c]);
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Paste");
        }
    ImGui::EndGroup();
}

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) 
{ 
    return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); 
}
static inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs) 
{ 
    return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); 
}
static inline ImVec2 ImRotate(const ImVec2& v, float cos_a, float sin_a) 
{ 
    return ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a);
}
void ImageRotated(ImTextureID tex_id, ImVec2 center, ImVec2 size, float angle, ImVec2 uv0, ImVec2 uv1, ImU32 color)
{
    float rad = (angle * 3.141592f) / 180.0f;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float cos_a = cosf(rad);
    float sin_a = sinf(rad);
    ImVec2 pos[4] =
    {
        center + ImRotate(ImVec2(-size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
        center + ImRotate(ImVec2(+size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
        center + ImRotate(ImVec2(+size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a),
        center + ImRotate(ImVec2(-size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a)
    };
    ImVec2 uvs[4] = 
    { 
        ImVec2(uv0.x, uv0.y), 
        ImVec2(uv1.x, uv0.y), 
        ImVec2(uv1.x, uv1.y), 
        ImVec2(uv0.x, uv1.y) 
    };

    draw_list->AddImageQuad(tex_id, pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], color);
}

void Editor::DrawMetaExample()
{
    ImGuiIO& io = ImGui::GetIO();
    ImTextureID my_tex_id = &editorEngineRef->spriteManager->spriteTexture.id;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos = ImVec2(0.0f, 0.0f);
    ImVec2 size = ImVec2(64.0f * io.FontGlobalScale, 64.0f * io.FontGlobalScale);
    pos = ImGui::GetCursorScreenPos();
    
    ImVec2 backSize = ImVec2(0, 0);
    for (int id= 0; id<4; id++)
    {
        int dir = (currentMetaSprite * 20 + 2096);
        dir += id * 5;
        float maxX= ((editorEngineRef->Peek(dir + 1)+8) * 8 )* io.FontGlobalScale;
        float maxY= ((editorEngineRef->Peek(dir + 2)+8) * 8 )* io.FontGlobalScale;
        if(maxX>backSize.y)
            backSize.x = maxX;
        if(maxY>backSize.y)
            backSize.y = maxY;
    }

    draw_list->AddRectFilled(pos, ImVec2(pos.x + backSize.x, pos.y + backSize.y), 
                            IM_COL32Col(drawMetaExampleColor));

    for (int id= 0; id<4; id++)
    {
        int dir = (currentMetaSprite * 20 + 2096);
        dir += id * 5;
        int sprite = editorEngineRef->Peek(dir);
        int flag = editorEngineRef->Peek(dir + 4);
        if (((flag >> 7) & 1)) continue;

        bool h = (flag & (1 << 3));
        bool v = (flag & (1 << 4));

        float x = sprite % 16; 
        float y = sprite / 16;

        ImVec2 uv0, uv1 = ImVec2(0.0f ,0.0f);

        uv0.x =  h ?  x * 0.0625f + 0.0625f : x * 0.0625f;
        uv1.x = !h ?  x * 0.0625f + 0.0625f : x * 0.0625f;
        uv0.y =  v ?  y * 0.0625f + 0.0625f : y * 0.0625f;
        uv1.y = !v ?  y * 0.0625f + 0.0625f : y * 0.0625f;

        float px = (editorEngineRef->Peek(dir + 1) * 8 )* io.FontGlobalScale;
        float py = (editorEngineRef->Peek(dir + 2) * 8 )* io.FontGlobalScale;

        ImVec2 spritePos = ImVec2(pos.x + px, pos.y + py);

        Color bcol = editorEngineRef->spriteManager->GetColor(editorEngineRef->Peek(dir + 3));
        ImVec4 color = ImVec4(bcol.r / 255.0f, bcol.g / 255.0f, bcol.b / 255.0f, 1.0f);

        ImageRotated(my_tex_id, spritePos + size * ImVec2(0.5f, 0.5f), size, (flag & 0x07) * 90, uv0, uv1, ImGui::ColorConvertFloat4ToU32(color)); 
    }
}

void Editor::DrawMetaSprites(int metaId)
{
    ImGui::BeginGroup();

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int id = x + y * 8;
            char button_label[5];
            sprintf(button_label, "%02i" , id);
            ImGui::PushID(id);
          
            ImVec4 color = Col_S2_M;

            int dir = (id * 20 + 2096);
            int flag = editorEngineRef->Peek(dir + 4) + editorEngineRef->Peek(dir + 9) + 
                       editorEngineRef->Peek(dir + 14) + editorEngineRef->Peek(dir + 19);
        
            if (flag == 512) 
                color = Col_S2_D2;
            
            if (currentMetaSprite == id) 
                color = Col_P_B1;

            ImGui::PushStyleColor(ImGuiCol_Button, color);
            if(ImGui::Button(button_label, ImVec2(40,40)))
            {
                currentMetaSprite = id;
                HighLightMemory(currentMetaSprite * 20 + 2096, 20);
            }
            ImGui::PopStyleColor();
            ImGui::PopID();
            ImGui::SameLine();
        }
        ImGui::NewLine();
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();

    ImGui::PushStyleColor(ImGuiCol_Text, Col_T_2);
    ImGui::Text("Sprite   Flags Position");
    ImGui::Text("ID  Col  H  V   X  Y  Mode");
    ImGui::PopStyleColor();

    for ( int i = 0; i<4; i++)
    {
        DrawMetaLine(i);
    }

    if(ImGui::Button("Bg Col"))
    {
        ImGui::OpenPopup("Bg_col");
    }
    ImGui::SameLine();
    int dir = (currentMetaSprite * 20 + 2096);
    if(ImGui::Button("Copy"))
    {
        for (int c = 0; c < 20; c++)
            metaSpriteCopy[c] = editorEngineRef->Peek(dir + c);
    }
    ImGui::SameLine();
    if(ImGui::Button("Paste"))
    {
        for (int c = 0; c < 20; c++)
             editorEngineRef->Poke(dir + c, metaSpriteCopy[c]);
    }
    static int selected_color = -1;
    if (ImGui::BeginPopup("Bg_col"))
    {
        ImGui::SeparatorText("Select Color");
        selected_color = GetColorPopup();
        if (selected_color != -1)
        {
            drawMetaExampleColor = editorEngineRef->spriteManager->GetColor(selected_color);
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    DrawMetaExample();

    ImGui::EndGroup();

}

void Editor::SetMousePosInEditor(int x, int y)
{
    x = (x / iScale) + mouseEditorPositionX;
    y = ((y / iScale) + mouseEditorPositionY) - mouseEditorOffset;
    SetMousePosition(x,y);
}
void Editor::ChangeLayout(uint8_t layout)
{
    SaveUIJson();
    currentLayout = layout;
    LoadUIJson();
}

void Editor::Package()
{
    std::string buildFolder = "builds/";
    std::string name = editorEngineRef->bios->CurrentProject.name;

    if (!std::filesystem::exists(buildFolder + name)) 
    {
        Tools::console->AddLog( "Packaging: %s", name.c_str());
        std::filesystem::create_directory(buildFolder);
        std::filesystem::create_directory(buildFolder + name);
        std::filesystem::create_directory(buildFolder + name + "/" + ASSETS_FOLDER);
        std::filesystem::create_directory(buildFolder + name + "/" + CONFIG_FOLDER);
    } 
    else 
    {
        Tools::console->AddLog( "[ERROR] Folder %s is already exist, please, delete it.", (buildFolder + name).c_str());
        return;
    }

    std::string exe = name + "/" + name +".exe";
    std::string bas = editorEngineRef->bios->CurrentProject.programFile;
    std::string dataFile = editorEngineRef->bios->CurrentProject.memoryFile;

    try {
        Tools::console->AddLog( "Copying: %s", name.c_str());
        std::filesystem::copy("enginsito.exe", buildFolder + exe);
        std::filesystem::copy(bas,  buildFolder +name + "/" + ASSETS_FOLDER + "/" + name + PROGRAM_EXTENSION);
        std::filesystem::copy(dataFile, buildFolder +name + "/" + ASSETS_FOLDER + "/" + name + MEM_EXTENSION);
    } catch (const std::filesystem::filesystem_error& e) {
        Tools::console->AddLog( "[ERROR] Error copying: %s, %s", name.c_str(), e.what());
        return;
    }
    
    std::string bootFile = buildFolder + name + "/" + BOOT_FILE;
    std::string bootText = "editor 0\nfullscreen\nload "+ name +"\nrun";
    std::ofstream outputFile(bootFile);
    if (outputFile.is_open()) 
    {
        outputFile << bootText << std::endl;
        outputFile.close(); 
        Tools::console->AddLog("Boot file OK:");
    } 
    else 
    {
        Tools::console->AddLog("[ERROR] Boot file Error");
        return;
    }
}

void Editor::Draw()
{
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar;

    if (ScreenWindowHasFocus) window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
        
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Main", NULL, window_flags);
    ImGui::PopStyleVar();

    if (ImGui::BeginMenuBar())
    {
        if (io.KeyCtrl && io.KeyAlt) 
        {
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_1))) ChangeLayout(0);
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_2))) ChangeLayout(1);
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_3))) ChangeLayout(2);
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_4))) ChangeLayout(3);
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_5))) ChangeLayout(4);
        }

        if (ImGui::BeginMenu("Editor"))
        {
            if (ImGui::MenuItem("Toggle editor", "F1", nullptr, true))
                editorEngineRef->bios->ShouldRecenter = true;
            if (ImGui::MenuItem("Run", "F5", nullptr, true))
                editorEngineRef->bios->ShouldRun = true;
            ImGui::Separator();
            if (ImGui::MenuItem("Recenter window", "F2", nullptr, true))
                editorEngineRef->postProcessing->UpdateWindowSize();
            if (ImGui::MenuItem("Toogle FullScreen", "F11", nullptr, true))
                editorEngineRef->postProcessing->FullScreen();
            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "Alt+F4", nullptr, true))
                editorEngineRef->bios->ShouldClose = true;
            ImGui::EndMenu();
        }
                
        if (ImGui::BeginMenu("View"))
        {
                ImGui::MenuItem("Tools", NULL, &show_tools);
                ImGui::MenuItem("FPS", NULL, &show_FPS);
                ImGui::Separator();
                ImGui::MenuItem("Player", NULL, &show_player);
                ImGui::MenuItem("Screen", NULL, &show_screen);
                ImGui::Separator();
                ImGui::MenuItem("Code", NULL, &show_code);
                ImGui::MenuItem("Console", NULL, &show_console);
                ImGui::Separator();
                ImGui::MenuItem("Crt", NULL, &show_crt);
                ImGui::MenuItem("Palette", NULL, &show_palette);
                ImGui::MenuItem("Memory", NULL, &show_memory);
                ImGui::MenuItem("File Browser", NULL, &show_filebrowser);
                ImGui::Separator();
                ImGui::MenuItem("Sprites", NULL, &show_sprites);
                ImGui::MenuItem("Make Sprite", NULL, &show_makeSprite);
                ImGui::MenuItem("Meta Sprite", NULL, &show_metaSprite);
                ImGui::Separator();
                ImGui::MenuItem("SFX", NULL, &show_sfx);

                #ifdef DEBUG
                ImGui::MenuItem("Demo", NULL, &show_demo);
                #endif

                ImGui::EndMenu();
        }
            
        if (ImGui::BeginMenu(TextFormat("Layouts(%i)", currentLayout + 1)))
        {
                if(ImGui::MenuItem("Layout 1", "Ctrl-Alt-1", currentLayout == 0)) {
                    ChangeLayout(0);
                }  
                if(ImGui::MenuItem("Layout 2", "Ctrl-Alt-2", currentLayout == 1)) {      
                    ChangeLayout(1);
                }       
                if(ImGui::MenuItem("Layout 3", "Ctrl-Alt-3", currentLayout == 2)) {
                    ChangeLayout(2);
                } 
                if(ImGui::MenuItem("Layout 4", "Ctrl-Alt-4", currentLayout == 3)) {
                    ChangeLayout(3);
                } 
                if(ImGui::MenuItem("Layout 5", "Ctrl-Alt-5", currentLayout == 4)) {
                    ChangeLayout(4);
                } 
                ImGui::Separator();
                if(ImGui::MenuItem("Close All", NULL)) {
                    CloseAll();
                }
                ImGui::EndMenu();
        }   

        if (ImGui::BeginMenu("Help"))
        {                
            ImGui::MenuItem("Documentation", NULL, &show_docs);
            ImGui::Separator();
            ImGui::MenuItem("About", NULL, &show_credits);        
            ImGui::EndMenu();
        }  
        if(editorEngineRef->bios->CurrentProject.name != "")
        {
            if (ImGui::BeginMenu("Build"))
            {                
                if(ImGui::MenuItem("Package", NULL, nullptr))
                    Package();       
                ImGui::EndMenu();
            }  
        }
        ImGui::EndMenuBar();
    }
        
        ImGuiID dockspace_id = ImGui::GetID("DockId");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f),dockspace_flags);

        if(show_credits)
        {
            Credits();      
        }

        if(show_tools)
        {
            ImGui::Begin("Tools", &show_tools, ImGuiWindowFlags_NoCollapse);
            
            if (ImGui::CollapsingHeader("Editor"))
            {
                ImGui::PushItemWidth(120.0f);
                ImGui::DragFloat("Font size", &io.FontGlobalScale, 0.05f, 0.5f, 2.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
                ImGui::SameLine();
                if (ImGui::Button("Nes"))
                {
                    std::string colors[17] = {
                    "FF8080",
                    "FF7070",
                    "5D5678",
                    "5D45B5",
                    "6F61A3",
                    "342F48",
                    "1E1833",
                    "769667", 
                    "75DF42", 
                    "8DCC6F", 
                    "425B36", 
                    "264019", 
                    "AD7D77", 
                    "FF604C", 
                    "EC8D80", 
                    "69433F", 
                    "4A221D"
                    };
                    LoadEditorPalette(colors);
                }
            }
            if (ImGui::CollapsingHeader("Sequencer"))
            {
                DrawToolsSequence();
            }
            if (ImGui::CollapsingHeader("Synth tester"))
            {
                DrawToolsPiano();
            }
            ImGui::End(); 
        }

        if (show_FPS)
        {
            ImGui::Begin("FPS", &show_FPS, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
                DrawFPS();
            ImGui::End();
        }

        #ifdef DEBUG
        if(show_demo)
        {
            ImGui::ShowDemoWindow(&show_demo);
        }
        #endif
            
        //Console
        if (show_console)
        {
            Tools::console->Draw(&show_console);
        }

        if (show_filebrowser)
        {
            ImGui::Begin("File browser", &show_filebrowser, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
                DrawshowFileBrowser();
            ImGui::End();    
        }

        if (show_screen)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Screen", &show_screen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
                ScreenWindowHasFocus = ImGui::IsWindowFocused();
                ImVec2 windowSize = ImGui::GetWindowSize();

                ImVec2 windowPositionTotal= ImGui::GetWindowPos();
                ImVec2 windowPosition= ImGui::GetCursorScreenPos();
                mouseEditorOffset = windowPosition.y - windowPositionTotal.y;

                windowSize.y -= mouseEditorOffset;

                float scale = (windowSize.x/windowSize.y < GAME_RATIO) ? windowSize.x/(float)GAME_SCREEN_W : windowSize.y/(float)GAME_SCREEN_H;
                ImVec2 imageSize = ImVec2(GAME_SCREEN_W * scale, GAME_SCREEN_H * scale); 
                ImVec2 imagePos = ImVec2((windowSize.x - imageSize.x) / 2, (windowSize.y - imageSize.y) / 2);
                imagePos.y += mouseEditorOffset;
                ImGui::SetCursorPos(imagePos);
                ImGui::Image(&editorEngineRef->postProcessing->editorRender.texture, imageSize, ImVec2(0,0), ImVec2(1,-1));
                
                if(ScreenWindowHasFocus)
                {
                    iScale = 1.0f / scale;
                    mouseEditorPositionX = imagePos.x + windowPosition.x;
                    mouseEditorPositionY = imagePos.y + windowPosition.y;
                    editorEngineRef->VirtualMouseX = (ImGui::GetMousePos().x - imagePos.x - windowPosition.x) * iScale;
                    editorEngineRef->VirtualMouseY = ((ImGui::GetMousePos().y + mouseEditorOffset) - imagePos.y - windowPosition.y) * iScale;
                }

                MouseInsideScreenWindow = editorEngineRef->VirtualMouseX > 0 && editorEngineRef->VirtualMouseX < GAME_SCREEN_W;
                MouseInsideScreenWindow &= editorEngineRef->VirtualMouseY > 0 && editorEngineRef->VirtualMouseY < GAME_SCREEN_H;

            ImGui::End();
            ImGui::PopStyleVar();
        }
            
        if(show_palette)
        {
            ImGui::Begin("Palette", &show_palette, ImGuiWindowFlags_NoCollapse);
                DrawPalette();
            ImGui::End();
        }

        if (show_crt)
        {
            ImGui::Begin("Crt", &show_crt, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar); 
                DrawCRT();
            ImGui::End();
        }
            
        if (show_player)
        {
            ImVec2 minHeight(200, 100);
            ImVec2 maxHeight(2000, 100);
            ImGui::SetNextWindowSizeConstraints(minHeight, maxHeight);
            ImGui::Begin("Player", &show_player, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
                DrawPlayer();
            ImGui::End();
        }

        if(show_memory)
        {
            ImGui::Begin("Memory", &show_memory, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar);
                DrawMemory();
            ImGui::End();
        }

        if (show_sprites)
        {
            ImGui::Begin("Sprites", &show_sprites, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar);
                ImGui::BeginGroup();
                    DrawSprites();
                ImGui::EndGroup();
            ImGui::End();
        } 

        if (show_makeSprite)
        {
            ImGui::Begin("Make Sprite", &show_makeSprite, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar);
                MakeSprite(currentSprite);
            ImGui::End(); 
        }

        if (show_metaSprite)
        {
            ImGui::Begin("Meta Sprite", &show_metaSprite, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar);
               DrawMetaSprites(currentMetaSprite);
            ImGui::End(); 
        }

        if (show_sfx)
        {
            ImGui::Begin("SFX", &show_sfx, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar);
                DrawSFX();
            ImGui::End();
        }

        if (show_code)
        {
            DrawCode(&show_code);
        }

        if (show_docs)
        {
            DrawDocs(&show_docs);
        }
        
        //Hack ¿?¿?¿?, if you remove this, ImGui fails. ¯\_(ツ)_/¯
        rlImGuiImageRect(&hackTexture, 1, 1, (Rectangle){0, 0, 1, 1});

    ImGui::End();
}


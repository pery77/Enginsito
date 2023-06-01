#include "editor.h"
#include "nlohmann/json.hpp"

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

static unsigned char spriteCopy[8]{};

int currentSprite = 0;
int currentMetaSprite = 0;
nlohmann::json data;

Editor::Editor(Engine* _engine)
{
    editorEngineRef = _engine;
    codeEditor.SetLanguageDefinition(lang);
    codeEditor.SetPalette(TextEditor::GetBasicPalette()); 

    mem_edit.HighlightColor = IM_COL32(22, 110, 162, 255);
    mem_edit.OptShowAscii = false;

    Image hackImage = GenImageColor(1,1,(Color){0,0,0,0});
    hackTexture = LoadTextureFromImage(hackImage);

	std::stringstream ss;
	ss << CONFIG_FOLDER << "/ui.json";

    std::ifstream f(ss.str().c_str());
    data = nlohmann::json::parse(f);

    show_player = data["show_player"].get<bool>();
    show_FPS = data["show_fps"].get<bool>();
    show_tools = data["show_tools"].get<bool>();
    show_screen = data["show_screen"].get<bool>();
    show_code = data["show_code"].get<bool>();
    show_console = data["show_console"].get<bool>();
    show_palette = data["show_palette"].get<bool>();
    show_crt = data["show_crt"].get<bool>();
    show_memory = data["show_memory"].get<bool>();
    show_filebrowser = data["show_filebrowser"].get<bool>();
    show_sprites = data["show_sprites"].get<bool>();
    show_makeSprite = data["show_makeSprite"].get<bool>();
    show_metaSprite = data["show_metaSprite"].get<bool>();
    show_sfx = data["show_sfx"].get<bool>();

    SetMainWindow();
}

Editor::~Editor()
{
    std::stringstream ss;
	ss << CONFIG_FOLDER << "/ui.json";

    data["window_width"] = GetScreenWidth();
    data["window_height"] = GetScreenHeight();
    data["window_position_x"] = window_position_x;
    data["window_position_y"] = window_position_y;

    data["show_player"] = show_player;
    data["show_fps"] = show_FPS;
    data["show_tools"] = show_tools;
    data["show_screen"] = show_screen;
    data["show_code"] = show_code;
    data["show_console"] = show_console;
    data["show_palette"] = show_palette;
    data["show_crt"] = show_crt;
    data["show_memory"] = show_memory;
    data["show_filebrowser"] = show_filebrowser;
    data["show_sprites"] = show_sprites;
    data["show_makeSprite"] = show_makeSprite;
    data["show_metaSprite"] = show_metaSprite;
    data["show_sfx"] = show_sfx;

    std::ofstream o(ss.str().c_str());
    o << std::setw(4) << data << std::endl;
}

void Editor::SetMainWindow()
{
    BeginDrawing();
        ClearBackground(BLACK);
    EndDrawing();
    SetWindowPosition(data["window_position_x"].get<int>(), data["window_position_y"].get<int>());
    SetWindowSize(data["window_width"].get<int>(), data["window_height"].get<int>());
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
    int frameH = ImGui::GetFrameHeight() + 5;
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
    ImGui::PlotLines("", values, IM_ARRAYSIZE(values), values_offset,overlay ,0.0f, 60.0f, ImVec2(pw_size.x, pw_size.y - frameH));
    ImGui::PopStyleColor();  
}

void Editor::Credits()
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (!ImGui::IsPopupOpen("Credits"))
        ImGui::OpenPopup("Credits");

    if(ImGui::BeginPopupModal("Credits", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {    
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 1.0f, 1.0f), "Programed by Pery - 2023");
        ImGui::Separator();
        ImGui::Text("Credits:");
        ImGui::Text("This application utilizes the following open-source libraries:");

        ImGui::Spacing();

        ImGui::BulletText("Raylib      - github.com/raysan5/raylib");
        ImGui::BulletText("My-Basic    - github.com/paladin-t/my_basic/");
        ImGui::BulletText("ImGui       - github.com/ocornut/imgui");
        ImGui::BulletText("Json        - github.com/nlohmann/json");
        ImGui::BulletText("Memory      - github.com/ocornut/imgui_club");
        ImGui::BulletText("Text Editor - github.com/BalazsJako/ImGuiColorTextEdit");
        ImGui::BulletText("Imgui Knobs - github.com/altschuler/imgui-knobs");
        ImGui::BulletText("rlImGui     - github.com/raylib-extras/rlImGui");
        ImGui::BulletText("MML_Parser  - github.com/vcraftjp/MML-Parser");
        ImGui::BulletText("FontAwesome - fontawesome.com");
        ImGui::BulletText("Font        - fontspace.com/mozart-nbp-font-f18977");

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

void Editor::DrawshowFileBrowser()
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

void Editor::DrawCode()
{
        auto cpos = codeEditor.GetCursorPosition();

        ImGui::Begin("Code Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar);

/*
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
*/
	    	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, codeEditor.GetTotalLines(),
	    		codeEditor.IsOverwrite() ? "Ovr" : "Ins",
	    		codeEditor.CanUndo() ? "*" : " ",
                editorEngineRef->bios->CurrentProgram.c_str());

            codeEditor.Render("TextEditor");

        ImGui::End();
    }

void Editor::DrawPalette()
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
        if((c+1) % 8 != 0) ImGui::SameLine();

        editorEngineRef->spriteManager->SetColor(c, color.x * 255, color.y * 255, color.z * 255);

        if (ImGui::IsItemHovered())
        {
            HighLightMemory(c*3,3);
        }
    }
}

void Editor::SpriteRect(int id, ImVec2 pos, ImVec2 size, int x, int y, ImTextureID my_tex_id) 
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec4 bg_col = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    ImVec4 bg_col_selected = ImVec4(0.1f, 0.5f, 0.9f, 1.0f);       

    ImVec2 uv0 = ImVec2(x * 0.0625f, y * 0.0625f);
    ImVec2 uv1 = ImVec2(x * 0.0625f + 0.0625f, y * 0.0625f + 0.0625f);
    ImVec4 col = currentSprite == id ? bg_col_selected : bg_col;

    draw_list->AddImage(my_tex_id, pos, ImVec2(pos.x + size.x, pos.y + size.y), uv0, uv1);
    draw_list->AddRect(pos, ImVec2(pos.x + size.x + 2, pos.y + size.y + 2),IM_COL32(20, 20, 20, 255), 0, 0, 4);

    ImGui::SetCursorScreenPos(pos);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 1.0f));
    if (ImGui::ImageButton("###", my_tex_id, size, uv0, uv1, col))
    {
        HighLightMemory(id * 8 + 48, 8);
        currentSprite = id;
    }
    ImGui::PopStyleVar();
}

static int metaSpriteRectSelected = -1;
void Editor::MetaSpriteRect(int id, ImVec2 pos, ImVec2 size, int x, int y, ImTextureID my_tex_id) 
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec4 col = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
  
    ImVec2 uv0 = ImVec2(x * 0.0625f, y * 0.0625f);
    ImVec2 uv1 = ImVec2(x * 0.0625f + 0.0625f, y * 0.0625f + 0.0625f);

    draw_list->AddImage(my_tex_id, pos, ImVec2(pos.x + size.x, pos.y + size.y), uv0, uv1);
    draw_list->AddRect(pos, ImVec2(pos.x + size.x + 2, pos.y + size.y + 2),IM_COL32(20, 20, 20, 255), 0, 0, 4);

    ImGui::SetCursorScreenPos(pos);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 1.0f));
    if (ImGui::ImageButton("###", my_tex_id, size, uv0, uv1, col))
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

    ImGui::BeginGroup();
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            int id = x + y * 16;
            ImGui::PushID(id);
            SpriteRect(id, pos, size, x, y, my_tex_id);
            pos.x += size.x + 4; 
            ImGui::PopID();
        }
        pos.x = p.x;
        pos.y += size.y + 4; 
    }
    ImGui::EndGroup();
    ImGui::SameLine();

    ImGui::BeginGroup();
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

void Editor::DrawPlayer()
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 playerSize = ImGui::GetWindowSize();
    ImVec2 buttonSize = ImVec2(32.0f * io.FontGlobalScale, 32.0f * io.FontGlobalScale);

    float buttonPosX = (playerSize.x - buttonSize.x * 5) / 2;
    float buttonPosY = (playerSize.y - buttonSize.y) / 2;

    ImGui::SetCursorPosX(buttonPosX);
    //ImGui::SetCursorPosY(buttonPosY);

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

void Editor::PianoKey(ImVec2 pos, ImVec2 size, int note, bool isBlack, bool pressed) 
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

bool Editor::IsBlack(int note)
{
    int n = note % 12;
    return (n == 1 || n == 3 || n == 6 || n == 8 || n == 10);
}

    void Editor::DrawSFX()
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


inline unsigned char setBit(unsigned char byte, int position, bool newState) {
    unsigned char mask = 1 << position;
    
    if (newState) {
        byte |= mask; // set bit to 1
    } else {
        byte &= ~mask; // set bit to 0
    }
    
    return byte;
}

void Editor::PixelRect(int dir, uint8_t bit, ImVec2 pos, ImVec2 size, bool state) 
{
    ImVec4 color = state ? ImVec4(.9f, .9f, .9f, 1.0f) : ImVec4(.1f, .1f, .1f, 1.0f);

    ImGui::SetCursorScreenPos(pos);

    ImGui::PushStyleColor(ImGuiCol_Button, color);

    if (ImGui::Button("###", size)) 
    {
        HighLightMemory(dir, 1);
        editorEngineRef->Poke(dir,setBit(editorEngineRef->Peek(dir), bit, !state));
    }
    ImGui::PopStyleColor();
}

void Editor::MakeSprite(int spriteId)
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec2 size = ImVec2(32.0f,32.0f);
    uint8_t byte = 0;

    ImGui::Text("Memory address: %.3X", currentSprite * 8 + 48);

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 p = pos;

    static unsigned char copyByte;

    ImGui::BeginGroup();

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int id = x + y * 8;
            byte = editorEngineRef->Peek((currentSprite * 8 + 48) + y);

            ImGui::PushID(id);

            unsigned char mascara = 1 << 7-x;

            PixelRect((currentSprite * 8 + 48) + y, 7-x, pos, size, (byte & mascara) != 0);
            pos.x += size.x + 1; 
            
            ImGui::PopID();
        }

        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::Text("%.2X", byte);
        ImGui::SameLine();
        ImGui::PushID(y);
        if(ImGui::SmallButton("Copy"))
        {
            copyByte = byte;
        }
        ImGui::PopID();
        ImGui::SameLine();
        ImGui::PushID(y+8);
        if(ImGui::SmallButton("Paste"))
        {
            editorEngineRef->Poke((currentSprite * 8 + 48) + y, copyByte);
        }
        ImGui::PopID();
        ImGui::EndGroup();
        pos.x = p.x;
        pos.y += size.y + 1; 
    }
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
    static int value = -1;
    ImGui::BeginGroup();
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            int id = x + y * 16;
            ImGui::PushID(id);
            MetaSpriteRect(id, pos, size, x, y, my_tex_id);
            pos.x += size.x + 4; 
            ImGui::PopID();
        }
        pos.x = p.x;
        pos.y += size.y + 4; 
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

    ImGui::BeginGroup();

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
        ImVec4 color = ImVec4(bcol.r / 255.0f, bcol.g / 255.0f, bcol.b / 255.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        if(ImGui::Button(TextFormat("%02i###%04i", col, dir + 3)))
        {
            ImGui::OpenPopup(colIdName);
        }
        ImGui::PopStyleColor(1);
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

        int flag = editorEngineRef->Peek(dir + 4);

        if (!((flag >> 7) & 1)) {
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

            ImGui::Button("X");
            ImGui::SameLine();
            ImGui::Button("Y");
            ImGui::SameLine();
        }


        ImGui::PushItemWidth(100);
        //ImGui::Combo(TextFormat("###meta_flag_%i", id), &item_current, "Disabled\0Normal\0Rot 90\0Rot 180\0Rot 270\0\0");
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

    ImGui::EndGroup();
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
          
            ImVec4 color = currentMetaSprite == id ?
                ImVec4(0.8f, 0.8f, 0.9f, 1.0f):
                ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
            ImGui::PushStyleColor(ImGuiCol_Button, color);
            if(ImGui::Button(button_label))
            {
                currentMetaSprite = id;
                HighLightMemory(currentMetaSprite * 20 + 2096, 20);
            }
            ImGui::PopStyleColor(1);
            ImGui::PopID();
            ImGui::SameLine();
        }
        ImGui::NewLine();
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    for ( int i = 0; i<4; i++)
    {
        DrawMetaLine(i);
    }

    ImGui::EndGroup();

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
        
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_AutoHideTabBar;
        
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Main", NULL, window_flags);
    ImGui::PopStyleVar();

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
            
            if (ImGui::BeginMenu("Help"))
            {
                ImGui::MenuItem("About", NULL, &show_credits);        
                ImGui::EndMenu();
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
                ImGui::DragFloat("Font", &io.FontGlobalScale, 0.05f, 0.5f, 2.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
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
            Tools::console->Draw();
        }

        if (show_filebrowser)
        {
            ImGui::Begin("File browser", &show_filebrowser, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
                DrawshowFileBrowser();
            ImGui::End();    
        }

        if (show_FPS)
        {
            ImGui::Begin("FPS", &show_FPS, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse);
                DrawFPS();
            ImGui::End();
        }

        if (show_screen)
        {
            ImGui::Begin("Screen", NULL, ImGuiWindowFlags_NoCollapse);
                ScreenWindowHasFocus = ImGui::IsWindowFocused();
                ImVec2 windowSize = ImGui::GetWindowSize();

                ImVec2 windowPositionTotal= ImGui::GetWindowPos();
                ImVec2 windowPosition= ImGui::GetCursorScreenPos();
                float offset = windowPosition.y - windowPositionTotal.y;

                windowSize.y -= offset;

                //ImGui::BeginTooltip();
                //    ImGui::Text("%f", offset);
                //ImGui::EndTooltip();

                float scale = (windowSize.x/windowSize.y < GAME_RATIO) ? windowSize.x/(float)GAME_SCREEN_W : windowSize.y/(float)GAME_SCREEN_H;
                ImVec2 imageSize = ImVec2(GAME_SCREEN_W * scale, GAME_SCREEN_H * scale); 
                ImVec2 imagePos = ImVec2((windowSize.x - imageSize.x) / 2, (windowSize.y - imageSize.y) / 2);
                imagePos.y += offset;
                ImGui::SetCursorPos(imagePos);
                ImGui::Image(&editorEngineRef->postProcessing->editorRender.texture, imageSize, ImVec2(0,0), ImVec2(1,-1));
                
                if(ScreenWindowHasFocus)
                {
                    editorEngineRef->VirtualMouseX = (ImGui::GetMousePos().x - imagePos.x - windowPosition.x) * 1.0f / scale;
                    editorEngineRef->VirtualMouseY = ((ImGui::GetMousePos().y - imagePos.y - windowPosition.y) * 1.0f / scale) + offset;
                }

                MouseInsideScreenWindow = editorEngineRef->VirtualMouseX > 0 && editorEngineRef->VirtualMouseX < GAME_SCREEN_W;
                MouseInsideScreenWindow &= editorEngineRef->VirtualMouseY > 0 && editorEngineRef->VirtualMouseY < GAME_SCREEN_H;

            ImGui::End();
        }
            
        if(show_palette)
        {
            ImGui::Begin("Palette", NULL, ImGuiWindowFlags_NoCollapse);
                DrawPalette();
            ImGui::End();
        }

        if (show_crt)
        {
            ImGui::Begin("CRT", NULL, ImGuiWindowFlags_NoCollapse); 
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
            ImGui::Begin("Memory", NULL, ImGuiWindowFlags_NoCollapse);
                DrawMemory();
            ImGui::End();
        }

        if (show_sprites)
        {
            ImGui::Begin("Sprites", NULL, ImGuiWindowFlags_NoCollapse);
                ImGui::BeginGroup();
                    DrawSprites();
                ImGui::EndGroup();
            ImGui::End();
        } 

        if (show_makeSprite)
        {
            ImGui::Begin("Make Sprite", NULL, ImGuiWindowFlags_NoCollapse);
                MakeSprite(currentSprite);
            ImGui::End(); 
        }

        if (show_metaSprite)
        {
            ImGui::Begin("Meta Sprite", NULL, ImGuiWindowFlags_NoCollapse);
               DrawMetaSprites(currentMetaSprite);
            ImGui::End(); 
        }

        if (show_sfx)
        {
            ImGui::Begin("SFX", NULL, ImGuiWindowFlags_NoCollapse);
                DrawSFX();
            ImGui::End();
        }

        if (show_code)
        {
            DrawCode();
        }



        //Hack ¿?¿?¿?, if you remove this, ImGui fails. ¯\_(ツ)_/¯
        rlImGuiImageRect(&hackTexture, 1, 1, (Rectangle){0, 0, 1, 1});

    ImGui::End();
}


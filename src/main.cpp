#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "my_basic.h"

#include <vector>

#define TSF_IMPLEMENTATION
#define RFXGEN_IMPLEMENTATION
#include "tools.h"

#include "mb_manager.h"
#include "wren_manager.h"
#include "postProcessing.h"

#include "bios.h"

#include <iostream>
#include "FileWatcher.h"

#include "imgui/rlImGui.h"

FilePathList droppedFiles = { 0 };
char peName[32];

void dropFile(){

    if (IsFileDropped())        {
        // Is some files have been previously loaded, unload them
        if (droppedFiles.count > 0) UnloadDroppedFiles(droppedFiles);
        
        // Load new dropped files
        droppedFiles = LoadDroppedFiles();
    }
    if (droppedFiles.count > 0)
    {
        printf("file: %s\n", *droppedFiles.paths[0]);
        UnloadDroppedFiles(droppedFiles);
    }
}

void drawImGuiManger(){

    ImGui::Begin(peName);
        static bool showConsole = false;
        ImGui::Checkbox("Console", &showConsole);

    if (showConsole){
        ImGui::Begin("Console");

        ImGui::End();
    }

    if (ImGui::CollapsingHeader("Info")){
        ImGui::SeparatorText("FPS");
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
        ImGui::SeparatorText("Program");
        ImGui::Text("Current Program:");
        ImGui::SeparatorText("Memory");
        ImGui::Text("Current Memory:");
    }

    if (ImGui::CollapsingHeader("Palette"))
    {
    }
    if (ImGui::CollapsingHeader("Graphics"))
    {
    }
    if (ImGui::CollapsingHeader("CRT filter"))
    {
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


    ImGui::End();
}

int main(int argc, char *argv[]){
    sprintf(peName, "peryEngine v: %.3f", PE_VERSION / 1000.0f);
    std::stringstream ss;
    ss << "./" << ASSETS_FOLDER << "/";
    FileWatcher* fw = new FileWatcher{ss.str(), 3.0f};

    const int windowWidth = GAME_SCREEN_W * 3;
    const int windowHeight = GAME_SCREEN_H * 3;

	//Set Vsync and make the window resizeable
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	//Create window
	InitWindow(windowWidth, windowHeight, peName);
	SetWindowMinSize(GAME_SCREEN_W, GAME_SCREEN_H);
	SetTargetFPS(GAME_FPS);

    rlImGuiSetup(true);

    InitAudioDevice();
    HideCursor();
    SetExitKey(KEY_NULL);

    Bios* bios = new Bios();
    PostProcessing* postProcessing = new PostProcessing();
    MBManager* basic = new MBManager(postProcessing);
    //DISABLED WREN AT MOMENT
    //WrenManager* wren = new WrenManager();

    GameState currentState = Off;
    bool showImgui = false;
    const char * pauseMessage = "Paused, press ESC again to exit.";
    int pauseMessageSize = MeasureTextEx(Tools::GetFont(),"Paused, press ESC again to exit.", 8,0).x * 0.5f;

    bios->LoadBoot();
    // Game Loop
    while (!(bios->ShouldClose || WindowShouldClose()))
    {
        fw->update([bios, postProcessing] (std::string path_to_watch, FileStatus status) -> void {
                if(!std::filesystem::is_regular_file(std::filesystem::path(path_to_watch)) && status != FileStatus::erased) {
            std::cout << "NO REGULAR: " << path_to_watch << '\n';
            return;
        }

        switch(status) {
            case FileStatus::created:
                std::cout << "File created: " << path_to_watch << '\n';
                break;
            case FileStatus::modified:
                std::cout << "File modified: " << path_to_watch << '\n';
                if (IsFileExtension(path_to_watch.c_str(),".bas")) {
                    bios->SetFile(path_to_watch);
                    bios->ShouldRun = true;
                    postProcessing->ReloadShaders();
                }
                else{
                    printf("Is not a .bas\n");
                }
                break;
            case FileStatus::erased:
                std::cout << "File erased: " << path_to_watch << '\n';
                break;
            default:
                std::cout << "Error! Unknown file status.\n";
        }
        });

        // Engine keys
        if(IsKeyReleased(KEY_F1)){
            showImgui = !showImgui;
        }

        if(IsKeyReleased(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyReleased(KEY_ENTER))){
		    postProcessing->FullScreen();
	    }

        if(IsKeyReleased(KEY_F10)){
		    postProcessing->ReloadShaders();
	    }

        if(IsWindowResized()) {
            postProcessing->UpdateGameScreenRects();
        }

        postProcessing->uTime = GetTime();
        
        //Interpreter
        if (IsKeyReleased(KEY_F5) || bios->ShouldRun){ 
            bios->ShouldRun = false;

            if (currentState == Running){
                basic->end();
            }

            if (basic->OpenBas(bios->GetFile().c_str()) == MB_FUNC_OK){
                basic->Run();
                basic->init();
                currentState = Running;
            }else{
                printf("%s not found.\n", bios->GetFile().c_str());
            }
        }
        if (IsKeyReleased(KEY_ESCAPE)){
            switch (currentState){
            case Running:
                currentState = Paused;
                break;
            case Paused:
                basic->end();
                currentState = Off;
                basic->CloseBas();
                break;
            default:
                break;
            }
        }

        if (currentState == Paused){
            int anyKey = GetKeyPressed();
            if (anyKey != 0 && anyKey != 256) currentState = Running; // key 256 is Escape key
        }

        // Update
        basic->UpdateAudio();

        if (currentState == Running){
            basic->tick();
            if(GetFrameTime()>10.0f) {
                //currentState = Off;
                //basic->CloseBas();
                printf("Bad framerate!\n");
            }
        }
        
        dropFile();

        // Draw
        BeginDrawing();

            //Draw game to texture.
            BeginTextureMode(postProcessing->mainRender);
                switch (currentState)
                {
                case Off:
                    bios->Update();
                    //DrawTexture(Tools::GetFont().texture,180,5,(Color){200,230,231,255});
                    break;
                case Running:
                    basic->draw();
                    break;
                case Paused:
                    DrawRectangle(0,88,320,12,Tools::GetBiosColor(2));
                    DrawRectangle(0,89,320,10,Tools::GetBiosColor(1));
                    DrawTextEx(Tools::GetFont(), pauseMessage,(Vector2){160 - pauseMessageSize, 90},8,0,Tools::GetBiosColor(3));
                    break;    
                default:
                    break;
                }

            EndTextureMode();

            // Main draw
            BeginBlendMode(0); // 0 Alpha (blur) 1 Additive (glow)
                postProcessing->RenderMain();
                postProcessing->RenderBlur();
            EndBlendMode();
   
            // Final Draw
            ClearBackground(BLACK);
            postProcessing->RenderFinal();

            // Engine over draw
            if(showImgui) {
                rlImGuiBegin();
                bool open = true;
                ImGui::ShowDemoWindow(&open);
                drawImGuiManger();
                rlImGuiEnd();
            }
    
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    basic->CloseBas();

    delete basic;
    delete postProcessing;

    return 0;
}
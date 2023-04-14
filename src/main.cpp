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

FilePathList droppedFiles = { 0 };

void CustomLog(int msgType, const char *text, va_list args) 
{
    std::string prefix;
    switch (msgType)
    {
        case LOG_INFO: prefix = "[INFO] : "; break;
        case LOG_ERROR: prefix = "[ERROR]: "; break;
        case LOG_WARNING: prefix = "[WARN] : "; break;
        case LOG_DEBUG: prefix = "[DEBUG]: "; break;
        default: break;
    }
    std::string str = prefix + text;
    char buffer[1024];
    vsprintf(buffer, str.c_str(), args);
    printf("%s\n", buffer);
    Tools::console->AddLog(buffer);
}

void dropFile() 
{
    if (IsFileDropped()) 
    {
        // Is some files have been previously loaded, unload them
        if (droppedFiles.count > 0) UnloadDroppedFiles(droppedFiles);
        
        // Load new dropped files
        droppedFiles = LoadDroppedFiles();
    }

    if (droppedFiles.count > 0)
    {
        char* firstFilePath = droppedFiles.paths[0];
        Tools::console->AddLog("[WARN] Dropped file: %s\n", firstFilePath);
    }
}

int main(int argc, char *argv[]){

    Tools::console->AddLog("Welcolme to %s", Tools::GetEngineName());
    SetTraceLogCallback(CustomLog);

    std::stringstream ss;
    ss << "./" << ASSETS_FOLDER << "/";
    FileWatcher* fw = new FileWatcher{ss.str(), 3.0f};

    const int windowWidth = GAME_SCREEN_W * 3;
    const int windowHeight = GAME_SCREEN_H * 3;

	//Set Vsync and make the window resizeable
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	//Create window
	InitWindow(windowWidth, windowHeight, Tools::GetEngineName());
	SetWindowMinSize(GAME_SCREEN_W, GAME_SCREEN_H);
	SetTargetFPS(GAME_FPS);

    rlImGuiSetup(true);

    InitAudioDevice();
    HideCursor();
    SetExitKey(KEY_NULL);

    Bios* bios = new Bios();
    PostProcessing* postProcessing = new PostProcessing();
    MBManager* basic = new MBManager(postProcessing);
    bios->postProcessingRef = postProcessing;
    bios->mbManagerRef = basic;

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
            Tools::console->AddLog("NO REGULAR: %s", path_to_watch.c_str());
            return;
        }

        switch(status) {
            case FileStatus::created:
                Tools::console->AddLog("File created: %s", path_to_watch.c_str());
                break;
            case FileStatus::modified:
                Tools::console->AddLog("File modified: %s",path_to_watch.c_str());
                if (IsFileExtension(path_to_watch.c_str(),".bas")) {
                    bios->SetFile(path_to_watch);
                    bios->ShouldRun = true;
                    postProcessing->ReloadShaders();
                }
                else{
                    Tools::console->AddLog("Is not a .bas");
                }
                break;
            case FileStatus::erased:
                Tools::console->AddLog("File erased: %s", path_to_watch.c_str());
                break;
            default:
                Tools::console->AddLog("Error! Unknown file status.");
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
                Tools::console->AddLog("%s not found.\n", bios->GetFile().c_str());
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
                Tools::console->AddLog("Bad framerate!\n");
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
                    if (!showImgui)
                    {
                        bios->Update();
                    }
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

                bios->DrawImGui();
                rlImGuiEnd();

                ShowCursor();
            }
            else
            {
                HideCursor();
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
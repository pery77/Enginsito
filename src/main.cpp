#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "my_basic.h"

#define TSF_IMPLEMENTATION
#define RFXGEN_IMPLEMENTATION
#include "tools.h"

#include "mb_manager.h"
#include "postProcessing.h"

#include "bios.h"

#include <iostream>
#include "FileWatcher.h"

int main(int argc, char *argv[]){

    IniManager* config = new IniManager();
    FileWatcher* fw = new FileWatcher{"./assets/", 3.0f};   

    const int windowWidth = GAME_SCREEN_W * config->size;
    const int windowHeight = GAME_SCREEN_H * config->size;

	//Set Vsync and make the window resizeable
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

	//Create window
	InitWindow(windowWidth, windowHeight, "peryEngine");
	SetWindowMinSize(GAME_SCREEN_W, GAME_SCREEN_H);
	SetTargetFPS(60);

    InitAudioDevice();
    HideCursor();
    SetExitKey(KEY_NULL);

    Bios* bios = new Bios();
    PostProcessing* postProcessing = new PostProcessing();
    MBManager* basic = new MBManager();

    GameState currentState = Off;
    bool showFps = false;
    const char * pauseMessage = "Paused, press ESC again to exit.";
    int pauseMessageSize = MeasureText("Paused, press ESC again to exit.", 8) * 0.5f;

    // Game Loop
    while (!(bios->ShouldClose || WindowShouldClose()))
    {
        fw->update([bios] (std::string path_to_watch, FileStatus status) -> void {
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
                bios->SetFile(path_to_watch);
                bios->ShouldRun = true;
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
            showFps = !showFps;
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
            Tools::CopyPalette();
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
                currentState = Off;
                basic->CloseBas();
                printf("Bad framerate!\n");
            }
        }

        // Draw
        BeginDrawing();

            //Draw game to texture.
            BeginTextureMode(postProcessing->mainRender);
                switch (currentState)
                {
                case Off:
                    bios->Update();
                    DrawRectangle(150,10,128,128,(Color){32,32,32,255});
                    DrawTexture(Tools::GetFont().texture,150,10,(Color){200,230,231,255});
                    break;
                case Running:
                    basic->draw();
                    break;
                case Paused:
                    DrawRectangle(0,89,320,12,Tools::GetFixedColor(15));
                    DrawRectangle(0,90,320,10,Tools::GetFixedColor(1));
                    DrawText(pauseMessage,160 - pauseMessageSize,90,8,Tools::GetFixedColor(15));
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
            if(showFps){
                DrawFPS(0, 0);
                postProcessing->uBlurPower = 
                    GuiSlider((Rectangle){0,50,300,20},"", TextFormat("%f",postProcessing->uBlurPower),postProcessing->uBlurPower ,0,20);
                postProcessing->uBlurFactor = 
                    GuiSlider((Rectangle){0,70,300,20},"", TextFormat("%f",postProcessing->uBlurFactor),postProcessing->uBlurFactor ,0.2,2);
                postProcessing->uTest = 
                    GuiSlider((Rectangle){0,90,300,20},"", TextFormat("%f",postProcessing->uTest),postProcessing->uTest ,-10,10);
                postProcessing->uCurvature = 
                    GuiSlider((Rectangle){0,110,300,20},"", TextFormat("%f",postProcessing->uCurvature),postProcessing->uCurvature ,0,1000);
            }
    
        EndDrawing();
    }

    CloseWindow();

    basic->CloseBas();

    delete config;
    delete basic;
    delete postProcessing;

    return 0;
}
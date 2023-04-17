#include "raylib.h"

#define RFXGEN_IMPLEMENTATION
#include "engine.h"

#include "wren_manager.h"
#include "mb_manager.h"
#include "postProcessing.h"
#include "bios.h"
#include "editor.h"

void raylibLog(int msgType, const char *text, va_list args) 
{
    std::string prefix;
    switch (msgType)
    {
        case LOG_INFO: prefix    = "[INFO] : "; break;
        case LOG_ERROR: prefix   = "[ERROR]: "; break;
        case LOG_WARNING: prefix = "[WARN] : "; break;
        case LOG_DEBUG: prefix   = "[DEBUG]: "; break;
        default: break;
    }
    std::string str = prefix + text;
    char buffer[1024];
    vsprintf(buffer, str.c_str(), args);
    printf("%s\n", buffer);
    Tools::console->AddLog(buffer);
}

int main(int argc, char *argv[])
{
    Engine* engine = new Engine();

    Tools::console->AddLog("Welcolme to %s", engine->GetEngineName());
    SetTraceLogCallback(raylibLog);

    const int windowWidth = GAME_SCREEN_W * 3;
    const int windowHeight = GAME_SCREEN_H * 3;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(windowWidth, windowHeight, engine->GetEngineName());
	SetWindowMinSize(GAME_SCREEN_W, GAME_SCREEN_H);
	SetTargetFPS(GAME_FPS);

    rlImGuiSetup(true);

    InitAudioDevice();
    HideCursor();
    SetExitKey(KEY_NULL);

    engine->Init();

    GameState currentState = Off;
    bool showImgui = false;
    const char * pauseMessage = "Paused, press ESC again to exit.";
    int pauseMessageSize = MeasureTextEx(Tools::GetFont(),"Paused, press ESC again to exit.", 8,0).x * 0.5f;

    engine->bios->LoadBoot();
    
    // Game Loop
    while (!(engine->bios->ShouldClose || WindowShouldClose()))
    {
        engine->UpdateFileWatcher();

        // Engine keys
        if(IsKeyReleased(KEY_F1))
        {
            showImgui = !showImgui;
        }

        if(IsKeyReleased(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyReleased(KEY_ENTER)))
        {
		    engine->postProcessing->FullScreen();
	    }

        if(IsWindowResized() && !showImgui) 
        {
            engine->postProcessing->UpdateGameScreenRects();
        }

        engine->postProcessing->uTime = GetTime();
        
        //Interpreter
        if (IsKeyReleased(KEY_F5) || engine->bios->ShouldRun)
        { 
            engine->bios->ShouldRun = false;

            if (currentState == Running)
            {
                engine->basicIntepreter->end();
            }

            if (engine->basicIntepreter->OpenBas(engine->bios->GetFile().c_str()) == MB_FUNC_OK)
            {
                engine->postProcessing->UpdateGameScreenRects();
                engine->basicIntepreter->Run();
                engine->basicIntepreter->init();
                currentState = Running;
            }
            else
            {
                Tools::console->AddLog("%s not found.\n", engine->bios->GetFile().c_str());
            }
        }
        if (IsKeyReleased(KEY_ESCAPE))
        {
            switch (currentState)
            {
                case Running:
                    currentState = Paused;
                    break;
                case Paused:
                    engine->basicIntepreter->end();
                    currentState = Off;
                    engine->basicIntepreter->CloseBas();
                    break;
                default:
                    break;
            }
        }

        if (currentState == Paused)
        {
            int anyKey = GetKeyPressed();
            if (anyKey != 0 && anyKey != 256) currentState = Running; // key 256 is Escape key
        }

        // Update
        engine->basicIntepreter->UpdateAudio();

        if (currentState == Running)
        {
            engine->basicIntepreter->tick();
        }
        
        engine->DropFileUpdate();

        // Draw
        BeginDrawing();

            //Draw game to texture.
            BeginTextureMode(engine->postProcessing->mainRender);
                switch (currentState)
                {
                    case Off:
                        if (!showImgui)
                        {
                            engine->bios->Update();
                        }
                        break;
                    case Running:
                        engine->basicIntepreter->draw();
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
                engine->postProcessing->RenderMain();
                engine->postProcessing->RenderBlur();
            EndBlendMode();
   
            // Final Draw
            ClearBackground(BLACK);
            engine->postProcessing->RenderFinal();

            // Engine over draw
            if(showImgui) 
            {
                rlImGuiBegin();
                bool open = true;
                ImGui::ShowDemoWindow(&open);

                engine->editor->DrawImGui();
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

    delete engine;

    return 0;
}
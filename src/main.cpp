#include "raylib.h"
#include "engine.h"

#include "wren_manager.h"
#include "mb_manager.h"
#include "postProcessing.h"
#include "bios.h"
#include "editor.h"
#include "sprite_manager.h"
#include "splash.h"

#include <thread> // Para std::this_thread::sleep_for
#include <chrono> // Para std::chrono::seconds

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

    Tools::console->AddLog("Welcome to %s", engine->GetEngineName());
    SetTraceLogCallback(raylibLog);

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(GAME_SCREEN_W, GAME_SCREEN_H, engine->GetEngineName());
	SetWindowMinSize(GAME_SCREEN_W, GAME_SCREEN_H);
	SetTargetFPS(GAME_FPS);

    HideCursor();
    SetExitKey(KEY_NULL);

    Texture splash;
    splash = Tools::TextureFromCode(SPLASH_FORMAT, SPLASH_HEIGHT, SPLASH_WIDTH, SPLASH_DATA, 1); 
    SetTextureFilter(splash, TEXTURE_FILTER_POINT);

    BeginDrawing();
        DrawTexture(splash, 0, 0, WHITE);
    EndDrawing();
    rlImGuiSetup(true);
    InitAudioDevice();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    engine->Init();

    bool showImgui = engine->editor->Enabled;
    engine->bios->LoadBoot();

    
    UnloadTexture(splash);
    // Game Loop
    while (!(engine->bios->ShouldClose || WindowShouldClose()))
    {
        //engine->UpdateFileWatcher();

        if (!engine->editor->Enabled) 
        {
            showImgui = false;
            engine->isInImGui = false;
        }
        // Engine keys
        if((IsKeyReleased(KEY_F1) || engine->bios->ShouldRecenter) && engine->editor->Enabled)
        {
            engine->bios->ShouldRecenter = false;
            showImgui = !showImgui;
            engine->isInImGui = showImgui;
            if (!showImgui)
            {
                engine->postProcessing->UpdateGameScreenRects();
                engine->postProcessing->UpdateWindowSize();
            }
            else
            {
                engine->postProcessing->RestoreWindow();
            }
        }

        if(IsKeyReleased(KEY_F2))
        {
            engine->postProcessing->UpdateWindowSize();
        }

        if(IsKeyReleased(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyReleased(KEY_ENTER)))
        {
		    engine->postProcessing->FullScreen();
	    }

        if(IsWindowResized()) 
        {
           engine->postProcessing->UpdateGameScreenRects();
        }

        engine->editor->window_position_x = (int)GetWindowPosition().x;
        engine->editor->window_position_y = (int)GetWindowPosition().y;

        engine->postProcessing->uTime = GetTime();
        
        //Interpreter
        if (IsKeyReleased(KEY_F5) || engine->bios->ShouldRun)
        { 
            engine->editor->codeEditor.RegisterCustomVars();
            engine->bios->ShouldRun = false;
            engine->editor->PlayerState = Running;

            if (engine->currentState == Running)
            {
                engine->basicIntepreter->close();
                engine->basicIntepreter->Reset();
                engine->audioManager->StopAll();
            }

            if (engine->basicIntepreter->OpenBas(engine->bios->CurrentProject.programFile.c_str()) == MB_FUNC_OK)
            {
                engine->audioManager->StopAll();
                engine->basicIntepreter->Run();
                engine->basicIntepreter->init();
                engine->currentState = Running;
                engine->editor->PlayerState = Running;
            }
            else
            {
                Tools::console->AddLog("%s not found.\n", engine->bios->CurrentProject.programFile.c_str());
            }
        }
        if (IsKeyReleased(KEY_ESCAPE))
        {
            switch (engine->currentState)
            {
                case Running:
                    engine->currentState = Paused;
                    engine->editor->PlayerState = Paused;
                    break;
                case Paused:
                    engine->basicIntepreter->close();
                    engine->currentState = Off;
                    engine->editor->PlayerState = Off;
                    engine->basicIntepreter->CloseBas();
                    engine->audioManager->StopAll();
                    break;
                default:
                    break;
            }
        }

        if (engine->currentState == Paused)
        {
            int anyKey = GetKeyPressed();
            if (anyKey != 0 && anyKey != 256) // key 256 is Escape key
            {
                engine->currentState = Running; 
                engine->editor->PlayerState = Running;
            }
            engine->basicIntepreter->pause();
        }

        // Update
        engine->basicIntepreter->UpdateAudio();
        engine->spriteManager->UpdateSpritesFromMemroy();

        if (engine->currentState == Running)
        {
            engine->basicIntepreter->UpdateVars();
            if (engine->editor->PlayerState == Running)
            {
                engine->basicIntepreter->tick();
            }
            if (engine->editor->DoStep)
            {
                engine->basicIntepreter->tick();
            }
        }
        
        engine->DropFileUpdate();

        // Draw
        BeginDrawing();

            //Draw game to texture.
            BeginTextureMode(engine->postProcessing->mainRender);
                switch (engine->currentState)
                {
                    case Off:
                        if (showImgui)
                        {
                            if (engine->editor->ScreenWindowHasFocus)
                            {
                                engine->bios->Update();
                            }
                        }
                        else
                        {
                            engine->bios->Update();
                        }
                        break;
                    case Running:
                        if (engine->editor->PlayerState == Running)
                        {
                            engine->basicIntepreter->draw();
                        }
                        if (engine->editor->DoStep)
                        {
                            engine->basicIntepreter->draw();
                            engine->editor->DoStep = false;
                        }
                        break;
                    case Paused:
                        engine->basicIntepreter->pause();
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

            if(showImgui)
            {
                BeginTextureMode(engine->postProcessing->editorRender);
                    engine->postProcessing->RenderFinal(true);
                EndTextureMode();
            }
            else
            {
                engine->postProcessing->RenderFinal(false);
            }

            // Engine over draw
            if(showImgui) 
            {
                rlImGuiBegin();
                engine->editor->Draw();
                rlImGuiEnd();
                engine->editor->ScreenWindowHasFocus && engine->editor->MouseInsideScreenWindow ? HideCursor() : ShowCursor();
            }
            else
            {
                HideCursor();
                engine->VirtualMouseX = Tools::GetVirtualMouse(true);
                engine->VirtualMouseY = Tools::GetVirtualMouse(false);
            }
    
        EndDrawing();
    }

    delete engine;
    
    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
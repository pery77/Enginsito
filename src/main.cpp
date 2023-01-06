#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "ini_manager.h"
#include "my_basic.h"

#include "tools.h"
#include "mb_manager.h"

RenderTexture2D mainRender;
RenderTexture2D bufferTexture;

Shader blurShader;
Shader crtShader;

Texture test;

int main(int argc, char *argv[])
{
    iniReader* config = new iniReader();
    Tools* tools = new Tools();
    MBManager* basic = new MBManager(tools);

    const int windowWidth = tools->GameScreenWidth * config->size;
    const int windowHeight = tools->GameScreenHeight * config->size;

	//Set Vsync and make the window resizeable
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

	//Create window
	InitWindow(windowWidth, windowHeight, "peryEngine");
	SetWindowMinSize(tools->GameScreenWidth, tools->GameScreenHeight);
	SetTargetFPS(60);

    //Create main texture and disable texture filter.
	mainRender = LoadRenderTexture(tools->GameScreenWidth, tools->GameScreenHeight);
	SetTextureFilter(mainRender.texture, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(mainRender.texture,TEXTURE_WRAP_MIRROR_REPEAT );

    bufferTexture = LoadRenderTexture(tools->GameScreenWidth * 8 , tools->GameScreenHeight * 8);

	SetTextureFilter(bufferTexture.texture, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(bufferTexture.texture,TEXTURE_WRAP_MIRROR_REPEAT );

    //Blur shader
    blurShader = LoadShader(0, "assets/blur.fs");
    int pass = GetShaderLocation(blurShader, "pass");
    int resolutionLoc = GetShaderLocation(blurShader, "resolution");
    int offsetLoc = GetShaderLocation(blurShader, "offset");

    //CRT shader
    crtShader = LoadShader(0, "assets/peryCRTDeluxe.fs");
    int blurTextureLoc = GetShaderLocation(crtShader, "blurTexture");
    int resolutionCRTLoc = GetShaderLocation(crtShader, "resolution");
    int uTimeLoc = GetShaderLocation(crtShader, "uTime");
    int testLoc = GetShaderLocation(crtShader, "test");
    SetShaderValueTexture(crtShader, blurTextureLoc, bufferTexture.texture);

    tools->UpdateGameScreenRects();
    bool running = false;
    bool showFps = false;

    float uTime;

    test = LoadTexture("assets/test.png");
    float t;

    // Game Loop
    while (!WindowShouldClose())
    {
        // Engine keys
        if(IsKeyReleased(KEY_F1)){
            showFps = !showFps;
        }

        if(IsKeyReleased(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyReleased(KEY_ENTER))){
		    tools->FullScreen();
	    }

        if(IsWindowResized()) 
        {
            tools->UpdateGameScreenRects();
        }
        uTime = GetTime();
        
        //Interpreter
        if (IsKeyReleased(KEY_F5)){ 
            if (running){
                basic->end();
            }
            
            basic->OpenBas(); 

            HideCursor();
            basic->init();
            running = true;
        }
        if (IsKeyReleased(KEY_F6)){ 
            if (running){
                ShowCursor();
                basic->end();
                running = false;
                basic->CloseBas();
            }
        }

        // Update
        if (running){
            basic->tick();
        }

        // Draw
        BeginDrawing();

            //Draw game to texture.
            BeginTextureMode(mainRender);
                if (!running){
                    ClearBackground(GRAY);
                    DrawTexture(test, 0, 0, WHITE);
                    DrawText("Press F5",0,0,8,RED);
                }else{
                    basic->draw();
                }
            EndTextureMode();

            // Main draw
            BeginBlendMode(0); // 0 Alpha (blur) 1 Additive (glow)
                // Copy game texture to buffer texutre
                BeginTextureMode(bufferTexture);
                    ClearBackground(BLACK);
                    DrawTexturePro(mainRender.texture, tools->gameRect, 
                                    (Rectangle){0,0,bufferTexture.texture.width, bufferTexture.texture.height},
                                    { 0, 0 }, 0.0f, WHITE); 
                EndTextureMode();

                // Start Blur
                BeginShaderMode(blurShader);
                    SetShaderValue(blurShader, resolutionLoc, &tools->resolution, SHADER_UNIFORM_VEC2);
                    for (auto blur : tools->blurPasses)
                    {
                        SetShaderValue(blurShader, pass, &blur.passType, SHADER_UNIFORM_INT);
                        SetShaderValue(blurShader, offsetLoc, &blur.offset, SHADER_UNIFORM_FLOAT);
                        BeginTextureMode(bufferTexture);
                            DrawTexturePro(bufferTexture.texture, (Rectangle){0,0,bufferTexture.texture.width, -bufferTexture.texture.height},
                                            (Rectangle){0,0,bufferTexture.texture.width, bufferTexture.texture.height}, 
                                            { 0, 0 }, 0.0f, WHITE);   
                        EndTextureMode();
                    }
                EndShaderMode();

            EndBlendMode();
   
            // Final Draw
            ClearBackground(BLACK);
            BeginShaderMode(crtShader);
                SetShaderValue(crtShader, resolutionCRTLoc, &tools->resolution, SHADER_UNIFORM_VEC2);
                SetShaderValue(crtShader, uTimeLoc, &uTime, SHADER_UNIFORM_FLOAT);
                SetShaderValue(crtShader, testLoc, &t, SHADER_UNIFORM_FLOAT);
                SetShaderValueTexture(crtShader, blurTextureLoc, bufferTexture.texture);
                DrawTexturePro(mainRender.texture, tools->gameRect, tools->gameScaledRect,
                                { 0, 0 }, 0.0f, WHITE); 
            EndShaderMode();

            // Engine over draw
            if(showFps){
                DrawFPS(0, 0);
                t = GuiSlider((Rectangle){0,16,300,30},"",TextFormat("%f",t),t,0,4);
            }

        EndDrawing();
    }

    CloseWindow();

    basic->CloseBas();

    delete config;
    delete basic;

    return 0;
}
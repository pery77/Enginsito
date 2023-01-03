#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "ini_reader.h"
#include "my_basic.h"

#include "tools.h"
#include "mb_manager.h"

RenderTexture2D mainRender;
RenderTexture2D bufferTexture;

Shader blurShader;

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

    //bufferTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    bufferTexture = LoadRenderTexture(320, 200);
	SetTextureFilter(bufferTexture.texture, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(bufferTexture.texture,TEXTURE_WRAP_MIRROR_REPEAT );

    blurShader = LoadShader(0, "assets/blur.fs");
    int pass = GetShaderLocation(blurShader, "pass");
    int offset = GetShaderLocation(blurShader, "offset");


    tools->UpdateGameScreenRects();

    bool running = false;
    bool showFps = false;

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

        if(IsWindowResized()) tools->UpdateGameScreenRects();

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
            ClearBackground(BLACK);
            DrawText("Press F5",0,0,20,RED);
        }else{
            basic->draw();
        }

        EndTextureMode();

        // Main draw
        BeginBlendMode(0);

        //BeginShaderMode(blurShader);
bufferTexture.texture.width = tools->gameScaledRect.width;
bufferTexture.texture.height = tools->gameScaledRect.height;
        BeginTextureMode(bufferTexture);
            ClearBackground(BLACK);
			DrawTexturePro(mainRender.texture, tools->gameRect, tools->gameScaledRect,
            { 0, 0 }, 0.0f, WHITE); 
        EndTextureMode();
/*
        int p = 0;
        float o = 0;

            SetShaderValue(blurShader, pass, &p, SHADER_UNIFORM_INT);
  for (int i = 1; i < 4; ++i)
  {
        BeginTextureMode(bufferTexture);
            o = GetMouseY()/(50.0*i);
            SetShaderValue(blurShader, offset, &o, SHADER_UNIFORM_FLOAT);
            DrawTexturePro(bufferTexture.texture, (Rectangle){0,0,GetScreenWidth(), -GetScreenHeight()},
            (Rectangle){0,0,GetScreenWidth(), GetScreenHeight()}, { 0, 0 }, 0.0f, WHITE); 
        EndTextureMode();
  }

p = 1;
            SetShaderValue(blurShader, pass, &p, SHADER_UNIFORM_INT);
  for (int i = 1; i < 3; ++i)
  {
        BeginTextureMode(bufferTexture);
            o = GetMouseX()/(50.0*i);
            SetShaderValue(blurShader, offset, &o, SHADER_UNIFORM_FLOAT);
            DrawTexturePro(bufferTexture.texture, (Rectangle){0,0,GetScreenWidth(), -GetScreenHeight()},
            (Rectangle){0,0,GetScreenWidth(), GetScreenHeight()}, { 0, 0 }, 0.0f, WHITE); 
        EndTextureMode();
  }
        EndShaderMode();

        EndBlendMode();
     */       
        //ClearBackground(BLACK);
        // Engine draw
        //Final Draw

        DrawTexturePro(bufferTexture.texture, (Rectangle){0,0,bufferTexture.texture.width, -bufferTexture.texture.height},
        tools->gameScaledRect, { 0, 0 }, 0.0f, WHITE);              

        float screenScale = min((float)GetScreenWidth()/(float)bufferTexture.texture.width,(float)GetScreenHeight()/(float)bufferTexture.texture.height);
        GuiLabel((Rectangle){0,0,100,10},TextFormat("sc %f",tools->screenScale));
        GuiLabel((Rectangle){0,20,100,10},TextFormat("x %f",tools->gameScaledRect.x));
        GuiLabel((Rectangle){0,40,100,10},TextFormat("y %f",tools->gameScaledRect.y));
        GuiLabel((Rectangle){0,60,100,10},TextFormat("w %f",tools->gameScaledRect.width));
        GuiLabel((Rectangle){0,80,100,10},TextFormat("h %f",tools->gameScaledRect.height));
        GuiLabel((Rectangle){0,100,100,10},TextFormat("sw %i",GetScreenWidth()));
        GuiLabel((Rectangle){0,110,100,10},TextFormat("sh %i",GetScreenHeight()));


        if(showFps){
            DrawFPS(0, 0);
        }

        //if (GuiButton(Rectangle{0,10,100,20},  "FullScreen")) { ToggleFullscreen(); }

        EndDrawing();
    }

    CloseWindow();

    basic->CloseBas();

    delete config;
    delete basic;

    return 0;
}



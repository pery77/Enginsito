#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "ini_reader.h"
#include "my_basic.h"

#include "tools.h"
#include "mb_manager.h"

RenderTexture2D mainRender;



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

    tools->UpdateGameScreenRects();

    bool running = false;
    bool showFps = false;

    // Game Loop
    while (!WindowShouldClose())
    {
        // Engine keys
        if(IsKeyReleased(KEY_F1)) {
            showFps = !showFps;
        }

        if(IsKeyReleased(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyReleased(KEY_ENTER)))	    {
		    tools->FullScreen();
	    }

        if(IsWindowResized()) tools->UpdateGameScreenRects();

        //Interpreter
        if (IsKeyReleased(KEY_F5))        { 
            if (running)            {
                basic->end();
                running = false;
            }
            basic->OpenBas(); 
            if (!running)            {
                basic->init();
                running = true;
            }

        }
        if (IsKeyReleased(KEY_F6)){ 
            if (running){
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

        if (!running)
        {
            ClearBackground(BLACK);
            DrawText("Press F5",0,0,20,RED);
        }

        if (running)
        {
            basic->draw();
        }

        EndTextureMode();

        // Main draw

        ClearBackground(BLACK);
		//Blend texture for postprocess effect.
		//BeginBlendMode(1);
        
		//Main Draw (draw game target texture in screen)
        //BeginShaderMode(crtShader);
			DrawTexturePro(mainRender.texture, tools->gameRect, tools->gameScaledRect, { 0, 0 }, 0.0f, WHITE);              
       // EndShaderMode();

		
		//EndBlendMode();

        // Engine draw

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



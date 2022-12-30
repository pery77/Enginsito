#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <sstream>
#include <assert.h>

#include "ini_reader.h"

#include "my_basic.h"
#include "tools.h"
#include "mb_manager.h"

RenderTexture2D mainRender;

int main(int argc, char *argv[])
{
    iniReader* config = new iniReader();
    mbManager* basic = new mbManager();
    Tools* tools = new Tools();

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

    // Game Loop
    while (!WindowShouldClose())
    {

        if(IsKeyReleased(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyReleased(KEY_ENTER)))
	    {
		    tools->UpdateGameScreenRects();
	    }

        // Update
        if (running)
        {
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

        if (IsKeyReleased(KEY_F5)) 
        { 
            if (running)
            {
                basic->end();
                running = false;
            }
            basic->OpenBas(); 
            if (!running)
            {
                basic->init();
                running = true;
            }

        }
        if (IsKeyReleased(KEY_F6)) 
        { 
            if (running)
            {
                basic->end();
                running = false;
                basic->CloseBas();
            }
        }

        EndTextureMode();

        ClearBackground(BLACK);
		//Blend texture for postprocess effect.
		//BeginBlendMode(1);
        
		//Main Draw (draw game target texture in screen)
        //BeginShaderMode(crtShader);
			DrawTexturePro(mainRender.texture, tools->gameRect, tools->gameScaledRect, { 0, 0 }, 0.0f, WHITE);              
       // EndShaderMode();

		
		//EndBlendMode();

        DrawFPS(0, 15);
        EndDrawing();
    }

    basic->CloseBas();
    CloseWindow();   

    delete config;

    return 0;
}



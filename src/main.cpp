#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <sstream>
#include <assert.h>

#include "ini_reader.h"

#include "my_basic.h"
#include "tools.h"
#include "mb_manager.h"

int main(int argc, char *argv[])
{
    iniReader* config = new iniReader();
    mbManager* basic = new mbManager();

    const int screenWidth = 640;
    const int screenHeight = 480;

    InitWindow(screenWidth, screenHeight, "peryEngine");
    SetTargetFPS(60);

    bool running = false;

    // Game Loop
    while (!WindowShouldClose())
    {
        // Update
        if (running)
        {
            basic->tick();
        }

        // Draw
        BeginDrawing();
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

        DrawFPS(550, 15);
        EndDrawing();
    }

    basic->CloseBas();
    CloseWindow();   

    delete config;

    return 0;
}



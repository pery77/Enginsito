#include "raylib.h"

int main()
{
    SetTargetFPS(60);
    InitWindow(320,200,"Enginsito");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }
    
}
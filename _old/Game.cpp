#include "Game.h"

Font font;
Texture test;

void Game::Init()
{
    font = LoadFont("assets/pixantiqua.fnt");
    test = LoadTexture("assets/test.png");
}

void Game::Tick()
{
}

void Game::Draw()
{
    ClearBackground({40,20,15,255});
    DrawTexture(test, GameScreenWidth/2-160, GameScreenHeight/2-100, WHITE);
    
	DrawRectangleLines(0,0,GameScreenWidth,GameScreenHeight,RED);

	DrawText(TextFormat("Default Mouse: [%i , %i]", (int)GetMousePosition().x, (int)GetMousePosition().y), 0, 0, 8, GREEN);
    DrawText(TextFormat("Virtual Mouse: [%i , %i]", GetVirtualMousePosition().x, GetVirtualMousePosition().y), 0, 10, 8, YELLOW);
    DrawTextEx(font, "pRUB Proanbde 3242", {20,60}, 8, 1, WHITE);
	DrawPixel(GetVirtualMousePosition().x, GetVirtualMousePosition().y,WHITE);
    DrawCircle(210,100,10.0f,GREEN);
}

void Game::OverDraw()
{
    Engine::OverDraw();
}
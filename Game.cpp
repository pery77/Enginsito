#include "Game.h"

Font font;

void Game::Init()
{
    font= LoadFont("pixantiqua.png");
}

void Game::Tick()
{

}

void Game::Draw()
{
    ClearBackground({40,20,15,255});
	DrawRectangleLines(0,0,GameScreenWidth,GameScreenHeight,RED);

	DrawText(TextFormat("Default Mouse: [%i , %i]", (int)GetMousePosition().x, (int)GetMousePosition().y), 0, 0, 8, GREEN);
    DrawText(TextFormat("Virtual Mouse: [%i , %i]", GetVirtualMousePosition().x, GetVirtualMousePosition().y), 0, 10, 8, YELLOW);
    DrawTextEx(font, "pRUB Proanbde 3242", {20,60}, font.baseSize, 1, WHITE);
	DrawPixel(GetVirtualMousePosition().x, GetVirtualMousePosition().y,WHITE);
    DrawCircle(100,100,10.0f,GREEN);

}

void Game::OverDraw()
{

}
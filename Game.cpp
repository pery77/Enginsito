#include "Game.h"

float minabo;

void Game::Init()
{
    minabo = 16.0f;
}

void Game::Tick()
{
  if(IsKeyReleased(KEY_A)) minabo++;
}

void Game::Draw()
{
    ClearBackground({31,20,15,255});
	DrawRectangleLines(0,0,GameScreenWidth,GameScreenHeight,RED);

	DrawText(TextFormat("Default Mouse: [%i , %i]", (int)GetMousePosition().x, (int)GetMousePosition().y), 0, 0, 8, GREEN);
    DrawText(TextFormat("Virtual Mouse: [%i , %i]", GetVirtualMousePosition().x, GetVirtualMousePosition().y), 0, 10, 8, YELLOW);
   
	DrawPixel(GetVirtualMousePosition().x, GetVirtualMousePosition().y,WHITE);
    DrawCircle(100,100,10.0f,GREEN);
}

void Game::OverDraw()
{

}
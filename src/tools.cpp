#include "tools.h"

Color palette[] = {
    {0,0,0,255},{128,0,0,255},{0,128,0,255},{128,128,0,255},{0,0,128,255},{128,0,128,255},
    {0,128,128,255},{192,192,192,255},{192,220,192,255},{166,202,240,255},{42,63,170,255},
    {42,63,255,255},{42,95,0,255},{42,95,85,255},{42,95,170,255},{42,95,255,255},{42,127,0,255},
    {42,127,85,255},{42,127,170,255},{42,127,255,255},{42,159,0,255},{42,159,85,255},
    {42,159,170,255},{42,159,255,255},{42,191,0,255},{42,191,85,255},{42,191,170,255},
    {42,191,255,255},{42,223,0,255},{42,223,85,255},{42,223,170,255},{42,223,255,255},
    {42,255,0,255},{42,255,85,255},{42,255,170,255},{42,255,255,255},{85,0,0,255},{85,0,85,255},
    {85,0,170,255},{85,0,255,255},{85,31,0,255},{85,31,85,255},{85,31,170,255},{85,31,255,255},
    {85,63,0,255},{85,63,85,255},{85,63,170,255},{85,63,255,255},{85,95,0,255},{85,95,85,255},
    {85,95,170,255},{85,95,255,255},{85,127,0,255},{85,127,85,255},{85,127,170,255},{85,127,255,255},
    {85,159,0,255},{85,159,85,255},{85,159,170,255},{85,159,255,255},{85,191,0,255},{85,191,85,255},
    {85,191,170,255},{85,191,255,255},{85,223,0,255},{85,223,85,255},{85,223,170,255},
    {85,223,255,255},{85,255,0,255},{85,255,85,255},{85,255,170,255},{85,255,255,255},
    {127,0,0,255},{127,0,85,255},{127,0,170,255},{127,0,255,255},{127,31,0,255},{127,31,85,255},
    {127,31,170,255},{127,31,255,255},{127,63,0,255},{127,63,85,255},{127,63,170,255},
    {127,63,255,255},{127,95,0,255},{127,95,85,255},{127,95,170,255},{127,95,255,255},
    {127,127,0,255},{127,127,85,255},{127,127,170,255},{127,127,255,255},{127,159,0,255},
    {127,159,85,255},{127,159,170,255},{127,159,255,255},{127,191,0,255},{127,191,85,255},
    {127,191,170,255},{127,191,255,255},{127,223,0,255},{127,223,85,255},{127,223,170,255},
    {127,223,255,255},{127,255,0,255},{127,255,85,255},{127,255,170,255},{127,255,255,255},
    {170,0,0,255},{170,0,85,255},{170,0,170,255},{170,0,255,255},{170,31,0,255},{170,31,85,255},
    {170,31,170,255},{170,31,255,255},{170,63,0,255},{170,63,85,255},{170,63,170,255},{170,63,255,255},
    {170,95,0,255},{170,95,85,255},{170,95,170,255},{170,95,255,255},{170,127,0,255},{170,127,85,255},
    {170,127,170,255},{170,127,255,255},{170,159,0,255},{170,159,85,255},{170,159,170,255},
    {170,159,255,255},{170,191,0,255},{170,191,85,255},{170,191,170,255},{170,191,255,255},
    {170,223,0,255},{170,223,85,255},{170,223,170,255},{170,223,255,255},{170,255,0,255},
    {170,255,85,255},{170,255,170,255},{170,255,255,255},{212,0,0,255},{212,0,85,255},{212,0,170,255},
    {212,0,255,255},{212,31,0,255},{212,31,85,255},{212,31,170,255},{212,31,255,255},{212,63,0,255},
    {212,63,85,255},{212,63,170,255},{212,63,255,255},{212,95,0,255},{212,95,85,255},{212,95,170,255},
    {212,95,255,255},{212,127,0,255},{212,127,85,255},{212,127,170,255},{212,127,255,255},
    {212,159,0,255},{212,159,85,255},{212,159,170,255},{212,159,255,255},{212,191,0,255},
    {212,191,85,255},{212,191,170,255},{212,191,255,255},{212,223,0,255},{212,223,85,255},
    {212,223,170,255},{212,223,255,255},{212,255,0,255},{212,255,85,255},{212,255,170,255},
    {212,255,255,255},{255,0,85,255},{255,0,170,255},{255,31,0,255},{255,31,85,255},
    {255,31,170,255},{255,31,255,255},{255,63,0,255},{255,63,85,255},{255,63,170,255},
    {255,63,255,255},{255,95,0,255},{255,95,85,255},{255,95,170,255},{255,95,255,255},
    {255,127,0,255},{255,127,85,255},{255,127,170,255},{255,127,255,255},{255,159,0,255},
    {255,159,85,255},{255,159,170,255},{255,159,255,255},{255,191,0,255},{255,191,85,255},
    {255,191,170,255},{255,191,255,255},{255,223,0,255},{255,223,85,255},{255,223,170,255},
    {255,223,255,255},{255,255,85,255},{255,255,170,255},{204,204,255,255},{255,204,255,255},
    {51,255,255,255},{102,255,255,255},{153,255,255,255},{204,255,255,255},{0,127,0,255},
    {0,127,85,255},{0,127,170,255},{0,127,255,255},{0,159,0,255},{0,159,85,255},{0,159,170,255},
    {0,159,255,255},{0,191,0,255},{0,191,85,255},{0,191,170,255},{0,191,255,255},{0,223,0,255},
    {0,223,85,255},{0,223,170,255},{0,223,255,255},{0,255,85,255},{0,255,170,255},{42,0,0,255},
    {42,0,85,255},{42,0,170,255},{42,0,255,255},{42,31,0,255},{42,31,85,255},{42,31,170,255},
    {42,31,255,255},{42,63,0,255},{42,63,85,255},{255,251,240,255},{160,160,164,255},
    {128,128,128,255},{255,0,0,255},{0,255,0,255},{255,255,0,255},{0,0,255,255},{255,0,255,255},
    {0,255,255,255},{255,255,255,255}
};

Tools::Tools(){
}
Tools::~Tools(){
}

Color Tools::GetColor(int id){
    if (id < 0 ) id = 0;
    if (id > 255) id = 255;
    return palette[id];
}

int Tools::GetVirtualMouse(bool isXAxis){   
	float mouse = isXAxis ? GetMousePosition().x : GetMousePosition().y;
    float screen = isXAxis ? GetScreenWidth() : GetScreenHeight();
    float gamescreen = isXAxis ? GameScreenWidth : GameScreenHeight;
    float value = (mouse - (screen - (gamescreen*screenScale)) * 0.5f) / screenScale;
    return (int)Clamp(value, 0,(float)gamescreen);
}

void Tools::UpdateGameScreenRects(){
	screenScale = min((float)GetScreenWidth()/GameScreenWidth,(float)GetScreenHeight()/GameScreenHeight);
	gameRect = { 0, 0, (float)(GameScreenWidth), -(float)(GameScreenHeight)};
	gameScaledRect = {0, 0,	(float)GetScreenWidth(), (float)GetScreenHeight()};

	currentAspectRatio = (float)GetScreenWidth()/GetScreenHeight();

	if (currentAspectRatio != GameRatio) 
	{
		SetWindowSize((currentAspectRatio > GameRatio) ? GameScreenWidth * screenScale : GetScreenWidth(), 
					  (currentAspectRatio > GameRatio) ? GetScreenHeight() : GameScreenHeight * screenScale);
		UpdateGameScreenRects();
	}
}

void Tools::FullScreen(){
    if (!IsWindowFullscreen())
	{
		previusWindowsWidth = GetScreenWidth();
		previusWindowsHeight = GetScreenHeight();

		for(auto currentR : resolution16_10)
		{
			SetWindowSize(currentR.x, currentR.y);
			float currentRatio =(float)GetScreenWidth()/GetScreenHeight();
			if(currentRatio==GameRatio) break;
		}
		UpdateGameScreenRects();
		ToggleFullscreen();
	}
	else
	{
		ToggleFullscreen();
		SetWindowSize(previusWindowsWidth, previusWindowsHeight);
		UpdateGameScreenRects();
	}
}

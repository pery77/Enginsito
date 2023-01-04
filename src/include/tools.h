#pragma once
#include "../../libs/raylib/include/raylib.h"
#include "../../libs/raylib/include/raymath.h"

#define max(a, b) ((a)>(b)? (a) : (b))
#define min(a, b) ((a)<(b)? (a) : (b))

struct Vector2i {
	int x;
	int y;
};

struct BlurPass{
	int passType; //Pass 0 downScale, other upScale
	float offset;
};

class Tools{
    public:

        Tools();
        ~Tools();

    	//Screen constant values.
        const int GameScreenWidth {320};
        const int GameScreenHeight {200};
        const float GameRatio {(float)GameScreenWidth/GameScreenHeight};

        Color GetColor(int col);

        void UpdateGameScreenRects();
        void FullScreen();
        int GetVirtualMouse(bool isXAxis);
    
	    float screenScale = {};
	    float previusWindowsWidth = {};
	    float previusWindowsHeight = {};
	    float currentAspectRatio = {};
	    Rectangle gameRect;
	    Rectangle gameScaledRect;

		BlurPass blurPasses[5] = {{0,2},{0,3},{0,8},{1,2},{1,3}};


        Vector2i resolution16_10[5] = {
		{2560,1600},
		{1920,1200},
		{1680,1050},
		{1440,900},
		{1280,800}
		};
};
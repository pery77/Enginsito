#pragma once
#include "../../libs/raylib/include/raylib.h"
#include "../../libs/raylib/include/raymath.h"

#define max(a, b) ((a)>(b)? (a) : (b))
#define min(a, b) ((a)<(b)? (a) : (b))

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
        int GetVirtualMouse(bool isXAxis);
    
	    float screenScale = {};
	    float previusWindowsWidth = {};
	    float previusWindowsHeight = {};
	    float currentAspectRatio = {};
	    Rectangle gameRect;
	    Rectangle gameScaledRect;

        private:
};
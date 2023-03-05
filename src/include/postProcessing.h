#pragma once
#include "../../libs/raylib/include/raylib.h"
#include "tools.h"

struct BlurPass{
	int passType; //Pass 0 downScale, other upScale
	float offset;
};

enum CRTProperty{
    BlurPower,
    BlurFactor,
    Chromatic
};

class PostProcessing{

    public:
    PostProcessing();
    ~PostProcessing();

    Vector2i resolution16_10[6] = {
    {3840,2400}, 
    {2560,1600},
    {1920,1200},
    {1680,1050},
    {1440,900},
    {1280,800}
    };

    void RenderMain();
    void RenderBlur();
    void RenderFinal();

    bool enabled = true;

    RenderTexture2D mainRender;
    float uTime;
    float uTest;
    float uCurvature;

    float uBlurPower = 0.85;
    float uBlurFactor = 1.2;
    float uChromatic = 0.3;

    void ReloadShaders();

    void UpdateGameScreenRects();
    void FullScreen();

    void SetState(bool newState);

	float screenScale;
	float previusWindowsWidth;
	float previusWindowsHeight;
	float currentAspectRatio;

    void SetCRTFloat(CRTProperty location, float value);

    private:
    void setUpShaders();

    RenderTexture2D bufferTexture;
    Texture grilleTexture;
    Shader crtShader;
    Shader blurShader;

	BlurPass blurPasses[5] = {{0,2},{0,3},{0,8},{1,2},{1,3}};

    // Blur shader locations
    int pass;
    int resolutionLoc;
    int offsetLoc;

    // CRT shader locations
    int grilleLoc;
    int blurTextureLoc;
    int resolutionCRTLoc;
    int uTimeLoc;
    int curvatureLoc;

    int blurPowerLoc;
    int blurFactorLoc;
    int chromaticLoc;

    Rectangle gameRect = { 0, 0, (float)(320), -(float)(200)};
    Rectangle gameScaledRect { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
    Vector2 resolution = {(float)GetScreenWidth(), (float)GetScreenHeight()};
};
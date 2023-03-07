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
    Chromatic,
    Curvature,
    Vignetting,
    ScanLine
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

    float uBlurPower = 0.85;
    float uBlurFactor = 1.2;
    float uChromatic = 0.3;
    float uCurvature = 0.15;
    float uVignetteIntensity = 0.2;
    float uScanline = 0.5;

    void ReloadShaders();

    void UpdateGameScreenRects();
    void FullScreen();

    void SetState(bool newState);

	float screenScale;
	float previusWindowsWidth;
	float previusWindowsHeight;
	float currentAspectRatio;

    void SetCRTFloat(CRTProperty location, float value);
    void SetGrilleTexture(int newTextureId);

    private:
    void setUpShaders();

    RenderTexture2D bufferTexture;
    Texture grilleTextures[3];
    int currentGrilleTexture;
    Shader crtShader;
    Shader blurShader;

	BlurPass blurPasses[5] = {{0,2},{0,3},{0,8},{1,2},{1,3}};

    // Blur shader locations
    int pass;
    int resolutionLoc;
    int offsetLoc;

    // CRT shader locations
    // Internal
    int grilleLoc;
    int blurTextureLoc;
    int resolutionCRTLoc;
    int timeLoc;
    // User
    int blurPowerLoc;
    int blurFactorLoc;
    int chromaticLoc;
    int curvatureLoc;
    int vignetteIntensityLoc;
    int scanlineLoc;

    Rectangle gameRect = { 0, 0, (float)(320), -(float)(200)};
    Rectangle gameScaledRect { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
    Vector2 resolution = {(float)GetScreenWidth(), (float)GetScreenHeight()};
};
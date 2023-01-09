#pragma once
#include "../../libs/raylib/include/raylib.h"
#include "tools.h"

class PostProcessing{

    public:
    PostProcessing(Tools* toolsref);
    ~PostProcessing();

    void RenderMain();
    void RenderBlur();
    void RenderFinal();

    RenderTexture2D mainRender;
    float uTime;
    float uTest;
    float uCurvature;

    float uBlurPower = 0.5;
    float uBlurFactor = 1;

    void ReloadShaders();

    private:
    Tools* tools;
    void setUpShaders();

    RenderTexture2D bufferTexture;
    Texture grilleTexture;
    Shader crtShader;
    Shader blurShader;

    // Blur shader locations
    int pass;
    int resolutionLoc;
    int offsetLoc;

    // CRT shader locations
    int grilleLoc;
    int blurTextureLoc;
    int resolutionCRTLoc;
    int uTimeLoc;
    int testLoc;
    int curvatureLoc;

    int blurPowerLoc;
    int blurFactorLoc;

};
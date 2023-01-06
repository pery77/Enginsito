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

    float uBlurPower = 1;
    float uBlurFactor = 1;

    private:
    Tools* tools;

    RenderTexture2D bufferTexture;
    Shader crtShader;
    Shader blurShader;
    void setUpShaders();

    // Blur shader locations
    int pass;
    int resolutionLoc;
    int offsetLoc;

    // CRT shader locations
    int blurTextureLoc;
    int resolutionCRTLoc;
    int uTimeLoc;
    int testLoc;

    int blurPowerLoc;
    int blurFactorLoc;

};
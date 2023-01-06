#include "include/postProcessing.h"

PostProcessing::PostProcessing(Tools* toolsref){
    tools = toolsref;
    setUpShaders();
}
PostProcessing::~PostProcessing(){}

void PostProcessing::setUpShaders(){

	mainRender = LoadRenderTexture(tools->GameScreenWidth, tools->GameScreenHeight);
	SetTextureFilter(mainRender.texture, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(mainRender.texture,TEXTURE_WRAP_MIRROR_REPEAT );

    bufferTexture = LoadRenderTexture(tools->GameScreenWidth * 8 , tools->GameScreenHeight * 8);
	SetTextureFilter(bufferTexture.texture, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(bufferTexture.texture,TEXTURE_WRAP_MIRROR_REPEAT );

    //Blur shader
    blurShader = LoadShader(0, "assets/blur.fs");

    //CRT shader
    crtShader = LoadShader(0, "assets/peryCRTDeluxe.fs");
    SetShaderValueTexture(crtShader, blurTextureLoc, bufferTexture.texture);


    // Blur shader locations
    pass = GetShaderLocation(blurShader, "pass");
    resolutionLoc = GetShaderLocation(blurShader, "resolution");
    offsetLoc = GetShaderLocation(blurShader, "offset");

    // CRT shader locations
    blurTextureLoc = GetShaderLocation(crtShader, "blurTexture");
    resolutionCRTLoc = GetShaderLocation(crtShader, "resolution");
    uTimeLoc = GetShaderLocation(crtShader, "uTime");
    testLoc = GetShaderLocation(crtShader, "test");

    blurPowerLoc = GetShaderLocation(crtShader, "uBlurPower");
    blurFactorLoc = GetShaderLocation(crtShader, "uBlurFactor");
}

void PostProcessing::RenderMain(){
    BeginTextureMode(bufferTexture);
    ClearBackground(BLACK);
        DrawTexturePro(mainRender.texture, tools->gameRect, 
                        (Rectangle){0,0,bufferTexture.texture.width, bufferTexture.texture.height},
                        { 0, 0 }, 0.0f, WHITE); 
    EndTextureMode();
}

void PostProcessing::RenderBlur(){

    BeginShaderMode(blurShader);
        SetShaderValue(blurShader, resolutionLoc, &tools->resolution, SHADER_UNIFORM_VEC2);
        for (auto blur : tools->blurPasses){
            SetShaderValue(blurShader, pass, &blur.passType, SHADER_UNIFORM_INT);
            SetShaderValue(blurShader, offsetLoc, &blur.offset, SHADER_UNIFORM_FLOAT);
            BeginTextureMode(bufferTexture);
                DrawTexturePro(bufferTexture.texture, (Rectangle){0,0,bufferTexture.texture.width, -bufferTexture.texture.height},
                                            (Rectangle){0,0,bufferTexture.texture.width, bufferTexture.texture.height}, 
                                            { 0, 0 }, 0.0f, WHITE);   
            EndTextureMode();
        }
    EndShaderMode();
}

void PostProcessing::RenderFinal(){
    BeginShaderMode(crtShader);
        SetShaderValue(crtShader, resolutionCRTLoc, &tools->resolution, SHADER_UNIFORM_VEC2);
        SetShaderValue(crtShader, uTimeLoc, &uTime, SHADER_UNIFORM_FLOAT);
        SetShaderValue(crtShader, testLoc, &uTest, SHADER_UNIFORM_FLOAT);
        
        SetShaderValue(crtShader, blurPowerLoc, &uBlurPower, SHADER_UNIFORM_FLOAT);
        SetShaderValue(crtShader, blurFactorLoc, &uBlurFactor, SHADER_UNIFORM_FLOAT);

        SetShaderValueTexture(crtShader, blurTextureLoc, bufferTexture.texture);
        DrawTexturePro(mainRender.texture, tools->gameRect, tools->gameScaledRect,
                                { 0, 0 }, 0.0f, WHITE); 
    EndShaderMode();
}
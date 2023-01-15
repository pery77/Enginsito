#include "postProcessing.h"

PostProcessing::PostProcessing(){
    setUpShaders();
}
PostProcessing::~PostProcessing(){}

void PostProcessing::setUpShaders(){

	mainRender = LoadRenderTexture(GAME_SCREEN_W, GAME_SCREEN_H);
	SetTextureFilter(mainRender.texture, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(mainRender.texture,TEXTURE_WRAP_MIRROR_REPEAT);

    bufferTexture = LoadRenderTexture(GAME_SCREEN_W * 8 , GAME_SCREEN_H * 8);
	SetTextureFilter(bufferTexture.texture, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(bufferTexture.texture,TEXTURE_WRAP_MIRROR_REPEAT);

    grilleTexture = LoadTexture("assets/grille.png");
    GenTextureMipmaps(&grilleTexture);
	SetTextureFilter(grilleTexture, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(grilleTexture,TEXTURE_WRAP_REPEAT);
    

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
    grilleLoc = GetShaderLocation(crtShader, "grilleTexture");
    blurTextureLoc = GetShaderLocation(crtShader, "blurTexture");
    resolutionCRTLoc = GetShaderLocation(crtShader, "resolution");
    uTimeLoc = GetShaderLocation(crtShader, "uTime");
    testLoc = GetShaderLocation(crtShader, "test");
    curvatureLoc = GetShaderLocation(crtShader, "curvatureDistance");

    blurPowerLoc = GetShaderLocation(crtShader, "uBlurPower");
    blurFactorLoc = GetShaderLocation(crtShader, "uBlurFactor");
}

void PostProcessing::RenderMain(){
    BeginTextureMode(bufferTexture);
    ClearBackground(BLACK);
        DrawTexturePro(mainRender.texture, gameRect, 
                        (Rectangle){0,0,bufferTexture.texture.width, bufferTexture.texture.height},
                        { 0, 0 }, 0.0f, WHITE); 
    EndTextureMode();
}

void PostProcessing::RenderBlur(){

    BeginShaderMode(blurShader);
        SetShaderValue(blurShader, resolutionLoc, &resolution, SHADER_UNIFORM_VEC2);
        for (auto blur : blurPasses){
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
        SetShaderValue(crtShader, resolutionCRTLoc, &resolution, SHADER_UNIFORM_VEC2);
        SetShaderValue(crtShader, uTimeLoc, &uTime, SHADER_UNIFORM_FLOAT);
        SetShaderValue(crtShader, testLoc, &uTest, SHADER_UNIFORM_FLOAT);
        SetShaderValue(crtShader, curvatureLoc, &uCurvature, SHADER_UNIFORM_FLOAT);
        
        SetShaderValue(crtShader, blurPowerLoc, &uBlurPower, SHADER_UNIFORM_FLOAT);
        SetShaderValue(crtShader, blurFactorLoc, &uBlurFactor, SHADER_UNIFORM_FLOAT);

        SetShaderValueTexture(crtShader, grilleLoc, grilleTexture);
        SetShaderValueTexture(crtShader, blurTextureLoc, bufferTexture.texture);
        DrawTexturePro(mainRender.texture, gameRect, gameScaledRect,
                                { 0, 0 }, 0.0f, WHITE); 
    EndShaderMode();
}

void PostProcessing::ReloadShaders(){

    UnloadRenderTexture(mainRender);
    UnloadRenderTexture(bufferTexture);
    UnloadTexture(grilleTexture);

    UnloadShader(blurShader);
    UnloadShader(crtShader);

    setUpShaders();
}


void PostProcessing::UpdateGameScreenRects(){

	screenScale = min((float)GetScreenWidth()/GAME_SCREEN_W,(float)GetScreenHeight()/GAME_SCREEN_H);
	gameRect = { 0, 0, (float)(GAME_SCREEN_W), -(float)(GAME_SCREEN_H)};
	gameScaledRect = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
	resolution = {(float)GetScreenWidth(), (float)GetScreenHeight()};

	currentAspectRatio = (float)GetScreenWidth()/GetScreenHeight();
    bool aspectRatioOk = Tools::CompareFloats(currentAspectRatio , GAME_RATIO, 0.05);

	if (!aspectRatioOk) {
		SetWindowSize((currentAspectRatio > GAME_RATIO) ? GAME_SCREEN_W * screenScale : GetScreenWidth(), 
					  (currentAspectRatio > GAME_RATIO) ? GetScreenHeight() : GAME_SCREEN_H * screenScale);
        UpdateGameScreenRects();
	}
}

void PostProcessing::FullScreen(){
    if (!IsWindowFullscreen())
	{
		previusWindowsWidth = GetScreenWidth();
		previusWindowsHeight = GetScreenHeight();

		for(auto currentR : resolution16_10)
		{
			SetWindowSize(currentR.x, currentR.y);
			float currentRatio =(float)GetScreenWidth()/GetScreenHeight();
			if (Tools::CompareFloats(currentRatio, GAME_RATIO, 0.05)) break;
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
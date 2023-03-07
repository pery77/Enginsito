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

    grilleTextures[0] = LoadTexture("assets/grille1.png");
    grilleTextures[1] = LoadTexture("assets/grille2.png");
    grilleTextures[2] = LoadTexture("assets/grille3.png");
    for (int gt = 0; gt<3; gt++){
        GenTextureMipmaps(&grilleTextures[gt]);
        SetTextureFilter(grilleTextures[gt], TEXTURE_FILTER_BILINEAR);
        SetTextureWrap(grilleTextures[gt],TEXTURE_WRAP_REPEAT);
    }
    
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
    timeLoc = GetShaderLocation(crtShader, "uTime");

    blurPowerLoc = GetShaderLocation(crtShader, "uBlurPower");
    blurFactorLoc = GetShaderLocation(crtShader, "uBlurFactor");
    chromaticLoc = GetShaderLocation(crtShader, "uChromatic");
    curvatureLoc = GetShaderLocation(crtShader, "uCurvature");
    vignetteIntensityLoc = GetShaderLocation(crtShader, "uVignetteIntensity");
    scanlineLoc = GetShaderLocation(crtShader, "uScanline");
}

void PostProcessing::RenderMain(){
    if (enabled)
        BeginTextureMode(bufferTexture);
    
    ClearBackground(BLACK);
        DrawTexturePro(mainRender.texture, gameRect, 
                        (Rectangle){0,0,bufferTexture.texture.width, bufferTexture.texture.height},
                        { 0, 0 }, 0.0f, WHITE); 
    if (enabled)
        EndTextureMode();
}

void PostProcessing::RenderBlur(){

    if (enabled){
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
}

void PostProcessing::RenderFinal(){
    if (enabled){

        BeginShaderMode(crtShader);
            SetShaderValueTexture(crtShader, grilleLoc, grilleTextures[currentGrilleTexture]);
            SetShaderValueTexture(crtShader, blurTextureLoc, bufferTexture.texture);
            SetShaderValue(crtShader, resolutionCRTLoc, &resolution, SHADER_UNIFORM_VEC2);
            SetShaderValue(crtShader, timeLoc, &uTime, SHADER_UNIFORM_FLOAT);
            
            SetShaderValue(crtShader, blurPowerLoc, &uBlurPower, SHADER_UNIFORM_FLOAT);
            SetShaderValue(crtShader, blurFactorLoc, &uBlurFactor, SHADER_UNIFORM_FLOAT);
            SetShaderValue(crtShader, chromaticLoc, &uChromatic, SHADER_UNIFORM_FLOAT);
            SetShaderValue(crtShader, curvatureLoc, &uCurvature, SHADER_UNIFORM_FLOAT);
            SetShaderValue(crtShader, vignetteIntensityLoc, &uVignetteIntensity, SHADER_UNIFORM_FLOAT);
            SetShaderValue(crtShader, scanlineLoc, &uScanline, SHADER_UNIFORM_FLOAT);

    }
        DrawTexturePro(mainRender.texture, gameRect, gameScaledRect,
                                { 0, 0 }, 0.0f, WHITE); 
    if(enabled)
        EndShaderMode();
}

void PostProcessing::ReloadShaders(){

    UnloadRenderTexture(mainRender);
    UnloadRenderTexture(bufferTexture);
    for (int gt = 0; gt<3; gt++){
        UnloadTexture(grilleTextures[gt]);
    }

    UnloadShader(blurShader);
    UnloadShader(crtShader);

    setUpShaders();
}

void PostProcessing::UpdateGameScreenRects(){

	screenScale = Tools::Min((float)GetScreenWidth()/(float)GAME_SCREEN_W,(float)GetScreenHeight()/(float)GAME_SCREEN_H);
	gameRect = { 0, 0, (float)(GAME_SCREEN_W), -(float)(GAME_SCREEN_H)};
	gameScaledRect = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
	resolution = {(float)GetScreenWidth(), (float)GetScreenHeight()};

	currentAspectRatio = (float)GetScreenWidth()/(float)GetScreenHeight();
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

void PostProcessing::SetState(bool newState){
    enabled = newState;
    SetTextureFilter(mainRender.texture, enabled ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
}

void PostProcessing::SetCRTFloat(CRTProperty property, float value){
    value *= 0.03921; // byte to float normalized 1/255
    switch (property)
    {
    case CRTProperty::BlurPower:
            uBlurPower = value;
        break;
    case CRTProperty::BlurFactor:
            uBlurFactor = value;
        break;
    case CRTProperty::Chromatic:
            uChromatic = value;
        break;
    case CRTProperty::Curvature:
            uCurvature = value;
        break;
    case CRTProperty::Vignetting:
            uVignetteIntensity = value;
        break;
    case CRTProperty::ScanLine:
            uScanline = value;
        break;    
    default:
        break;
    }
}

 void PostProcessing::SetGrilleTexture(int newTextureId){
    if (newTextureId<0) newTextureId = 0;
    if (newTextureId>2) newTextureId = 2;
    currentGrilleTexture = newTextureId;
 }
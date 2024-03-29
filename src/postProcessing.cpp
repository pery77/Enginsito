#include "postProcessing.h"
#include "engine.h"

Engine* postProEngineRef;

PostProcessing::PostProcessing(Engine* _engine)
{
    postProEngineRef = _engine;
    setUpShaders();
}
PostProcessing::~PostProcessing(){}

void PostProcessing::setUpShaders()
{
	editorRender = LoadRenderTexture(GAME_SCREEN_W * 8, GAME_SCREEN_H * 8);
	SetTextureFilter(editorRender.texture, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(editorRender.texture,TEXTURE_WRAP_REPEAT);

	mainRender = LoadRenderTexture(GAME_SCREEN_W, GAME_SCREEN_H);
	SetTextureFilter(mainRender.texture, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(mainRender.texture,TEXTURE_WRAP_MIRROR_REPEAT);

    bufferTexture = LoadRenderTexture(GAME_SCREEN_W * 8 , GAME_SCREEN_H * 8);
	SetTextureFilter(bufferTexture.texture, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(bufferTexture.texture,TEXTURE_WRAP_MIRROR_REPEAT);

    grilleTextures[0] = Tools::TextureFromCode(GRILLE1_FORMAT, GRILLE1_HEIGHT, GRILLE1_HEIGHT, GRILLE1_DATA, 3);
    grilleTextures[1] = Tools::TextureFromCode(GRILLE2_FORMAT, GRILLE2_HEIGHT, GRILLE2_HEIGHT, GRILLE2_DATA, 3);
    grilleTextures[2] = Tools::TextureFromCode(GRILLE3_FORMAT, GRILLE3_HEIGHT, GRILLE3_HEIGHT, GRILLE3_DATA, 3);

/* //Image to Code
    Image i1 = LoadImage("assets/grille1.png");
    Image i2 = LoadImage("assets/grille2.png");
    Image i3 = LoadImage("assets/grille3.png");

    ExportImageAsCode(i1, "grille1.h");
    ExportImageAsCode(i2, "grille2.h");
    ExportImageAsCode(i3, "grille3.h");

    UnloadImage(i1);
    UnloadImage(i2);
    UnloadImage(i3);
*/
    for (int gt = 0; gt<3; gt++){
        GenTextureMipmaps(&grilleTextures[gt]);
        SetTextureFilter(grilleTextures[gt], TEXTURE_FILTER_BILINEAR);
        SetTextureWrap(grilleTextures[gt],TEXTURE_WRAP_REPEAT);
    }
    
    //Blur shader
    //blurShader = LoadShader(0, "assets/blur.fs");
    blurShader = LoadShaderFromMemory(0, blurShaderCode);

    //CRT shader
    //crtShader = LoadShader(0, "assets/peryCRTDeluxe.fs");
    crtShader = LoadShaderFromMemory(0, crtShaderCode);
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
    verticalLineLoc = GetShaderLocation(crtShader, "uVerticalLine");
    grilleForceLoc = GetShaderLocation(crtShader, "uGrilleForce");
    noiseLoc = GetShaderLocation(crtShader, "uNoise");
    flikerLoc = GetShaderLocation(crtShader, "uFliker");
}

void PostProcessing::RenderMain()
{
    if (enabled)
        BeginTextureMode(bufferTexture);
            
        ClearBackground(BLACK);
        DrawTexturePro(mainRender.texture, gameRect, 
                            (Rectangle){0,0,bufferTexture.texture.width, bufferTexture.texture.height},
                            { 0, 0 }, 0.0f, WHITE);


    
     if (enabled)
        EndTextureMode();

    //BeginTextureMode(editorRender);
    //    DrawTexturePro(mainRender.texture, gameRect, 
    //            (Rectangle){0,0,bufferTexture.texture.width, bufferTexture.texture.height},
    //            { 0, 0 }, 0.0f, WHITE);
    //EndTextureMode();

}

void PostProcessing::RenderBlur()
{
    if (!enabled) return;
    
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

void PostProcessing::RenderFinal(bool isEditor)
{
    SetState(postProEngineRef->Peek(4091)!= 0);

    Rectangle screenRect = isEditor ? (Rectangle){0,0, editorRender.texture.width, editorRender.texture.height} : gameScaledRect;

    if (enabled)
    {
        uBlurPower         = GetCRTFloat(CRTProperty::BlurPower);
        uBlurFactor        = GetCRTFloat(CRTProperty::BlurFactor);
        uChromatic         = GetCRTFloat(CRTProperty::Chromatic);
        uCurvature         = GetCRTFloat(CRTProperty::Curvature);
        uVignetteIntensity = GetCRTFloat(CRTProperty::Vignetting);
        uScanline          = GetCRTFloat(CRTProperty::ScanLine);
        uVerticalLine      = GetCRTFloat(CRTProperty::VerticalLine);
        uGrilleForce       = GetCRTFloat(CRTProperty::GrilleForce);
        uNoise             = GetCRTFloat(CRTProperty::Noise);
        uFliker            = GetCRTFloat(CRTProperty::Fliker);

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
            SetShaderValue(crtShader, verticalLineLoc, &uVerticalLine, SHADER_UNIFORM_FLOAT);
            SetShaderValue(crtShader, grilleForceLoc, &uGrilleForce, SHADER_UNIFORM_FLOAT);
            SetShaderValue(crtShader, noiseLoc, &uNoise, SHADER_UNIFORM_FLOAT);
            SetShaderValue(crtShader, flikerLoc, &uFliker, SHADER_UNIFORM_FLOAT);

            DrawTexturePro(mainRender.texture, gameRect, screenRect, { 0, 0 }, 0.0f, WHITE);

        EndShaderMode();

    }
    else
    {
        DrawTexturePro(mainRender.texture, gameRect, screenRect, { 0, 0 }, 0.0f, WHITE); 
    }
}

void PostProcessing::ReloadShaders()
{
    UnloadRenderTexture(mainRender);
    UnloadRenderTexture(bufferTexture);
    for (int gt = 0; gt<3; gt++){
        UnloadTexture(grilleTextures[gt]);
    }

    UnloadShader(blurShader);
    UnloadShader(crtShader);

    setUpShaders();
}

void PostProcessing::UpdateGameScreenRects()
{
    resolution = (Vector2){(float)GetScreenWidth(), (float)GetScreenHeight()};
	currentAspectRatio = resolution.x/resolution.y;

    screenScale = (currentAspectRatio < GAME_RATIO) ? resolution.x/(float)GAME_SCREEN_W : resolution.y/(float)GAME_SCREEN_H;

	gameScaledRect = { (resolution.x - (GAME_SCREEN_W * screenScale)) / 2, (resolution.y - (GAME_SCREEN_H * screenScale)) / 2, 
                        GAME_SCREEN_W * screenScale, GAME_SCREEN_H * screenScale };
}

void PostProcessing::UpdateWindowSize()
{
    previusWindowsWidth  = GetScreenWidth();
	previusWindowsHeight = GetScreenHeight();

    previusWindowsX = GetWindowPosition().x;
    previusWindowsY = GetWindowPosition().y;

    int w = (currentAspectRatio > GAME_RATIO) ? GAME_SCREEN_W * screenScale : GetScreenWidth();
    int h = (currentAspectRatio > GAME_RATIO) ? GetScreenHeight(): GAME_SCREEN_H * screenScale;

	SetWindowSize(w,h);

    int monitor = GetCurrentMonitor();
    SetWindowPosition(GetMonitorWidth(monitor) / 2 - GetScreenWidth() / 2, GetMonitorHeight(monitor) / 2 - GetScreenHeight() / 2 );
}

void PostProcessing::RestoreWindow()
{
    SetWindowSize(previusWindowsWidth, previusWindowsHeight);
    SetWindowPosition(previusWindowsX, previusWindowsY);
}

void PostProcessing::FullScreen()
{
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

void PostProcessing::SetState(bool newState)
{
    if (newState == enabled)
        return;

    enabled = newState;
    postProEngineRef->Poke(4091, enabled ? 1 : 0);
    SetTextureFilter(mainRender.texture, enabled ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
    SetTextureFilter(editorRender.texture, enabled ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT);
}

void PostProcessing::SetCRTValue(CRTProperty property, uint8_t value)
{
    unsigned short dir = 4080;
    postProEngineRef->Poke(dir + property, value);
}
float PostProcessing::GetCRTFloat(CRTProperty property)
{
    unsigned short dir = 4080;
    return postProEngineRef->MainMemory[dir + property] * 0.003921f;
}

 void PostProcessing::SetGrilleTexture(int newTextureId)
 {
    if (newTextureId<0) newTextureId = 0;
    if (newTextureId>2) newTextureId = 2;
    postProEngineRef->Poke(4090, newTextureId);
    currentGrilleTexture = newTextureId;
 }



void PostProcessing::Refresh()
{
    bool ppState = postProEngineRef->Peek(4091);
    SetState(ppState);

    int blurPower    = postProEngineRef->Peek(4080);
    int blurFactor   = postProEngineRef->Peek(4081);
    int chromatic    = postProEngineRef->Peek(4082);
    int curvature    = postProEngineRef->Peek(4083);
    int vignetting   = postProEngineRef->Peek(4084);
    int scanLine     = postProEngineRef->Peek(4085);
    int verticalLine = postProEngineRef->Peek(4086);
    int grilleForce  = postProEngineRef->Peek(4087);
    int noise        = postProEngineRef->Peek(4088);
    int fliker       = postProEngineRef->Peek(4089);
    int grille       = postProEngineRef->Peek(4090);

    SetCRTValue(CRTProperty::BlurPower, blurPower);
    SetCRTValue(CRTProperty::BlurFactor, blurFactor);
    SetCRTValue(CRTProperty::Chromatic, chromatic);
    SetCRTValue(CRTProperty::Curvature, curvature);
    SetCRTValue(CRTProperty::Vignetting, vignetting);
    SetCRTValue(CRTProperty::ScanLine, scanLine);
    SetCRTValue(CRTProperty::VerticalLine, verticalLine);
    SetCRTValue(CRTProperty::GrilleForce, grilleForce);
    SetCRTValue(CRTProperty::Noise, noise);
    SetCRTValue(CRTProperty::Fliker, fliker);
    SetGrilleTexture(grille);
}
#include "engine.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

Engine::Engine()
{
	printf("Starting Enginsito\n");
}

Engine::~Engine()
{
	printf("Unload Enginsito\n");
	UnloadRenderTexture(mainRender);
}

//Start engine.
void Engine::Run()
{
	const int windowWidth = GameScreenWidth*4;
    const int windowHeight = GameScreenHeight*4;


	//Set Vsync and make the window resizeable
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

	//Create window
	InitWindow(windowWidth, windowHeight, "Enginsito");
	SetWindowMinSize(GameScreenWidth, GameScreenHeight);
	SetTargetFPS(60);

	//Create main texture and disable texture filter.
	mainRender = LoadRenderTexture(GameScreenWidth, GameScreenHeight);
	SetTextureFilter(mainRender.texture, TEXTURE_FILTER_POINT);
	
	shader = LoadShader(crtFileVs, crtFileFs);//LoadShaderCode to embebed.
	time = 0.0f;
	
	UpdateGameScreenRects();
	HideCursor();

	InitCRTShader();

	Init();
	MainLoop();
}

void Engine::MainLoop()
{    
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		time = (float)GetTime();

		UpdateCRTShader();

		if(IsWindowResized()) UpdateGameScreenRects();
		UpdateMouse();
		ProcessInput();

		Tick();

		RenderFrame();
	}		

	CloseWindow();					// Close window and OpenGL context
}

void Engine::ProcessInput()
{
	if(IsKeyReleased(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyReleased(KEY_ENTER)))
	{
		FullScreen();
	}

	if(IsKeyReleased(KEY_F10))
	{
		shader = LoadShader(crtFileVs, crtFileFs);
	}
}

//Draw loop.
void Engine::RenderFrame()
{
	BeginDrawing();

		//Draw game to texture.
		BeginTextureMode(mainRender);
			Draw();
		EndTextureMode();

		ClearBackground(BLACK);
/*
		//Blend texture for postprocess effect.
		BeginBlendMode(1);
			//Draw main texture scaled to screen.
			DrawTexturePro(mainRender.texture, gameRect, gameScaledRect, { 0, 0 }, 0.0f, WHITE);

		EndBlendMode();
*/
		//Main Draw (draw game target texture in screen)
        BeginShaderMode(shader);
			DrawTexturePro(mainRender.texture, gameRect, gameScaledRect, { 0, 0 }, 0.0f, WHITE);              
        EndShaderMode();

		OverDraw();

	EndDrawing();
}

//Tools
Vector2 Engine::ClampValue(Vector2 value, Vector2 min, Vector2 max)
{
    Vector2 result = value;
    result.x = (result.x > max.x)? max.x : result.x;
    result.x = (result.x < min.x)? min.x : result.x;
    result.y = (result.y > max.y)? max.y : result.y;
    result.y = (result.y < min.y)? min.y : result.y;
    return result;
}

void Engine::UpdateMouse()
{
	mouse = GetMousePosition();
    virtualMouse = { 0 };
    virtualMouse.x = (mouse.x - (GetScreenWidth() - (GameScreenWidth*screenScale))*0.5f)/screenScale;
    virtualMouse.y = (mouse.y - (GetScreenHeight() - (GameScreenHeight*screenScale))*0.5f)/screenScale;
    virtualMouse = ClampValue(virtualMouse, { 0, 0 },{ (float)GameScreenWidth, (float)GameScreenHeight});
}
void Engine::UpdateGameScreenRects()
{
	screenScale = min((float)GetScreenWidth()/GameScreenWidth,(float)GetScreenHeight()/GameScreenHeight);
	gameRect = { 0.0f, 0.0f, (float)(GameScreenWidth), -(float)(GameScreenHeight)};
	gameScaledRect = {(GetScreenWidth() - ((float)(GameScreenWidth)*screenScale)) * 0.5f,
					  (GetScreenHeight() - ((float)(GameScreenHeight)*screenScale)) * 0.5f,
						(float)(GameScreenWidth)*screenScale, (float)(GameScreenHeight) * screenScale};

	currentAspectRatio = (float)GetScreenWidth()/GetScreenHeight();

	if (currentAspectRatio != GameRatio) 
	{
		SetWindowSize((currentAspectRatio > GameRatio) ? GameScreenWidth * screenScale : GetScreenWidth(), 
					  (currentAspectRatio > GameRatio) ? GetScreenHeight() : GameScreenHeight * screenScale);
		UpdateGameScreenRects(); //Al compilador no le gusta nada esto aquí, mirar... peta si CFLAGS += -s -O1
	}
}

void Engine::FullScreen()
{
	if (!IsWindowFullscreen())
	{
		previusWindowsWidth = GetScreenWidth();
		previusWindowsHeight = GetScreenHeight();

		for(auto currentR : resolution16_10)
		{
			SetWindowSize(currentR.x, currentR.y);
			float currentRatio =(float)GetScreenWidth()/GetScreenHeight();
			if(currentRatio==GameRatio) break;
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

void Engine::Init(){}
void Engine::Tick(){}
void Engine::Draw(){}
void Engine::OverDraw()
{
	DrawRectangle(0,0,GameScreenWidth,GameScreenHeight, (Color){ 0, 0, 0, 220});
    if (GuiButton(Rectangle{10,10,100,20},  "FullScreen")) { ToggleFullscreen(); }
}

void Engine::InitCRTShader()
{
    timeLoc = GetShaderLocation(shader, "uTime");
}
void Engine::UpdateCRTShader()
{
    SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
}

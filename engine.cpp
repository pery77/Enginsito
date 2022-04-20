#include "engine.h"

Engine::Engine()
{

}

Engine::~Engine()
{
	printf("Unload Engine");
	UnloadRenderTexture(mainRender);
}

//Initialize engine.
void Engine::Init()
{
	const int windowWidth = GameScreenWidth*3;
    const int windowHeight = GameScreenHeight*3;

	//Set Vsync and make the window resizeable
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

	//Create window
	InitWindow(windowWidth, windowHeight, "Enginsito");
	SetWindowMinSize(GameScreenWidth, GameScreenHeight);
	SetTargetFPS(60);

	//Create main texture and disable texture filter.
	mainRender = LoadRenderTexture(GameScreenWidth, GameScreenHeight);
	SetTextureFilter(mainRender.texture, TEXTURE_FILTER_POINT);



	//Create glow effect.
	//glow = new Glow(GameScreenWidth, ScreenHeight);
	//glow->SetFilter(1);

}

//Start engine.
void Engine::Go()
{
	Init();
	MainLoop();
}

void Engine::MainLoop()
{
	UpdateGameScreenRects();
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		if(IsWindowResized()) UpdateGameScreenRects();
		UpdateMouse();
		ProcessInput();
		Update();
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
}


void Engine::Update()
{

}

//Draw loop.
void Engine::RenderFrame()
{
	BeginDrawing();

	//Draw game to texture.
	BeginTextureMode(mainRender);
	//DrawBackground solid color.
	ClearBackground(BLACK);
	DrawRectangleLines(0,0,GameScreenWidth,GameScreenHeight,RED);

	DrawText(TextFormat("Default Mouse: [%i , %i]", (int)mouse.x, (int)mouse.y), 0, 0, 8, GREEN);
    DrawText(TextFormat("Virtual Mouse: [%i , %i]", (int)virtualMouse.x, (int)virtualMouse.y), 0, 10, 8, YELLOW);
    DrawText(TextFormat("ssss ss: [%i , %i]", (int)gameScaledRect.width, (int)gameScaledRect.height), 0, 20, 8, YELLOW);
    DrawText(TextFormat("ssss ss: [%i , %i]", (int)GetScreenWidth(), (int)GetScreenHeight()), 0, 30, 8, BLUE);
   

	EndTextureMode();

	ClearBackground(BLACK);

	//Blend texture for postprocess effect.
	BeginBlendMode(1);

	//Draw main texture scaled to screen.
	DrawTexturePro(mainRender.texture, gameRect, gameScaledRect, { 0.0f, 0.0f }, 0.0f, WHITE);
	
	//Draw glow frist pass (big glow effect)
	//glow->BigGlow(mainRender.texture);
	//DrawTexturePro(glow->BlurTexture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);
	//Draw glow second pass
	//glow->SetValues(.5, 1.1, 1.5);
	//glow->SetValues(.99, 0.1, 0.1);
	//glow->BlurTexture = glow->DrawGlow(mainRender.texture);
	//DrawTexturePro(glow->BlurTexture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);
	//End draw main + postprocess 
	EndBlendMode();

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
        virtualMouse = ClampValue(virtualMouse, { 0, 0 },{ static_cast<float>(GameScreenWidth), static_cast<float>(GameScreenHeight) });
}

void Engine::UpdateGameScreenRects()
{
	screenScale = min((float)GetScreenWidth()/GameScreenWidth,(float)GetScreenHeight()/GameScreenHeight);
	gameRect = { 0.0f, 0.0f, static_cast<float>(GameScreenWidth), -static_cast<float>(GameScreenHeight)};
	gameScaledRect = {(GetScreenWidth() - (static_cast<float>(GameScreenWidth)*screenScale))*0.5f,
						(GetScreenHeight() - (static_cast<float>(GameScreenHeight)*screenScale))*0.5f,
						static_cast<float>(GameScreenWidth)*screenScale, static_cast<float>(GameScreenHeight)*screenScale};

}

void Engine::FullScreen()
{
	if (!IsWindowFullscreen())
	{
		previusWindowsWidth = GetScreenWidth();
		previusWindowsHeight = GetScreenHeight();
		SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
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
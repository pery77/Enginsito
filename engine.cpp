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
	HideCursor();
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
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
}

//Draw loop.
void Engine::RenderFrame()
{
	BeginDrawing();

	//Draw game to texture.
	BeginTextureMode(mainRender);
	//DrawBackground solid color.
	ClearBackground({31,20,15,255});
	DrawRectangleLines(0,0,GameScreenWidth,GameScreenHeight,RED);

	DrawText(TextFormat("Default Mouse: [%i , %i]", (int)mouse.x, (int)mouse.y), 0, 0, 8, GREEN);
    DrawText(TextFormat("Virtual Mouse: [%i , %i]", (int)virtualMouse.x, (int)virtualMouse.y), 0, 10, 8, YELLOW);
    DrawText(TextFormat("game r: [%f]", GameRatio), 0, 20, 8, YELLOW);
    DrawText(TextFormat("apect: [%f]",currentAspectRatio), 0, 30, 8, BLUE);
   
	DrawPixel(virtualMouse.x, virtualMouse.y,WHITE);


	Draw();

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
        virtualMouse = ClampValue(virtualMouse, { 0, 0 },{ static_cast<float>(GameScreenWidth), static_cast<float>(GameScreenHeight) });
}

void Engine::UpdateGameScreenRects()
{
	printf("Updating\n");
	screenScale = min((float)GetScreenWidth()/GameScreenWidth,(float)GetScreenHeight()/GameScreenHeight);
	gameRect = { 0.0f, 0.0f, (float)(GameScreenWidth), -(float)(GameScreenHeight)};
	gameScaledRect = {(GetScreenWidth() - ((float)(GameScreenWidth)*screenScale))*0.5f,
						(GetScreenHeight() - ((float)(GameScreenHeight)*screenScale))*0.5f,
						(float)(GameScreenWidth)*screenScale, (float)(GameScreenHeight)*screenScale};
	currentAspectRatio = (float)GetScreenWidth()/GetScreenHeight();

	if (currentAspectRatio != GameRatio) 
	{
		SetWindowSize((currentAspectRatio > GameRatio) ? GameScreenWidth * screenScale : GetScreenWidth(), 
					 (currentAspectRatio > GameRatio) ? GetScreenHeight() : GameScreenHeight * screenScale);
		UpdateGameScreenRects();
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

void Engine::Tick(){}
void Engine::Draw(){}
void Engine::OverDraw(){}

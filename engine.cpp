#include "engine.h"

pery::Engine::Engine()
{

}

pery::Engine::~Engine()
{
	printf("Unload Engine");
	UnloadRenderTexture(mainRender);
}

//Initialize engine.
void pery::Engine::Init()
{
	//Set Vsync
	SetConfigFlags(FLAG_VSYNC_HINT);
	//Create window
	InitWindow(ScreenWidth * ScreenScale, ScreenHeight * ScreenScale, "Enginsito");
	SetTargetFPS(60);
	//ToggleFullscreen();
	//Create main texture and disable texture filter.
	mainRender = LoadRenderTexture(ScreenWidth, ScreenHeight);
	SetTextureFilter(mainRender.texture, 0);

	//Create glow effect.
	//glow = new Glow(ScreenWidth, ScreenHeight);
	//glow->SetFilter(1);

}

//Start engine.
void pery::Engine::Go()
{
	Init();
	MainLoop();
}

void pery::Engine::MainLoop()
{
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		ProcessInput();
		RenderFrame();
		Update();
	}		

	CloseWindow();					// Close window and OpenGL context
}

void pery::Engine::ProcessInput()
{
	
	if(IsKeyReleased(KEY_F11))
		ToggleFullscreen();
}

void pery::Engine::Update()
{

}

//Draw loop.
void pery::Engine::RenderFrame()
{

	BeginDrawing();
	ClearBackground(BLANK);
	//Draw game to texture.
	BeginTextureMode(mainRender);
	//DrawBackground solid color.
	DrawRectangleLines(0,0,ScreenWidth,ScreenHeight,RED);

	EndTextureMode();

	ClearBackground(BLACK);


	//Blend texture for postprocess effect.
	BeginBlendMode(1);
	//Draw main texture scaled to screen.
	DrawTexturePro(mainRender.texture, sourceRec, scaledRec, { 0, 0 }, 0, WHITE);
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
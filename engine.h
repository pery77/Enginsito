#ifndef ENGINE_H_
#define ENGINE_H_

#include "raylib.h"
#include <string>

#define max(a, b) ((a)>(b)? (a) : (b))
#define min(a, b) ((a)<(b)? (a) : (b))

struct Vector2i {
	int x;
	int y;
};

class Engine {

public:
	//Screen constant values.
	const int GameScreenWidth {320};
	const int GameScreenHeight {200};
	const float GameRatio {(float)GameScreenWidth/GameScreenHeight};
	const char * crtFile = "assets/pery_crt_deluxe.fs";

	Engine();
	~Engine();

	//Start the engine
	void Run();

	virtual void Init();		//Initialize engine.
	virtual void Tick();		//Update loop.
	virtual void Draw();		//Draw inside the game.	
	virtual void OverDraw();	//Draw in window.

	Vector2i GetVirtualMousePosition()	{
		return Vector2i{(int)virtualMouse.x, (int)virtualMouse.y};
	}

private:
	void MainLoop();	 //Main loop
	void RenderFrame();  //Draw loop
	void ProcessInput(); //Keys events loop.
	void UpdateMouse();	 //Mouse updates
	void FullScreen();   //Controll fullscreen.
	void UpdateGameScreenRects();

	//Tools
	Vector2 ClampValue(Vector2 value, Vector2 min, Vector2 max);

	//Main texture for render
	RenderTexture2D mainRender;
	float screenScale = {};
	float previusWindowsWidth = {};
	float previusWindowsHeight = {};
	float currentAspectRatio = {};
	Rectangle gameRect;
	Rectangle gameScaledRect;
	Vector2 mouse;
	Vector2 virtualMouse;
	
	Shader shader;

	Vector2i resolution16_10[5] = {
		{2560,1600},
		{1920,1200},
		{1680,1050},
		{1440,900},
		{1280,800}
		};
};
#endif  //ENGINE_H_
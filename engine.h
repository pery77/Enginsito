#pragma once
#include "raylib.h"
#include <string>

#define max(a, b) ((a)>(b)? (a) : (b))
#define min(a, b) ((a)<(b)? (a) : (b))

class Engine {

public:
	//Screen constant values.
	const int GameScreenWidth {320};
	const int GameScreenHeight {200};

	Engine();
	~Engine();

	//Start the engine
	void Go();


private:
	void Init();		 //Initialize engine.
	void MainLoop();	 //Main loop
	void RenderFrame();  //Draw loop
	void ProcessInput(); //Keys events loop.
	void Update();		 //Update loop.
	void UpdateMouse();

	//Tools
	Vector2 ClampValue(Vector2 value, Vector2 min, Vector2 max);
	void UpdateGameScreenRects();

	//Main texture for render
	RenderTexture2D mainRender;
	float screenScale;
	Rectangle gameRect;
	Rectangle gameScaledRect;
	Vector2 mouse;
	Vector2 virtualMouse;
};
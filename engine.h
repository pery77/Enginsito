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

	Engine();
	~Engine();

	//Start the engine
	void Go();
	virtual void Tick();		 //Update loop.
	virtual void Draw();
	virtual void OverDraw();
private:
	void Init();		 //Initialize engine.
	void MainLoop();	 //Main loop
	void RenderFrame();  //Draw loop
	void ProcessInput(); //Keys events loop.
	void UpdateMouse();
	void FullScreen();
	void UpdateGameScreenRects();

	//Tools
	Vector2 ClampValue(Vector2 value, Vector2 min, Vector2 max);

	//Main texture for render
	RenderTexture2D mainRender;
	float screenScale;
	float previusWindowsWidth;
	float previusWindowsHeight;
	float currentAspectRatio;
	Rectangle gameRect;
	Rectangle gameScaledRect;
	Vector2 mouse;
	Vector2 virtualMouse;

	Vector2i resolution16_10[5] = {
		{2560,1600},
		{1920,1200},
		{1680,1050},
		{1440,900},
		{1280,800}
		};

};
#endif  //ENGINE_H_
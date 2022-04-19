#pragma once
#include "raylib.h"
#include <string>

namespace pery {

	class Engine {

	public:
		//Screen constant values.
		const int ScreenWidth {320};
		const int ScreenHeight {200};
		const int ScreenScale {4};

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

		void LoadLevel(std::string name);

		//Main texture for render
		RenderTexture2D mainRender;

		//Rectangles for render main texture in screen or window.
		Rectangle sourceRec = { 0.0f, 0.0f, (float)ScreenWidth, -(float)ScreenHeight };
		Rectangle scaledRec = { 0.0f, 0.0f, (float)ScreenWidth * (float)ScreenScale, (float)ScreenHeight * (float)ScreenScale };

	};
}
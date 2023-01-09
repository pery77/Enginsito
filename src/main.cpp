#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "ini_manager.h"
#include "my_basic.h"

#include "tools.h"
#include "mb_manager.h"
#include "postProcessing.h"

#define RFXGEN_IMPLEMENTATION
#include "rfxgen.h"

Texture test;
Sound fxWav;

static Tools* tools = new Tools();
#define MAX_WAVE_SLOTS 255
int sc = 0;

int main(int argc, char *argv[])
{
    IniManager* config = new IniManager();
    tools = new Tools();
    MBManager* basic = new MBManager(tools);

    const int windowWidth = tools->GameScreenWidth * config->size;
    const int windowHeight = tools->GameScreenHeight * config->size;

	//Set Vsync and make the window resizeable
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

	//Create window
	InitWindow(windowWidth, windowHeight, "peryEngine");
	SetWindowMinSize(tools->GameScreenWidth, tools->GameScreenHeight);
	SetTargetFPS(60);

    InitAudioDevice();  

    PostProcessing* postProcessing = new PostProcessing(tools);

    tools->UpdateGameScreenRects();
    bool running = false;
    bool showFps = false;

    test = LoadTexture("assets/test.png");
    Sound fxWav = LoadSound("assets/sound.wav");





    WaveParams params[MAX_WAVE_SLOTS] = { 0 }; // Wave parameters for generation
    Wave wave[MAX_WAVE_SLOTS] = { 0 };
    Sound sound[MAX_WAVE_SLOTS] = { 0 };

    for (int i = 0; i < MAX_WAVE_SLOTS; i++)
    {
        // Reset generation parameters
        // NOTE: Random seed for generation is set
        ResetWaveParams(&params[i]);
        params[i] = GenRandomize();

        // Default wave values
        wave[i].sampleRate = RFXGEN_GEN_SAMPLE_RATE;
        wave[i].sampleSize = RFXGEN_GEN_SAMPLE_SIZE;
        wave[i].channels = RFXGEN_GEN_CHANNELS;
        wave[i].frameCount = 10*wave[i].sampleRate;    // Max frame count for 10 seconds
        wave[i].data = (float *)RL_CALLOC(wave[i].frameCount, sizeof(float));
        wave[i].data = GenerateWave(params[i], &wave[i].frameCount);
        sound[i] = LoadSoundFromWave(wave[i]);
    }







    // Game Loop
    while (!WindowShouldClose())
    {
        // Engine keys
        if(IsKeyReleased(KEY_F1)){
            showFps = !showFps;
            PlaySound(fxWav);
        }

        if(IsKeyReleased(KEY_F2)){
            PlaySound(sound[sc]);
            printf("PLAy %i\n",sc++);
            if (sc > MAX_WAVE_SLOTS - 1) sc = 0;
        }

        if(IsKeyReleased(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyReleased(KEY_ENTER))){
		    tools->FullScreen();
	    }

        if(IsKeyReleased(KEY_F10)){
		    postProcessing->ReloadShaders();
	    }

        if(IsWindowResized()) 
        {
            tools->UpdateGameScreenRects();
        }

        postProcessing->uTime = GetTime();
        
        //Interpreter
        if (IsKeyReleased(KEY_F5)){ 
            if (running){
                basic->end();
            }
            
            basic->OpenBas(); 

            HideCursor();
            basic->init();
            running = true;
        }
        if (IsKeyReleased(KEY_F6)){ 
            if (running){
                ShowCursor();
                basic->end();
                running = false;
                basic->CloseBas();
            }
        }

        // Update
        if (running){
            basic->tick();
        }

        // Draw
        BeginDrawing();

            //Draw game to texture.
            BeginTextureMode(postProcessing->mainRender);
                if (!running){
                    ClearBackground(GRAY);
                    DrawTexture(test, 0, 0, WHITE);
                    DrawText("Press F5",0,0,8,RED);
                }else{
                    basic->draw();
                }
            EndTextureMode();

            // Main draw
            BeginBlendMode(0); // 0 Alpha (blur) 1 Additive (glow)
                postProcessing->RenderMain();
                postProcessing->RenderBlur();
            EndBlendMode();
   
            // Final Draw
            ClearBackground(BLACK);
            postProcessing->RenderFinal();

            // Engine over draw
            if(showFps){
                DrawFPS(0, 0);
                postProcessing->uBlurPower = 
                    GuiSlider((Rectangle){0,50,300,20},"", TextFormat("%f",postProcessing->uBlurPower),postProcessing->uBlurPower ,0,20);
                postProcessing->uBlurFactor = 
                    GuiSlider((Rectangle){0,70,300,20},"", TextFormat("%f",postProcessing->uBlurFactor),postProcessing->uBlurFactor ,0.2,2);
                               postProcessing->uTest = 
                    GuiSlider((Rectangle){0,90,300,20},"", TextFormat("%f",postProcessing->uTest),postProcessing->uTest ,-10,10);
                                                   postProcessing->uCurvature = 
                    GuiSlider((Rectangle){0,110,300,20},"", TextFormat("%f",postProcessing->uCurvature),postProcessing->uCurvature ,-10,10);
            }

        EndDrawing();
    }

    CloseWindow();

    basic->CloseBas();

    delete config;
    delete basic;

    return 0;
}
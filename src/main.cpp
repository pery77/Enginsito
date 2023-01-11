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

#define TSF_IMPLEMENTATION
#include "tsf.h"

#include<stdio.h>
#include<stdlib.h>

#define MAX_SAMPLES               512
#define MAX_SAMPLES_PER_UPDATE   4096

Texture test;

static Tools* tools = new Tools();
#define MAX_WAVE_SLOTS 32
int sc = 0;


tsf* ptsf = tsf_load_filename("assets/8bit.sf2");
//tsf* ptsf = tsf_load_filename("assets/ins.sf2");

void AudioInputCallback(void *buffer, unsigned int frames)
{
	int sampleCount = (frames / (0.5 * sizeof(short)));
	tsf_render_short(ptsf, (short*)buffer, sampleCount, 0);
}


//void wait(unsigned int);

void wait(unsigned int t)
{
    WaitTime(t * 0.001);
}


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


    WaveParams params[MAX_WAVE_SLOTS] = { 0 }; // Wave parameters for generation
    Wave wave[MAX_WAVE_SLOTS] = { 0 };
    Sound sound[MAX_WAVE_SLOTS] = { 0 };

    for (int i = 0; i < MAX_WAVE_SLOTS; i++)
    {
        // Reset generation parameters
        // NOTE: Random seed for generation is set
        ResetWaveParams(&params[i]);
        //params[i] = GenRandomize();

        // Default wave values
        wave[i].sampleRate = RFXGEN_GEN_SAMPLE_RATE;
        wave[i].sampleSize = RFXGEN_GEN_SAMPLE_SIZE;
        wave[i].channels = RFXGEN_GEN_CHANNELS;
        wave[i].frameCount = 10*wave[i].sampleRate;    // Max frame count for 10 seconds
        //wave[i].data = (float *)RL_CALLOC(wave[i].frameCount, sizeof(float));
        wave[i].data = GenerateWave(params[i], &wave[i].frameCount);
        sound[i] = LoadSoundFromWave(wave[i]);
    }






    tsf_set_output(ptsf, TSF_STEREO_INTERLEAVED, 44100, -10);          


    SetAudioStreamBufferSizeDefault(MAX_SAMPLES_PER_UPDATE);
    AudioStream stream = LoadAudioStream(44100, 16, 2);
    SetAudioStreamCallback(stream, AudioInputCallback);

    PlayAudioStream(stream);
    /*
    const char* seq = "ML AA8G8E.D8C2P2 E.D8C<A8G8G2>P2 <G.A8G.A8>C.D8EG A.G8E8D8CD2";
    tsf_play_async(ptsf, 0, seq, 1.0f);
    while (seq && *seq) {
        seq = tsf_play_await(ptsf, 10.0f / 1000.0f);
        WaitTime(0.01);
    }
    */
    //tsf_play(ptsf,0, "ML AA8G8E.D8C2P2 E.D8C<A8G8G2>P2 <G.A8G.A8>C.D8EG A.G8E8D8CD2", 1.0f, wait);
    // Game Loop
    while (!WindowShouldClose())
    {


int k = GetKeyPressed();
if (k!= 0)
{
    tsf_note_on(ptsf, sc, k, 1.0f);
}




        // Engine keys
        if(IsKeyReleased(KEY_F1)){
            showFps = !showFps;
        }

        if(IsKeyReleased(KEY_F2)){
            PlaySound(sound[sc]);
            printf("PLAy %i\n",sc);
            if (sc > MAX_WAVE_SLOTS - 1) sc = 0;
            tsf_play(ptsf,sc, "ML AA8G8E.D8C2P2 E.D8C<A8G8G2>P2 <G.A8G.A8>C.D8EG A.G8E8D8CD2", 1.0f, wait);
            sc++;
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
Vector2 position;
             for (int i = 0; i < 200; i++)
            {
                position.x = (float)i;
                //position.y = 128+ 50*(stream->buffer)[i*22050/200]/32000.0f;

                DrawPixelV(position, RED);
            }
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
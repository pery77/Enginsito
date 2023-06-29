#pragma once
#include <stdint.h>

#define NUM_CHANNELS 4
#define NUM_PRESETS  16

#define TABLE_SIZE  64
#define SAMPLERATE  44100.0

#define NOISE_SAMPLES 22050

typedef struct 
{
	int    note       = 69;
    float  volume     = 1.0f;
	double phase      = 0.0;
	double lfoPhase   = 0.0;
	double slidePhase = 0.0;
	double noteTimeOn 	  = 0.0;
    double noteTimeOff    = 0.0;
    double noteTotalTime  = 0.0;
	double fltp       = 0.0;
	double fltdp      = 0.0;
	bool isPlaying	    = false;
    double sequenceTime = 0.0;
	unsigned int tick   = 0;
	uint8_t frame[441];
	uint8_t currentPreset = 0;
} Channel;

const int SQUARE       = 0;
const int OSC_SQUARE12 = 1;
const int OSC_SQUARE25 = 2;
const int OSC_TRIANGLE = 3;
const int OSC_SAW      = 4;
const int OSC_NOISE    = 5;

class AudioManager;

class RetroSynth
{
    public:
    RetroSynth(AudioManager* _audioManagerRef);
    ~RetroSynth();

	uint8_t SQUARE [64] =
		{ 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		};
	uint8_t PULSE_12 [64]  =
		{ 255, 255, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		};		
	uint8_t PULSE_25 [64]  =
		{ 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		};
	uint8_t TRIANGLE [64] = 
		{   0,   8,  16,  24,  32,  40,  48,  56,  64,  72,  80,  88,  96, 104, 112, 120, 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224, 232, 240, 248,
		  248, 240, 232, 224, 216, 208, 200, 192, 184, 176, 168, 160, 152, 144, 136, 128, 120, 112, 104,  96,  88,  80,  72,  64,  56,  48,  40,  32,  24,  16,   8,   0
		};
	uint8_t SAW [64] = 
		{   0,   8,  16,  24,  32,  40,  48,  56,  64,  72,  80,  88,  96, 104, 112, 120, 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224, 232, 240, 248,
		    0,   8,  16,  24,  32,  40,  48,  56,  64,  72,  80,  88,  96, 104, 112, 120, 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224, 232, 240, 248
		};
	uint8_t NOISE [NOISE_SAMPLES];

    Channel channels[NUM_CHANNELS] = {0};

	void SetChannelPreset(uint8_t channel, uint8_t preset);
	void AudioInputCallback(void* buffer, unsigned int frames);
	AudioManager* audioManagerRef;
	uint8_t GetFrameAverage(uint8_t channel, uint8_t frame);
    private:
    double renderChannel(uint8_t channel);
    void   resetChannelPhase(uint8_t channel);
	double waveTable(uint8_t channel, float freq, uint8_t osc);
	float  steps = 1.0 / SAMPLERATE;
	double amplitude(uint8_t preset, const double dTime, const double dTimeOn, const double dTimeOff);
};
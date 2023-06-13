#pragma once
#include <stdint.h>

#define NUM_CHANNELS 4
#define NUM_PRESETS  16

#define TABLE_SIZE  64
#define SAMPLERATE  44100.0


#define NOISE_SAMPLES 22050

struct envelope {
	virtual double amplitude(const double dTime, const double dTimeOn, const double dTimeOff) = 0;
};

struct ADSR : public envelope{

	double Attack;
	double Decay;
	double Sustain;
	double Release;
	double Amplitude;

	ADSR() {

		Attack    = 0.04;
		Decay     = 0.04;
		Sustain   = 1.0;
		Release   = 0.02;
	}

	virtual double amplitude(const double dTime, const double dTimeOn, const double dTimeOff) {

		double dAmplitude = 0.0;
		double dReleaseAmplitude = 0.0;
		if (dTimeOn > dTimeOff) // Note is on
		{
			double dLifeTime = dTime - dTimeOn;

			if (dLifeTime <= Attack)
				dAmplitude = (dLifeTime / Attack);

			if (dLifeTime > Attack && dLifeTime <= (Attack + Decay))
				dAmplitude = ((dLifeTime - Attack) / Decay) * (Sustain - Amplitude) + 1.0f;

			if (dLifeTime > (Attack + Decay))
				dAmplitude = Sustain;
		}
		else // Note is off
		{
			double dLifeTime = dTimeOff - dTimeOn;

			if (dLifeTime <= Attack)
				dReleaseAmplitude = (dLifeTime / Attack);

			if (dLifeTime > Attack && dLifeTime <= (Attack + Decay))
				dReleaseAmplitude = ((dLifeTime - Attack) / Decay) * (Sustain - Amplitude) + 1.0f;

			if (dLifeTime > (Attack + Decay))
				dReleaseAmplitude = Sustain;

			dAmplitude = ((dTime - dTimeOff) / Release) * (0.0 - dReleaseAmplitude) + dReleaseAmplitude;

		}

		if (dAmplitude <= 0.00001)	dAmplitude = 0.0;
		if (dAmplitude > 0.9)		dAmplitude = 0.9;

		return dAmplitude;
	}
};

typedef struct {
	float speed   = 0.0;
 	float depht  = 0.0;
} LFO;

typedef struct {
	float slope = 0.0;
 	float curve = 0.0;
} Slide;

typedef struct {
	float cutoff    = 1.0;
 	float resonance = 0.0;
} Filter;

typedef struct {
    int    osc = 0;
    ADSR   env;
	LFO    lfo;
	Filter LPF;
	Slide  slide;
} Preset;

typedef struct {
	Preset *preset;
	int    note       = 69;
    float  volume     = 0.5f;
	double phase      = 0.0;
	double lfoPhase   = 0.0;
	double slidePhase = 0.0;
	double timeOn 	  = 0.0;
    double timeOff    = 0.0;
    double time       = 0.0;
	double fltp       = 0.0;
	double fltdp      = 0.0;
} Channel;

const int SQUARE       = 0;
const int OSC_SQUARE12 = 1;
const int OSC_SQUARE25 = 2;
const int OSC_TRIANGLE = 3;
const int OSC_NOISE    = 4;

class RetroSynth{

    public:
    RetroSynth();
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

	uint8_t NOISE [NOISE_SAMPLES];

    Channel channels[NUM_CHANNELS] = {0};
    Preset presets[NUM_PRESETS] = {0};

    double RenderNote(int channel, int oscT, int note);
	void SetChannelPreset(uint8_t channel, uint8_t preset);
	void AudioInputCallback(void* buffer, unsigned int frames);

	double musicTime = 0.0;

    private:
    double osc(const int dChannel, const double dHertz, const int nType);
	double waveTable(int channel, float freq, uint8_t osc);
	float steps = 1.0 / SAMPLERATE;
};
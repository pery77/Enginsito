#pragma once
#include <stdint.h>

#define MAX_VOICES 4 // one per track
#define FTYPE double

#define TABLE_SIZE  64

struct envelope {
	virtual FTYPE amplitude(const FTYPE dTime, const FTYPE dTimeOn, const FTYPE dTimeOff) = 0;
};

struct ADSR : public envelope{

	FTYPE Attack;
	FTYPE Decay;
	FTYPE Sustain;
	FTYPE Release;
	FTYPE Amplitude;

	ADSR() {

		Attack = 0.04;
		Decay = 0.04;
		Sustain = 1.0;
		Release = 0.02;
		Amplitude = 1.0;
	}

	virtual FTYPE amplitude(const FTYPE dTime, const FTYPE dTimeOn, const FTYPE dTimeOff) {

		FTYPE dAmplitude = 0.0;
		FTYPE dReleaseAmplitude = 0.0;
		if (dTimeOn > dTimeOff) // Note is on
		{
			FTYPE dLifeTime = dTime - dTimeOn;

			if (dLifeTime <= Attack)
				dAmplitude = (dLifeTime / Attack) * Amplitude;

			if (dLifeTime > Attack && dLifeTime <= (Attack + Decay))
				dAmplitude = ((dLifeTime - Attack) / Decay) * (Sustain - Amplitude) + Amplitude;

			if (dLifeTime > (Attack + Decay))
				dAmplitude = Sustain;
		}
		else // Note is off
		{
			FTYPE dLifeTime = dTimeOff - dTimeOn;

			if (dLifeTime <= Attack)
				dReleaseAmplitude = (dLifeTime / Attack) * Amplitude;

			if (dLifeTime > Attack && dLifeTime <= (Attack + Decay))
				dReleaseAmplitude = ((dLifeTime - Attack) / Decay) * (Sustain - Amplitude) + Amplitude;

			if (dLifeTime > (Attack + Decay))
				dReleaseAmplitude = Sustain;

			dAmplitude = ((dTime - dTimeOff) / Release) * (0.0 - dReleaseAmplitude) + dReleaseAmplitude;

		}

		// Amplitude should not be negative
		if (dAmplitude <= 0.00001)
			dAmplitude = 0.0;

		return dAmplitude;
	}
};

typedef struct {
	float freq      = 0.0;
 	float amplitude = 0.0;
	float phase     = 0.0;
} LFO;

typedef struct {
	float slope = 0.0;
 	float curve = 0.0;
	float phase = 0.0;
} Slide;

typedef struct {
	float cutoff    = 1.0;
 	float resonance = 0.0;
} Filter;

typedef struct {
    int osc = 0;
    int note = 69;
    float volume = 0.5;
    double timeOn = 0.0;
    double timeOff = 0.0;
    double time = 0.0;
	double phase = 0.0;
    ADSR env;
	LFO lfo;
	Filter LPF;
	Slide slide;
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

    FTYPE RenderNote(int channel, int oscT, int note);
    FTYPE FrequencyFromNote(int midi_note);

    Channel channels[MAX_VOICES] = {0};

    private:
    FTYPE osc(const int dChannel, const FTYPE dHertz, const int nType);
	FTYPE waveTable(int channel, float freq, uint8_t osc);

};
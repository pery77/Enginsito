#pragma once
#include <stdint.h>

#define MAX_VOICES 4 // one per track
#define FTYPE double

#define TABLE_SIZE  64

struct envelope {
	virtual FTYPE amplitude(const FTYPE dTime, const FTYPE dTimeOn, const FTYPE dTimeOff) = 0;
};

struct envelope_adsr : public envelope{

	FTYPE dAttackTime;
	FTYPE dDecayTime;
	FTYPE dSustainAmplitude;
	FTYPE dReleaseTime;
	FTYPE dStartAmplitude;

	envelope_adsr() {

		dAttackTime = 0.04;
		dDecayTime = 0.04;
		dSustainAmplitude = 1.0;
		dReleaseTime = 0.02;
		dStartAmplitude = 1.0;
	}

	virtual FTYPE amplitude(const FTYPE dTime, const FTYPE dTimeOn, const FTYPE dTimeOff) {

		FTYPE dAmplitude = 0.0;
		FTYPE dReleaseAmplitude = 0.0;
		if (dTimeOn > dTimeOff) // Note is on
		{
			FTYPE dLifeTime = dTime - dTimeOn;

			if (dLifeTime <= dAttackTime)
				dAmplitude = (dLifeTime / dAttackTime) * dStartAmplitude;

			if (dLifeTime > dAttackTime && dLifeTime <= (dAttackTime + dDecayTime))
				dAmplitude = ((dLifeTime - dAttackTime) / dDecayTime) * (dSustainAmplitude - dStartAmplitude) + dStartAmplitude;

			if (dLifeTime > (dAttackTime + dDecayTime))
				dAmplitude = dSustainAmplitude;
		}
		else // Note is off
		{
			FTYPE dLifeTime = dTimeOff - dTimeOn;

			if (dLifeTime <= dAttackTime)
				dReleaseAmplitude = (dLifeTime / dAttackTime) * dStartAmplitude;

			if (dLifeTime > dAttackTime && dLifeTime <= (dAttackTime + dDecayTime))
				dReleaseAmplitude = ((dLifeTime - dAttackTime) / dDecayTime) * (dSustainAmplitude - dStartAmplitude) + dStartAmplitude;

			if (dLifeTime > (dAttackTime + dDecayTime))
				dReleaseAmplitude = dSustainAmplitude;

			dAmplitude = ((dTime - dTimeOff) / dReleaseTime) * (0.0 - dReleaseAmplitude) + dReleaseAmplitude;

		}

		// Amplitude should not be negative
		if (dAmplitude <= 0.00001)
			dAmplitude = 0.0;

		return dAmplitude;
	}
};

typedef struct {
	FTYPE dLFOHertz = 0.0;
 	FTYPE dLFOAmplitude = 0.0;
} LFO;

typedef struct {
    int osc = 0;
    int note = 69;
    float volume = 0.5;
    double timeOn = 0.0;
    double timeOff = 0.0;
    envelope_adsr env;
	LFO lfo;
	float cutOff = 0.0; // 0 to 1
	float resonance = -20; //-20 to 20
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

    float RenderNote(int oscT, int note, float time, float timeOn, float lfoHertz, float lfoAmp);
	void SetEnv(int channel, float attackTime, float decayTime, float sustainAmplitude, float releaseTime, float dStartAmplitude);
	
    float FrequencyFromNote(int midi_note);

    Channel channels[MAX_VOICES] = {0};

    private:
    FTYPE osc(const FTYPE dTime, const FTYPE dHertz, const int nType, const FTYPE dLFOHertz, const FTYPE dLFOAmplitude);
	FTYPE waveTable(float freq, uint8_t osc);

};
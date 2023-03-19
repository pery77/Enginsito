#pragma once

#define MAX_VOICES 4
#define FTYPE double

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
		if (dAmplitude <= 0.01)
			dAmplitude = 0.0;
		return dAmplitude;
	}
};

typedef struct {
    int osc = 0;
    int note = 69;
    float volume = 0.5;
    double timeOn = 0.0;
    double timeOff = 0.0;
    envelope_adsr env;

} Voice;

const int OSC_SINE = 0;
const int OSC_SQUARE = 1;
const int OSC_TRIANGLE = 2;
const int OSC_SAW_DIG = 3;
const int OSC_NOISE = 4;

class RetroSynth{

    public:
    RetroSynth();
    ~RetroSynth();

    float RenderNote(int oscT, int note, float time);

    Voice voices[MAX_VOICES] = {0};

    private:
    float frequencyFromNote(int midi_note);
    FTYPE osc(const FTYPE dTime, const FTYPE dHertz, const int nType = OSC_SINE, const FTYPE dLFOHertz = 0.0, const FTYPE dLFOAmplitude = 0.0, FTYPE dCustom = 50.0);

};
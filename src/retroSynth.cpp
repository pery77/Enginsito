#include "retroSynth.h"
#include <cmath>

#define INV2PI  0.63661977 // 2.0 / PI
#define HALF_PI 1.57079632 // PI / 2.0
#define PI      3.14159565
#define PI2     6.28318530 // PI * 2.0

// Converts frequency (Hz) to angular velocity
FTYPE w(const FTYPE dHertz) {
	return dHertz * PI2;
}

RetroSynth::RetroSynth(){
}
RetroSynth::~RetroSynth(){
}

float RetroSynth::frequencyFromNote(int midi_note) {
    return 440.0f * pow(2.0f, (midi_note - 69) / 12.0f);
}

float RetroSynth::RenderNote(int oscT, int note, float time) {
    float frequency = frequencyFromNote(note);
    float value = osc(time, frequency, oscT);
    return value;
}

FTYPE RetroSynth::osc(const FTYPE dTime, const FTYPE dHertz, const int nType, const FTYPE dLFOHertz, const FTYPE dLFOAmplitude) {

	FTYPE dFreq = w(dHertz) * dTime + dLFOAmplitude * dHertz * (sin(w(dLFOHertz) * dTime));
	switch (nType)
	{
        case OSC_SINE: // Sine wave bewteen -1 and +1
            return sin(dFreq);

        case OSC_SQUARE50: // Square wave between -1 and +1
            return sin(dFreq) > 0 ? 1.0 : -1.0;

        case OSC_SQUARE25: // Square wave 25%
            return sin(dFreq) > 0.5 ? 1.0 : -1.0;    

        case OSC_SQUARE12: // Square wave 12%
            return sin(dFreq) > 0.75 ? 1.0 : -1.0;  

        case OSC_TRIANGLE: // Triangle wave between -1 and +1
            return asin(sin(dFreq)) * INV2PI;

        case OSC_SAW_DIG:
            return INV2PI * (dHertz * PI * fmod(dTime, 1.0 / dHertz) - HALF_PI);

        case OSC_NOISE:
            return 2.0 * ((FTYPE)rand() / (FTYPE)RAND_MAX) - 1.0;

        default:
            return 0.0;
	}
}


FTYPE env(const FTYPE dTime, envelope &env, const FTYPE dTimeOn, const FTYPE dTimeOff){
	return env.amplitude(dTime, dTimeOn, dTimeOff);
}

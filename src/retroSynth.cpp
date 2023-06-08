#include "retroSynth.h"
#include <cmath>
#include <iostream>
#include <random>

#define INV2PI  0.63661977 // 2.0 / PI
#define HALF_PI 1.57079632 // PI / 2.0
#define PI      3.14159565
#define PI2     6.28318530 // PI * 2.0

#define NOISE_SAMPLES 22050
uint8_t NOISE [NOISE_SAMPLES];
static FTYPE phase = 0.0;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int8_t> rnd(0, 255);

RetroSynth::RetroSynth()
{
    for (int i = 0; i < NOISE_SAMPLES; i++) 
    {
        NOISE[i] = rnd(gen);
    }
}

RetroSynth::~RetroSynth(){}

float RetroSynth::FrequencyFromNote(int midi_note) 
{
    return 440.0f * pow(2.0f, (midi_note - 69) / 12.0f);
}

float RetroSynth::RenderNote(int oscT, int note, float time, float timeOn, float lfoHertz, float lfoAmp) 
{
    float frequency = FrequencyFromNote(note);
    float value = osc(time - timeOn, frequency, oscT, lfoHertz, lfoAmp);
    return value;
}

float vibratoSpeed =  0.0f;
float vibratoAmplitude = 0.0f;
float vibratoPhase = 0.0f;

FTYPE RetroSynth::osc(const FTYPE dTime, FTYPE dHertz, const int nType, const FTYPE dLFOHertz, const FTYPE dLFOAmplitude) {

    float vibratoSpeed = powf(dLFOHertz, 2.0f)*0.01f;
    float vibratoAmplitude = dLFOAmplitude * 2.0f;

    if (vibratoAmplitude > 0.0f)
    {
        vibratoPhase += vibratoSpeed;
        dHertz = (float)(dHertz*(1.0 + sinf(vibratoPhase)*vibratoAmplitude));
    }

    return waveTable(dHertz, nType);

}

FTYPE RetroSynth::waveTable(float freq, uint8_t osc)
{   
    FTYPE increment = freq / 44100.0;
    int size = osc == 4 ? NOISE_SAMPLES : TABLE_SIZE;

    int i = static_cast<int>(phase * size) % size;
    int iN = static_cast<int>((phase * size) * increment) % size;
 
    phase += increment;

    uint8_t tableValue;
    
    switch (osc)
    {
        case 1:
            tableValue = PULSE_12[i];
            break;
        case 2:
            tableValue = PULSE_25[i];
            break;
        case 3:
            tableValue = TRIANGLE[i];
            break;
        case 4:
            tableValue = NOISE[iN];
            break;
        default:
            tableValue = SQUARE[i];
    }

    return ((tableValue / 127.0) - 1.0);
}

FTYPE env(const FTYPE dTime, envelope &env, const FTYPE dTimeOn, const FTYPE dTimeOff)
{
	return env.amplitude(dTime, dTimeOn, dTimeOff);
}

void RetroSynth::SetEnv(int channel, float attackTime, float decayTime,
                         float sustainAmplitude, float releaseTime, float startAmplitude)
{

    channels[channel].env.dAttackTime = attackTime;
    channels[channel].env.dDecayTime = decayTime;
    channels[channel].env.dSustainAmplitude = sustainAmplitude;
    channels[channel].env.dReleaseTime = releaseTime;
    channels[channel].env.dStartAmplitude = startAmplitude;
}
#include "retroSynth.h"
#include <cmath>
#include <iostream>
#include <random>

#define INV2PI  0.63661977 // 2.0 / PI
#define HALF_PI 1.57079632 // PI / 2.0
#define PI      3.14159565
#define PI2     6.28318530 // PI * 2.0

#define NOISE_SAMPLES 16000
uint8_t NOISE [NOISE_SAMPLES];
static FTYPE tablePosition = 0.0;  // Variable para el tiempo o posición en la tabla

// Converts frequency (Hz) to angular velocity
FTYPE w(const FTYPE dHertz) {
	return dHertz * PI2;
}

RetroSynth::RetroSynth()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dis1(128, 192);
    std::uniform_int_distribution<uint8_t> dis0(64, 128);
    std::uniform_int_distribution<int8_t> dis(0, 32);

    for (int i = 0; i < NOISE_SAMPLES; i++) 
    {
        //NOISE[i] = dis(gen);
        //int part = (i / NOISE_SAMPLES) * 64;
        //NOISE[i] = part % 2 == 0 ? dis1(gen) : dis0(gen);
        NOISE[i] = (sin(i * ((1.0/NOISE_SAMPLES) * PI2)) + 1.0) * 127;
    }

    for (int i = 0; i < NOISE_SAMPLES; i++) 
    {
        //NOISE[i] = dis(gen);
        //int part = (i / NOISE_SAMPLES) * 64;
        //NOISE[i] = part % 2 == 0 ? dis1(gen) : dis0(gen);
        NOISE[i] += dis(gen);
    }

}

RetroSynth::~RetroSynth(){
}

float RetroSynth::FrequencyFromNote(int midi_note) {
    return 440.0f * pow(2.0f, (midi_note - 69) / 12.0f);
}

float RetroSynth::RenderNote(int oscT, int note, float time, float timeOn, float lfoHertz, float lfoAmp) {
    float frequency = FrequencyFromNote(note);
    float value = osc(time - timeOn, frequency, oscT, lfoHertz, lfoAmp);
    return value;
}

FTYPE RetroSynth::osc(const FTYPE dTime, const FTYPE dHertz, const int nType, const FTYPE dLFOHertz, const FTYPE dLFOAmplitude) {

	//FTYPE dFreq = w(dHertz) * dTime + dLFOAmplitude * dHertz * (sin(w(dLFOHertz) * dTime));
	return waveTable(dHertz, nType);
    
}


FTYPE RetroSynth::waveTable(float freq, uint8_t osc)
{   
    FTYPE increment = freq / 44100.0;
    int size = osc == 4 ? NOISE_SAMPLES : TABLE_SIZE;

    int i = static_cast<int>(tablePosition * size) % size;
    tablePosition += increment;
    uint8_t tableValue = SQUARE[i];
    
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
            tableValue = NOISE[i];
            break;
    }

    return (tableValue / 127.0) - 1.0;
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

void RetroSynth::SetLFO(int channel, float lfoHertz, float lfoAmp)
{

    channels[channel].lfo.dLFOHertz = lfoHertz;
    channels[channel].lfo.dLFOAmplitude = lfoAmp;
}

void RetroSynth::SetOsc(int channel, float osc)
{
    //tablePosition = 0.0;
    channels[channel].osc = osc;
}
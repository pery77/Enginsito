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

FTYPE RetroSynth::FrequencyFromNote(int midi_note) 
{
    return 440.0f * pow(2.0f, (midi_note - 69) / 12.0f);
}

FTYPE RetroSynth::RenderNote(int channel, int oscT, int note) 
{
    FTYPE frequency = FrequencyFromNote(note);
    return osc(channel, frequency, oscT);
}

float fltp  = 0.0f;
float fltdp = 0.0f;

FTYPE RetroSynth::osc(const int channel, FTYPE dHertz, const int nType) 
{
    float slope = channels[channel].slide.slope;
    float curve = channels[channel].slide.curve;
    float phase = channels[channel].slide.phase;

    float vibratoAmplitude = channels[channel].lfo.amplitude * 2.0f;

    if (vibratoAmplitude > 0.0f)
    {
        channels[channel].lfo.phase += powf(channels[channel].lfo.freq, 1.2f) * 0.01f;;
        dHertz = (dHertz*(1.0 + sinf(channels[channel].lfo.phase) * vibratoAmplitude));
    }

    if(slope + curve != 0.0)
    {
        channels[channel].slide.phase = channels[channel].time;
        float fslide = 1.0 - pow(slope, 3.0)*0.001;
        float fdslide = -pow(curve, 3.0)*0.0001;
        fslide += fdslide;
        channels[channel].phase *= fslide;
    }

    if (dHertz < 20) dHertz = 20;
    if (dHertz > 20000) dHertz = 20000;

    double sample = waveTable(channel, dHertz, nType);

    float cutoff    = channels[channel].LPF.cutoff;
    float resonance = channels[channel].LPF.resonance;

    float fltw = powf(cutoff, 3.0f);
    float fltdmp = 1.0f/(1.0f + powf(resonance, 2.0f)*10.0f)*(0.01f + fltw);

    if (fltdmp > 0.8f) fltdmp = 0.8f;

    float pp = fltp;

    if (fltw < 0.0f) fltw = 0.0f;
    if (fltw > 0.1f) fltw = 0.1f;

    if (cutoff != 1.0f)  
    {
        fltdp += (sample-fltp)*fltw;
        fltdp -= fltdp*fltdmp;
        fltp += fltdp;
        sample = fltp;
    }

    if (sample > 1.) sample = 1.;
    if (sample < -1.) sample = -1.;

    return sample;
}

FTYPE RetroSynth::waveTable(int channel, float freq, uint8_t osc)
{   
    FTYPE increment = freq / 44100.0;
    int size = osc == 4 ? NOISE_SAMPLES : TABLE_SIZE;

    float phaseXsize = channels[channel].phase * size;

    int i = static_cast<int>(phaseXsize) % size;
    int iN = static_cast<int>(phaseXsize * increment) % size;
 
    channels[channel].phase += increment;

    //if (channels[channel].phase > 1000.f) channels[channel].phase = 0.0f;

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
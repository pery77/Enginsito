#include "retroSynth.h"
#include <cmath>
#include <iostream>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int8_t> rnd(0, 255);

RetroSynth::RetroSynth()
{
    SetChannelPreset(0,0);
    SetChannelPreset(1,0);
    SetChannelPreset(2,0);
    SetChannelPreset(3,0);
    
    for (int i = 0; i < NOISE_SAMPLES; i++) 
    {
        NOISE[i] = rnd(gen);
    }
}

RetroSynth::~RetroSynth(){}

inline double frequencyFromNote(int midi_note) 
{
    return 440.0f * pow(2.0f, (midi_note - 69) / 12.0f);
}

void RetroSynth::resetChannelPhase(uint8_t channel)
{
    channels[channel].phase      = 0.0;
    channels[channel].lfoPhase   = 0.0;
    channels[channel].slidePhase = 0.0;
    channels[channel].noteTotalTime       = 0.0;
}

double RetroSynth::renderChannel(uint8_t channel) 
{
    double dHertz = frequencyFromNote(channels[channel].note);
    float slope = channels[channel].preset->slide.slope;
    float curve = channels[channel].preset->slide.curve;

    float vibratoAmplitude = channels[channel].preset->lfo.depht * 2.0f;

    if (vibratoAmplitude > 0.0f)
    {
        channels[channel].lfoPhase += powf(channels[channel].preset->lfo.speed, 1.2f) * 0.01f;;
        dHertz = (dHertz*(1.0 + sinf(channels[channel].lfoPhase) * vibratoAmplitude));
    }

    if(slope + curve != 0.0)
    {
        channels[channel].slidePhase = channels[channel].noteTotalTime;
        float fslide  = 1.0 - pow(slope, 3.0)*0.001;
        float fdslide = -pow(curve, 3.0)*0.0001;
        fslide += fdslide;
        channels[channel].phase *= fslide;
    }

    if (dHertz < 20)    dHertz = 20;
    if (dHertz > 20000) dHertz = 20000;

    double sample = waveTable(channel, dHertz, channels[channel].preset->osc);

    float cutoff    = channels[channel].preset->LPF.cutoff;   
    if (cutoff != 1.0f)  
    {
        float resonance = channels[channel].preset->LPF.resonance;

        float fltw = powf(cutoff, 3.0f);
        float fltdmp = 1.0f/(1.0f + powf(resonance, 2.0f)*10.0f)*(0.01f + fltw);

        if (fltdmp > 0.8f) fltdmp = 0.8f;

        if (fltw < 0.0f) fltw = 0.0f;
        if (fltw > 0.1f) fltw = 0.1f;

        channels[channel].fltdp += (sample-channels[channel].fltp)*fltw;
        channels[channel].fltdp -= channels[channel].fltdp*fltdmp;
        channels[channel].fltp  += channels[channel].fltdp;
        sample = channels[channel].fltp;
    }

    if (sample > 1.) sample = 1.;
    if (sample < -1.) sample = -1.;

    return sample;
}

double RetroSynth::waveTable(uint8_t channel, float freq, uint8_t osc)
{   
    double increment = freq / SAMPLERATE;
    int size = osc == 4 ? NOISE_SAMPLES : TABLE_SIZE;

    float phaseXsize = channels[channel].phase * size;

    int i = static_cast<int>(phaseXsize) % size;
    int iN = static_cast<int>(phaseXsize * increment) % size;
 
    channels[channel].phase += increment;

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

void RetroSynth::SetChannelPreset(uint8_t channel, uint8_t preset)
{
    channels[channel].preset = &presets[preset];
    resetChannelPhase(channel);
}

void RetroSynth::AudioInputCallback(void* buffer, unsigned int frames) 
{
    short* bufferData = (short*)buffer;
    for (int frame = 0; frame < frames; frame++)
    {
        float samples[NUM_CHANNELS] = {0};
        float mixedSample = 0;
        int channelPlaying = 0;

        for (int track = 0; track < NUM_CHANNELS; track++) 
        {
            float amplitude = 
                channels[track].preset->env.amplitude(channels[track].sequenceTime, channels[track].noteTimeOn, channels[track].noteTimeOff);

            samples[track] = 0;

            if (amplitude > 0.0001) 
            {
                samples[track] += renderChannel(track);
                samples[track] *= channels[track].volume * amplitude;

                samples[track] *= 32767.0;
                
                channelPlaying++;
                channels[track].noteTotalTime += steps;
            }
            else
            {
                resetChannelPhase(track);
            }

            if (channelPlaying > 0)
            {
                mixedSample += samples[track] / channelPlaying;
            }
            
            channels[track].sequenceTime += steps;
        }

        bufferData[frame] = mixedSample;
    }
}
#include "retroSynth.h"
#include <cmath>
#include <iostream>
#include <random>
#include "audio_manager.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int8_t> rnd(0, 255);

RetroSynth::RetroSynth(AudioManager* _audioManagerRef)
{
    audioManagerRef = _audioManagerRef;
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
    channels[channel].phase         = 0.0;
    channels[channel].lfoPhase      = 0.0;
    channels[channel].slidePhase    = 0.0;
    channels[channel].noteTotalTime = 0.0;
}

double RetroSynth::renderChannel(uint8_t channel) 
{
    uint8_t currentPreset = channels[channel].currentPreset;
    uint8_t osc = audioManagerRef->GetOSC(currentPreset);

    double dHertz = frequencyFromNote(channels[channel].note);
    float slope = audioManagerRef->GetSlope(currentPreset);
    float curve = audioManagerRef->GetCurve(currentPreset);

    float vibratoAmplitude = audioManagerRef->GetLFODepth(currentPreset) * 2.0f;

    if (vibratoAmplitude > 0.0f)
    {
        float lfoSpeed = audioManagerRef->GetLFOSpeed(currentPreset);

        channels[channel].lfoPhase += powf(lfoSpeed, 1.2f) * 0.01f;;
        dHertz = (dHertz*(1.0 + sinf(channels[channel].lfoPhase) * vibratoAmplitude));
    }

    if(slope + curve != 0.0)
    {
        channels[channel].slidePhase = channels[channel].noteTotalTime;
        float fslide  = 1.0 - pow(slope, 3.0) * 0.001;
        float fdslide = -pow(curve, 3.0)* 0.0001;
        fslide += fdslide;
        channels[channel].phase *= fslide;
        //dHertz += fslide * channels[channel].slidePhase;

    }

    if (dHertz < 20)    dHertz = 20;
    if (dHertz > 20000) dHertz = 20000;

    double sample = waveTable(channel, dHertz, osc);

    float cutoff = audioManagerRef->GetCut(currentPreset);   
    if (cutoff != 1.0f)  
    {
        float resonance = audioManagerRef->GetRes(currentPreset);

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
    int size = osc == 5 ? NOISE_SAMPLES : TABLE_SIZE;

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
            tableValue = SAW[i];
            break;    
        case 5:
            tableValue = NOISE[iN];
            break;
        default:
            tableValue = SQUARE[i];
    }

    return ((tableValue / 127.0) - 1.0);
}

void RetroSynth::SetChannelPreset(uint8_t channel, uint8_t preset)
{
    channels[channel].currentPreset = preset;
    resetChannelPhase(channel);
}

void RetroSynth::AudioInputCallback(void* buffer, unsigned int frames) 
{
    short* bufferData = (short*)buffer;

    for (int frame = 0; frame < frames; frame++)
    {
        float samples[NUM_CHANNELS] = {0};
        int channelPlaying = 0;
        float mixedSample = 0.0;
        float maxAbsValue = 0.0;

        for (int track = 0; track < NUM_CHANNELS; track++) 
        {
            float amp = amplitude(channels[track].currentPreset, channels[track].sequenceTime, 
                                        channels[track].noteTimeOn, channels[track].noteTimeOff);

            samples[track] = 0;

            if (amp > 0.0001) 
            {
                samples[track] = renderChannel(track) * channels[track].volume * amp;
                
                channelPlaying++;
                channels[track].noteTotalTime += steps;
            }
            else
            {
                resetChannelPhase(track);
            }

            channels[track].sequenceTime += steps;
            channels[track].frame[frame] = ((samples[track] + 1.0f) * 127);
            
            mixedSample += samples[track];
            
            if (fabs(samples[track]) > maxAbsValue)
            {
                maxAbsValue = fabs(samples[track]);
            }
        }

        float scale = 1.0;
        if (maxAbsValue > 0.0)
        {
            scale = 1.0f / maxAbsValue;
        }

        for (int track = 0; track < NUM_CHANNELS; track++) 
        {
            samples[track] *= scale;
        }

        bufferData[frame] = mixedSample * 32767.0;

    }
}

uint8_t RetroSynth::GetFrameAverage(uint8_t channel, uint8_t frame)
{
    if (frame > 62) frame = 62;
    float suma = 0;

    for (int i = 0; i < 7; i++) 
    { 
        suma += channels[channel].frame[(frame * 7) + i];
    }

    suma /= 7;
    return suma;
}

double RetroSynth::amplitude(uint8_t preset, const double dTime, const double dTimeOn, const double dTimeOff)
{
    double Attack  = audioManagerRef->GetEnvA(preset);
	double Decay   = audioManagerRef->GetEnvD(preset);
	double Sustain = audioManagerRef->GetEnvS(preset);
	double Release = audioManagerRef->GetEnvR(preset);
	double Amplitude = 1.0;

	double dAmplitude = 0.0;
    double dReleaseAmplitude = 0.0;

	if (dTimeOn > dTimeOff) 
	{ // Note is on
		double dLifeTime = dTime - dTimeOn;

		if (dLifeTime <= Attack)
			dAmplitude = (dLifeTime / Attack);
		else if (dLifeTime <= (Attack + Decay))
			dAmplitude = ((dLifeTime - Attack) / Decay) * (Sustain - Amplitude) + Amplitude;
		else
			dAmplitude = Sustain;
    } 
	else 
	{ // Note is off
		double dLifeTime = dTimeOff - dTimeOn;

		if (dLifeTime <= Attack)
			dReleaseAmplitude = (dLifeTime / Attack);
		else if (dLifeTime <= (Attack + Decay))
			dReleaseAmplitude = ((dLifeTime - Attack) / Decay) * (Sustain - Amplitude) + Amplitude;
		else
			dReleaseAmplitude = Sustain;
		dAmplitude = ((dTime - dTimeOff) / Release) * (0.0 - dReleaseAmplitude) + dReleaseAmplitude;
    }

	if (dAmplitude <= 0.00001) dAmplitude = 0.0;
	if (dAmplitude > 0.9)      dAmplitude = 0.9;

	return dAmplitude;
}
#include "audio_manager.h"
#include "engine.h"

#include <stdio.h>
#include <cstring>
#include <cmath>
#include <iostream>

MMLParser* mml[TRACK_COUNT];
RetroSynth* synth;

AudioStream stream;
Engine* audioEngineRef;

bool isPlaying = false;

void mmlCallback(MMLEvent event, int channel, int program, int note, int volume, AudioManager* au)
{
    switch (event) 
    {
        case MML_NOTE_ON:
            au->PlayNote(channel, note, volume);
            break;
        case MML_NOTE_OFF:
            au->StopNote(channel);
            break;
        case MML_PROGRAM_CHANGE:
            au->SetChannelPreset(channel, program);
            break; 
    }
}

AudioManager::AudioManager(Engine* _engine)
{
    audioEngineRef = _engine;

    SetAudioStreamBufferSizeDefault(MAX_SAMPLES_PER_UPDATE);
    stream = LoadAudioStream(SAMPLE_RATE, SAMPLE_SIZE, CHANNELS);
    SetAudioStreamCallback(stream, [](void* buffer, unsigned int frames) { synth->AudioInputCallback(buffer, frames);});

    PlayAudioStream(stream);
   
    for (int i = 0; i < TRACK_COUNT; i++) 
    {
        mml[i] = new MMLParser(this);
        mml[i]->setChannel(i);
        mml[i]->setCallback(mmlCallback);
        sequence[i] = "";
    }

    synth = new RetroSynth(this);
}

AudioManager::~AudioManager(){}

//Sequencer
void AudioManager::Update()
{
    for (int i = 0; i < TRACK_COUNT; i++) 
    {
        if (mml[i]->isPlaying()) 
            mml[i]->update(synth->channels[i].tick++);
    }
}

void AudioManager::SetSequence(uint8_t id, const char* newSequence)
{
    if (id > TRACK_COUNT) id = TRACK_COUNT;
    sequence[id] = newSequence;
}

const char* AudioManager::GetSequence(uint8_t id)
{
    return sequence[id];
}

void AudioManager::PlayNote(uint8_t channel, uint8_t note, uint8_t volume)
{
    if (channel > TRACK_COUNT) return;
    printf("[CHAN: %i] %i\n", channel ,note);
    synth->channels[channel].note    = note;
    synth->channels[channel].volume  = volume * 0.007874; // 1/127
    synth->channels[channel].noteTimeOn  = synth->channels[channel].sequenceTime;
    synth->channels[channel].noteTimeOff = 0;
}

void AudioManager::StopNote(uint8_t channel)
{
    if (channel > TRACK_COUNT) return;
    printf("[STOP NOTE CHAN: %i]\n", channel);
    synth->channels[channel].noteTimeOff = synth->channels[channel].sequenceTime;
}

void AudioManager::SetChannelPreset(uint8_t channel, uint8_t preset)
{
    printf("[Program: %i in CH: %i]\n", preset, channel);
    synth->SetChannelPreset(channel, preset);
}

void AudioManager::ChannelPlay(uint8_t channel, const char* newSequence)
{
    SetSequence(channel, newSequence);
    ChannelPlay(channel);
}

void AudioManager::ChannelPlay(uint8_t channel)
{
    synth->channels[channel].tick = 0;

    size_t lenght = strlen(sequence[channel]);
    if (lenght > 2)
    {
        mml[channel]->play(sequence[channel], false);
        synth->channels[channel].isPlaying = true;   
    }
}

void AudioManager::ChannelStop(uint8_t channel)
{
    synth->channels[channel].tick = 0;
    synth->channels[channel].isPlaying = false;

    mml[channel]->stop();
}

void AudioManager::StopAll()
{
    for (uint8_t i = 0; i < TRACK_COUNT; i++)
    {
        StopNote(i);
        ChannelStop(i);
        SetSequence(i, "");
    }
    
}

unsigned int AudioManager::GetChannelPosition(uint8_t channel)
{
    return mml[channel]->getTotalSteps();
}

unsigned int AudioManager::GetChannelSize(uint8_t channel)
{
    return mml[channel]->getSize();
}

//RetroSynth
RetroSynth* AudioManager::GetSynth()
{
    return synth;
}

uint8_t AudioManager::GetOSC(uint8_t preset)
{
    unsigned short dir = GetSoundDir(preset);
    return audioEngineRef->Peek(dir);
}

float AudioManager::GetEnvA(uint8_t preset)
{
    unsigned short dir = GetSoundDir(preset);
    return audioEngineRef->Peek(dir + 1) / 255.0f;
}

float AudioManager::GetEnvD(uint8_t preset)
{
    unsigned short dir = GetSoundDir(preset);
    return audioEngineRef->Peek(dir + 2) / 255.0f;

}

float AudioManager::GetEnvS(uint8_t preset)
{
    unsigned short dir = GetSoundDir(preset);
    return audioEngineRef->Peek(dir + 3) / 255.0f;

}

float AudioManager::GetEnvR(uint8_t preset)
{
    unsigned short dir = GetSoundDir(preset);
    return audioEngineRef->Peek(dir + 4) / 255.0f;
}

float AudioManager::GetLFOSpeed(uint8_t preset)
{
    unsigned short dir = GetSoundDir(preset);
    return audioEngineRef->Peek(dir + 5) / 255.0;
}

float AudioManager::GetLFODepth(uint8_t preset)
{
    unsigned short dir = GetSoundDir(preset);
    return audioEngineRef->Peek(dir + 6)  / 255.0;
}

float AudioManager::GetCut(uint8_t preset)
{
    unsigned short dir = GetSoundDir(preset);
    return audioEngineRef->Peek(dir + 7) / 255.0f;
}

float AudioManager::GetRes(uint8_t preset)
{
    unsigned short dir = GetSoundDir(preset);
    return audioEngineRef->Peek(dir + 8) / 255.0f;
}

float AudioManager::GetSlope(uint8_t preset)
{
    unsigned short dir = GetSoundDir(preset);
    return audioEngineRef->Peek(dir + 9) / 127.0f - 1.0f;
}

float AudioManager::GetCurve(uint8_t preset)
{
    unsigned short dir = GetSoundDir(preset);
    return audioEngineRef->Peek(dir + 10) / 127.0f - 1.0f;
}

void AudioManager::SetOSC(uint8_t preset, uint8_t osc)
{

    unsigned short dir = GetSoundDir(preset);
    audioEngineRef->Poke(dir, osc);
}

void AudioManager::SetEnv(uint8_t preset, uint8_t attackTime, uint8_t decayTime,
                         uint8_t sustainAmplitude, uint8_t releaseTime)
{
    unsigned short dir = GetSoundDir(preset);
    audioEngineRef->Poke(dir + 1, attackTime);
    audioEngineRef->Poke(dir + 2, decayTime);
    audioEngineRef->Poke(dir + 3, sustainAmplitude);
    audioEngineRef->Poke(dir + 4, releaseTime);
}
void AudioManager::SetLFO(uint8_t preset, uint8_t speed, uint8_t depth)
{
    unsigned short dir = GetSoundDir(preset);
    audioEngineRef->Poke(dir + 5, speed);
    audioEngineRef->Poke(dir + 6, depth);
}

void AudioManager::SetFilter(uint8_t preset, uint8_t cutoff, uint8_t resonance)
{
    unsigned short dir = GetSoundDir(preset);
    audioEngineRef->Poke(dir + 7, cutoff);
    audioEngineRef->Poke(dir + 8, resonance);
}

void AudioManager::SetSlide(uint8_t preset, uint8_t slope, uint8_t curve)
{
    unsigned short dir = GetSoundDir(preset);
    audioEngineRef->Poke(dir + 9, slope);
    audioEngineRef->Poke(dir + 10, curve);
}

unsigned short AudioManager::GetSoundDir(uint8_t id)
{
    return (3376 + (id * 11));
}
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
            printf("[CHAN: %i] %i\n", channel ,note);
            au->PlayNote(channel, note, volume);
            break;
        case MML_NOTE_OFF:
            au->StopNote(channel);
            break;
        case MML_PROGRAM_CHANGE:
            printf("[Program: %i]\n", program);
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

    synth = new RetroSynth();
    InitializePresets();
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
    if (channel >= TRACK_COUNT) return;
    synth->channels[channel].note    = note;
    synth->channels[channel].volume  = volume * 0.007874; // 1/127
    synth->channels[channel].noteTimeOn  = synth->channels[channel].sequenceTime;
    synth->channels[channel].noteTimeOff = 0;
}

void AudioManager::StopNote(uint8_t channel)
{
    if (channel >= TRACK_COUNT) return;
    synth->channels[channel].noteTimeOff = synth->channels[channel].sequenceTime;
}

void AudioManager::SetChannelPreset(uint8_t channel, uint8_t preset)
{
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
    //synth->channels[channel].sequenceTime = 0.0;
    
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
    //synth->channels[channel].sequenceTime = 0.0;
    //synth->channels[channel].noteTimeOn  = 0.0;
    //synth->channels[channel].noteTimeOff = 0.0;

    mml[channel]->stop();
}

void AudioManager::StopAll()
{
    for (uint8_t i = 0; i < TRACK_COUNT; i++)
    {
        ChannelStop(i);
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
void AudioManager::InitializePresets()
{
    for (uint8_t i = 0; i < NUM_PRESETS; i++)
    {
        unsigned short dir = GetSoundDir(i);

        SetOSC   (i, audioEngineRef->Peek(dir));
        SetEnv   (i, audioEngineRef->Peek(dir + 1), audioEngineRef->Peek(dir + 2), audioEngineRef->Peek(dir + 3), audioEngineRef->Peek(dir + 4));
        SetLFO   (i, audioEngineRef->Peek(dir + 5), audioEngineRef->Peek(dir + 6));
        SetFilter(i, audioEngineRef->Peek(dir + 7), audioEngineRef->Peek(dir + 8));
        SetSlide (i, audioEngineRef->Peek(dir + 9), audioEngineRef->Peek(dir + 10));
    }
    
}
void AudioManager::SetOSC(uint8_t preset, uint8_t osc)
{
    synth->presets[preset].osc = osc;

    unsigned short dir = GetSoundDir(preset);
    audioEngineRef->Poke(dir, osc);
}

void AudioManager::SetEnv(uint8_t preset, uint8_t attackTime, uint8_t decayTime,
                         uint8_t sustainAmplitude, uint8_t releaseTime)
{
    synth->presets[preset].env.Attack  = attackTime       / 255.0;
    synth->presets[preset].env.Decay   = decayTime        / 255.0;
    synth->presets[preset].env.Sustain = sustainAmplitude / 255.0;
    synth->presets[preset].env.Release = releaseTime      / 255.0;

    unsigned short dir = GetSoundDir(preset);
    audioEngineRef->Poke(dir + 1, attackTime);
    audioEngineRef->Poke(dir + 2, decayTime);
    audioEngineRef->Poke(dir + 3, sustainAmplitude);
    audioEngineRef->Poke(dir + 4, releaseTime);
}

void AudioManager::SetLFO(uint8_t preset, uint8_t speed, uint8_t depth)
{
    synth->presets[preset].lfo.speed = speed / 255.0;
    synth->presets[preset].lfo.depht = depth  / 255.0;

    unsigned short dir = GetSoundDir(preset);
    audioEngineRef->Poke(dir + 5, speed);
    audioEngineRef->Poke(dir + 6, depth);
}

void AudioManager::SetFilter(uint8_t preset, uint8_t cutoff, uint8_t resonance)
{
    synth->presets[preset].LPF.cutoff    = cutoff    / 255.0f;
    synth->presets[preset].LPF.resonance = resonance / 255.0f;

    unsigned short dir = GetSoundDir(preset);
    audioEngineRef->Poke(dir + 7, cutoff);
    audioEngineRef->Poke(dir + 8, resonance);
}

void AudioManager::SetSlide(uint8_t preset, uint8_t slope, uint8_t curve)
{
    synth->presets[preset].slide.slope = slope / 127.0f - 1.0f;
    synth->presets[preset].slide.curve = curve / 127.0f - 1.0f;

    unsigned short dir = GetSoundDir(preset);
    audioEngineRef->Poke(dir + 9, slope);
    audioEngineRef->Poke(dir + 10, curve);
}

unsigned short AudioManager::GetSoundDir(uint8_t id)
{
    return (3376 + (id * 11));
}
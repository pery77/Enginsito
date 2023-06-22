#pragma once
#include "../../libs/raylib/include/raylib.h"

#include "rfxgen.h"
#include <math.h>
#include "MMLParser.h"
#include "tools.h"
#include "retroSynth.h"

#define SAMPLE_RATE               44100
#define SAMPLE_SIZE               16
#define CHANNELS                  1
#define MAX_SAMPLES               512
#define MAX_SAMPLES_PER_UPDATE    2048
#define MAX_WAVE_SLOTS            16

#define AUDIO_STEP                0.00390625 // 1. / 256.
#define TRACK_COUNT               4

class Engine;

class AudioManager 
{
    
    public:
    AudioManager(Engine* _engine);
    ~AudioManager();

    WaveParams params[MAX_WAVE_SLOTS] = { 0 }; // Wave parameters for generation
    Wave wave[MAX_WAVE_SLOTS] = { 0 };
    Sound sound[MAX_WAVE_SLOTS] = { 0 };

    void Update();
    int voice = 0;
    void SetSequence(uint8_t id, const char* newSequence);
    const char* GetSequence(uint8_t id);

    void PlayNote(uint8_t channel, uint8_t note, uint8_t volume);
    void StopNote(uint8_t channel);
    void MusicPlay();
    void MusicStop();
    
    unsigned int GetMusicPosition(uint8_t channel);
    unsigned int GetMusicSize(uint8_t channel);

    void SetEnv(uint8_t preset, uint8_t attackTime, uint8_t decayTime, uint8_t sustainAmplitude, uint8_t releaseTime);
	void SetLFO(uint8_t preset, uint8_t lfoHertz, uint8_t lfoAmp);
	void SetOSC(uint8_t preset, uint8_t osc);
    void SetFilter(uint8_t preset, uint8_t cutoff, uint8_t resonance);
    void SetSlide(uint8_t preset, uint8_t slope, uint8_t curve);
    
    void SetChannelPreset(uint8_t channel, uint8_t preset);

    unsigned short GetSoundDir(uint8_t id);

    bool MusicIsPlaying = false;
    unsigned int AudioTick = 0;

    RetroSynth* GetSynth();

    private:
    const char* sequence[TRACK_COUNT + 1];
    void setNote(uint8_t id, uint8_t note);
};
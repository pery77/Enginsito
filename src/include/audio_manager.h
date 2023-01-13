#pragma once
#include "../../libs/raylib/include/raylib.h"

#include "rfxgen.h"
#include "tsf.h"

#define SAMPLERATE               44100
#define SAMPLESIZE               16
#define CHANNELS                  2
#define MAX_SAMPLES               512
#define MAX_SAMPLES_PER_UPDATE    4096
#define MAX_WAVE_SLOTS            32
#define WAIT_TICKS                30

class AudioManager {
    public:

    AudioManager();
    ~AudioManager();

    WaveParams params[MAX_WAVE_SLOTS] = { 0 }; // Wave parameters for generation
    Wave wave[MAX_WAVE_SLOTS] = { 0 };
    Sound sound[MAX_WAVE_SLOTS] = { 0 };

    //tsf* ptsf = tsf_load_filename("assets/8bit.sf2");
    //tsf* ptsf = tsf_load_filename("assets/ins.sf2");
    //tsf* ptsf = tsf_load_filename("assets/florestan-subset.sf2");

    void Update();

    void SetSequence(const char* newSequence);
    const char* GetSequence();

    void PlayNote(int note, int voice, int volume);
    void Stop();
    void GetPresets();

    private:
    const char* sequence = "";
    int audioTick = 0;

};
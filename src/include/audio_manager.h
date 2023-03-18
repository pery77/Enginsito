#pragma once
#include "../../libs/raylib/include/raylib.h"

#include "rfxgen.h"
#include "tsf.h"
#include <math.h>
#include "MMLParser.h"
#include "tools.h"

#define SAMPLERATE               22050
#define SAMPLESIZE               16
#define CHANNELS                  2
#define MAX_SAMPLES               512
#define MAX_SAMPLES_PER_UPDATE    4096
#define MAX_WAVE_SLOTS            16

#define AUDIO_STEP                 0.00390625
#define TRACK_COUNT                4

class AudioManager {
    
    public:
    AudioManager();
    ~AudioManager();

    WaveParams params[MAX_WAVE_SLOTS] = { 0 }; // Wave parameters for generation
    Wave wave[MAX_WAVE_SLOTS] = { 0 };
    Sound sound[MAX_WAVE_SLOTS] = { 0 };

    //tsf* ptsf = tsf_load_filename("assets/8bit.sf2");

    void Update();
    int voice = 0;
    void SetSequence(unsigned char id, const char* newSequence);
    const char* GetSequence(unsigned char id);

    void PlayNote(int note, int voice, int volume);
    void StopNote(int note, int voice);
    void MusicPlay();
    void MusicStop();
    void GetTSFPresets();
    
    void SFXPlay(unsigned char id, unsigned char vol = 127);
    void SFXRender(unsigned char id, unsigned char note);
    void SFXWave(unsigned char id, unsigned char waveType);
    void SFXEnv(unsigned char id, unsigned char att, unsigned char susT, unsigned char susP, unsigned char dec);
    void SFXFreq(unsigned char id, unsigned char slide, unsigned char delta, unsigned char vibratoD, unsigned char vibratoS);
    void SFXTone(unsigned char id, unsigned char amount, unsigned char speed, unsigned char square, unsigned char duty);
    void SFXRepeat(unsigned char id, unsigned char speed, unsigned char offset, unsigned char sweep);
    void SFXFilter(unsigned char id, unsigned char lpfCutoff, unsigned char lpfSweep, unsigned char lpfRes, unsigned char hpfCutoff, unsigned char hpfSweep);
    
    void LoadSoundData(unsigned char id);

    unsigned short GetSoundDir(unsigned char id);

    private:
    int audioTick = 0;
    const char* sequence[TRACK_COUNT];
    void setNote(unsigned char id, unsigned char note);
};
#pragma once
#include "../../libs/raylib/include/raylib.h"

#include "rfxgen.h"
#include "tsf.h"
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
    void SetSequence(uint8_t id, const char* newSequence);
    const char* GetSequence(uint8_t id);

    void PlayNote(uint8_t channel, uint8_t note, uint8_t volume);
    void StopNote(uint8_t channel);
    void MusicPlay();
    void MusicStop();
    void GetTSFPresets();
    unsigned int GetMusicPosition(uint8_t channel);
    unsigned int GetMusicSize(uint8_t channel);

    void SetEnv(uint8_t channel, uint8_t attackTime, uint8_t decayTime, uint8_t sustainAmplitude, uint8_t releaseTime, uint8_t dStartAmplitude);
	void SetLFO(uint8_t channel, uint8_t lfoHertz, uint8_t lfoAmp);
	void SetOSC(uint8_t channel, uint8_t osc);
    
    void SFXPlay(uint8_t id, uint8_t vol = 127);
    void SFXStop(uint8_t id);
    void SFXRender(uint8_t id, uint8_t note);
    void SFXWave(uint8_t id, uint8_t waveType);
    void SFXEnv(uint8_t id, uint8_t att, uint8_t susT, uint8_t susP, uint8_t dec);
    void SFXFreq(uint8_t id, uint8_t slide, uint8_t delta, uint8_t vibratoD, uint8_t vibratoS);
    void SFXTone(uint8_t id, uint8_t amount, uint8_t speed, uint8_t square, uint8_t duty);
    void SFXRepeat(uint8_t id, uint8_t speed, uint8_t offset, uint8_t sweep);
    void SFXFilter(uint8_t id, uint8_t lpfCutoff, uint8_t lpfSweep, uint8_t lpfRes, uint8_t hpfCutoff, uint8_t hpfSweep);
    
    void LoadSoundData(uint8_t id);

    unsigned short GetSoundDir(uint8_t id);

    bool MusicIsPlaying = false;
    unsigned int AudioTick = 0;

    private:
    const char* sequence[TRACK_COUNT + 1];
    void setNote(uint8_t id, uint8_t note);
};
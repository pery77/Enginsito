#pragma once

#define MAX_VOICES 4

typedef struct {
    bool noteOn = false;
    int note = 69;
    float volume = 0.5;
    float time;
} Voice;

class RetroSynth{

    public:
    RetroSynth();
    ~RetroSynth();

    float SineWave(int note, float time);

    Voice voices[MAX_VOICES] = {0};

    private:
    float frequencyFromNote(int midi_note);

};
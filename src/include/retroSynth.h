#pragma once

#define MAX_VOICES 4
#define FTYPE float

typedef struct {
    bool noteOn = false;
    int osc = 0;
    int note = 69;
    float volume = 0.5;
    float time;
} Voice;

const int OSC_SINE = 0;
const int OSC_SQUARE = 1;
const int OSC_TRIANGLE = 2;
const int OSC_SAW_DIG = 3;
const int OSC_NOISE = 4;

class RetroSynth{

    public:
    RetroSynth();
    ~RetroSynth();

    float RenderNote(int oscT, int note, float time);

    Voice voices[MAX_VOICES] = {0};

    private:
    float frequencyFromNote(int midi_note);
    FTYPE osc(const FTYPE dTime, const FTYPE dHertz, const int nType = OSC_SINE, const FTYPE dLFOHertz = 0.0, const FTYPE dLFOAmplitude = 0.0, FTYPE dCustom = 50.0);

};
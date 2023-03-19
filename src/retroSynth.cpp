#include "retroSynth.h"
#include <cmath>

#define PI 3.14159565
#define PI2 6.2831853

RetroSynth::RetroSynth(){
}
RetroSynth::~RetroSynth(){
}

float RetroSynth::frequencyFromNote(int midi_note) {
    return 440.0f * pow(2.0f, (midi_note - 69) / 12.0f);
}

float RetroSynth::SineWave(int note, float time) {
    float frequency = frequencyFromNote(note);
    float value = sin(PI2 * frequency * time);
    return value;
}
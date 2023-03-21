#include "audio_manager.h"
#include <stdio.h>
#include <cstring>
#include <cmath>

tsf* ptsf;
MMLParser* mml[TRACK_COUNT];
RetroSynth* synth;

AudioStream stream;

float steps = 1.0f / (float)SAMPLE_RATE;
double musicTime = 0.0;
bool isPlaying = false;

void audioInputCallback(void *buffer, unsigned int frames) {

    short *bufferData = (short*) buffer;

    for (int i = 0; i < frames; i++) {

        short samples[TRACK_COUNT] = {0};
        short mixedSample = 0;
        
        for (int j = 0; j < TRACK_COUNT; j++) {

            float amplitude = synth->voices[j].env.amplitude(musicTime, synth->voices[j].timeOn, 
                                                                synth->voices[j].timeOff);
            if (amplitude > 0.1) {
                samples[j] += synth->RenderNote(synth->voices[j].osc, synth->voices[j].note, musicTime) 
                                            * synth->voices[j].volume * 32000.0 * amplitude;
            }
            else{
                samples[j] = 0;
            }
        }

        for (int j = 0; j < TRACK_COUNT; j++) {
            mixedSample += samples[j] * 0.25;
        }

        bufferData[i] = mixedSample;
        musicTime += steps;
    }
}

void mmlCallback(MMLEvent event, int channel, int osc, int note, int volume, AudioManager* au){
    switch (event) {
        case MML_NOTE_ON:
            au->PlayNote(channel, osc, note, volume);
            break;
        case MML_NOTE_OFF:
            au->StopNote(channel);
            break;
        case MML_PROGRAM_CHANGE:
            printf("\n[OSC:%d]\n", osc);
            break; 
    }
}

AudioManager::AudioManager(){

    for (int i = 0; i < MAX_WAVE_SLOTS; i++){
        LoadSoundData(i);

        // Default wave values
        wave[i].sampleRate = RFXGEN_GEN_SAMPLE_RATE;
        wave[i].sampleSize = RFXGEN_GEN_SAMPLE_SIZE;
        wave[i].channels = RFXGEN_GEN_CHANNELS;
        wave[i].frameCount = wave[i].sampleRate;

        wave[i].data = GenerateWave(params[i], &wave[i].frameCount);
        sound[i] = LoadSoundFromWave(wave[i]);
    }
   
    SetAudioStreamBufferSizeDefault(MAX_SAMPLES_PER_UPDATE);
    stream = LoadAudioStream(SAMPLE_RATE, SAMPLE_SIZE, CHANNELS);
    SetAudioStreamCallback(stream, audioInputCallback);

    PlayAudioStream(stream);
   
    for (int i = 0; i < TRACK_COUNT; i++) {
        mml[i] = new MMLParser(this);
        mml[i]->setChannel(i);
        mml[i]->setCallback(mmlCallback);
        sequence[i] = "";
    }

    synth = new RetroSynth();
}

AudioManager::~AudioManager(){}

//Music
void AudioManager::Update(){

    MusicIsPlaying = false;
    for (int i = 0; i < TRACK_COUNT; i++) {
        if (mml[i]->isPlaying()) {
            MusicIsPlaying = true;
            mml[i]->update(AudioTick);
        }
    }
    isPlaying = MusicIsPlaying;
    if (MusicIsPlaying) AudioTick++;
}

void AudioManager::SetSequence(uint8_t id, const char* newSequence){
    if (id > TRACK_COUNT - 1) 
        id = TRACK_COUNT - 1;
    sequence[id] = newSequence;
    AudioTick = 0;
}
const char* AudioManager::GetSequence(uint8_t id){
        return sequence[id];
}

void AudioManager::PlayNote(uint8_t channel, uint8_t osc, uint8_t note, uint8_t volume){
    synth->voices[channel].osc = osc;
    synth->voices[channel].note = note;
    synth->voices[channel].volume = volume * 0.007874; // 1/127
    synth->voices[channel].timeOn = musicTime;
    synth->voices[channel].timeOff = 0;
}
void AudioManager::StopNote(uint8_t channel){
    synth->voices[channel].timeOff = musicTime;
}
void AudioManager::MusicPlay(){
    AudioTick = 0;
    musicTime = 0;
    for (int i = 0; i < TRACK_COUNT; i++) {
        size_t lenght = strlen(sequence[i]);
        if (lenght > 2){
            printf("Playing #%d '%s'\n", i, sequence[i]);
            mml[i]->play(sequence[i], false);
            MusicIsPlaying = true;
        }    
    }
}
void AudioManager::MusicStop(){
    AudioTick = 0;
    musicTime = 0;
    for (int i = 0; i < TRACK_COUNT; i++) {
        mml[i]->stop();
        MusicIsPlaying = false;
        synth->voices[i].timeOn = -999;
        synth->voices[i].timeOff = -1;
    }
}
unsigned int AudioManager::GetMusicPosition(uint8_t channel){
    return mml[channel]->getTotalSteps();
}
unsigned int AudioManager::GetMusicSize(uint8_t channel){
    return mml[channel]->getSize();
}
//Effects
void AudioManager::SFXRender(uint8_t id, uint8_t note){

    setNote(id, note);

    wave[id].data = GenerateWave(params[id], &wave[id].frameCount);
    sound[id] = LoadSoundFromWave(wave[id]);

    unsigned short dir = GetSoundDir(id);
    Tools::Poke(dir + 21, note);
}
void AudioManager::SFXWave(uint8_t id, uint8_t waveType){

    params[id].waveTypeValue = waveType;
    unsigned short dir = GetSoundDir(id);
    Tools::Poke(dir, waveType);
}
void AudioManager::SFXEnv(uint8_t id, uint8_t att, uint8_t susT, uint8_t susP, uint8_t dec){

    params[id].attackTimeValue   = (float) (att * AUDIO_STEP);
    params[id].sustainTimeValue  = (float) (susT * AUDIO_STEP);
    params[id].sustainPunchValue = (float) (susP * AUDIO_STEP);
    params[id].decayTimeValue    = (float) (dec * AUDIO_STEP);
    
    unsigned short dir = GetSoundDir(id);
    Tools::Poke(dir + 1, att);
    Tools::Poke(dir + 2, susT);
    Tools::Poke(dir + 3, susP);
    Tools::Poke(dir + 4, dec);
}
void AudioManager::SFXFreq(uint8_t id, uint8_t slide, uint8_t delta, uint8_t vibratoD, uint8_t vibratoS){

    params[id].slideValue        = (float) (Tools::ToSigned(slide) * AUDIO_STEP) * 2;
    params[id].deltaSlideValue   = (float) (Tools::ToSigned(delta) * AUDIO_STEP) * 2;
    params[id].vibratoDepthValue = (float) (vibratoD * AUDIO_STEP);
    params[id].vibratoSpeedValue = (float) (vibratoS * AUDIO_STEP);

    unsigned short dir = GetSoundDir(id);
    Tools::Poke(dir + 5, slide);
    Tools::Poke(dir + 6, delta);
    Tools::Poke(dir + 7, vibratoD);
    Tools::Poke(dir + 8, vibratoS);
  
}
void AudioManager::SFXTone(uint8_t id, uint8_t amount, uint8_t speed, uint8_t square, uint8_t duty){

    params[id].changeAmountValue = (float) (Tools::ToSigned(amount) * AUDIO_STEP) * 2;
    params[id].changeSpeedValue  = (float) (speed * AUDIO_STEP);
    params[id].squareDutyValue   = (float) (square * AUDIO_STEP);
    params[id].dutySweepValue    = (float) (Tools::ToSigned(duty) * AUDIO_STEP) * 2;
    
    unsigned short dir = GetSoundDir(id);
    Tools::Poke(dir + 9,  amount);
    Tools::Poke(dir + 10, speed);
    Tools::Poke(dir + 11, square);
    Tools::Poke(dir + 12, duty);

}
void AudioManager::SFXRepeat(uint8_t id, uint8_t speed, uint8_t offset, uint8_t sweep){

    params[id].repeatSpeedValue = (float) (speed * AUDIO_STEP);
    params[id].phaserOffsetValue = (float) (Tools::ToSigned(offset) * AUDIO_STEP) * 2;
    params[id].phaserSweepValue = (float) (Tools::ToSigned(sweep) * AUDIO_STEP) * 2;

    unsigned short dir = GetSoundDir(id);
    Tools::Poke(dir + 13, speed);
    Tools::Poke(dir + 14, offset);
    Tools::Poke(dir + 15, sweep);
   
}
void AudioManager::SFXFilter(uint8_t id, uint8_t lpfCutoff, uint8_t lpfSweep, 
        uint8_t lpfRes, uint8_t hpfCutoff, uint8_t hpfSweep){

    params[id].lpfCutoffValue = (float) (lpfCutoff * AUDIO_STEP);
    params[id].lpfCutoffSweepValue = (float) (Tools::ToSigned(lpfSweep) * AUDIO_STEP) * 2;
    params[id].lpfResonanceValue = (float) (lpfRes * AUDIO_STEP);
    params[id].hpfCutoffValue = (float) (hpfCutoff * AUDIO_STEP);
    params[id].hpfCutoffSweepValue = (float) (Tools::ToSigned(hpfSweep) * AUDIO_STEP) * 2;

    unsigned short dir = GetSoundDir(id);
    Tools::Poke(dir + 16, lpfCutoff);
    Tools::Poke(dir + 17, lpfSweep);
    Tools::Poke(dir + 18, lpfRes);
    Tools::Poke(dir + 19, hpfCutoff);
    Tools::Poke(dir + 20, hpfSweep);
   
}

void AudioManager::SFXPlay(uint8_t id, uint8_t vol){
        SetSoundVolume(sound[id], (float)(vol * AUDIO_STEP)); // 1/256
        PlaySound(sound[id]);
}
void AudioManager::SFXStop(uint8_t id){
        StopSound(sound[id]);
}
void AudioManager::LoadSoundData(uint8_t id){
    unsigned int dir = 3376 + (id * 22);
    
    SFXWave(id, Tools::Peek(dir));
    SFXEnv(id, Tools::Peek(dir + 1), Tools::Peek(dir + 2), Tools::Peek(dir + 3), Tools::Peek(dir + 4));
    SFXFreq(id, Tools::Peek(dir + 5), Tools::Peek(dir + 6), Tools::Peek(dir + 7), Tools::Peek(dir + 8));
    SFXTone(id, Tools::Peek(dir + 9), Tools::Peek(dir + 10), Tools::Peek(dir + 11), Tools::Peek(dir + 12));
    SFXRepeat(id, Tools::Peek(dir + 13), Tools::Peek(dir + 14), Tools::Peek(dir + 15));
    SFXFilter(id, Tools::Peek(dir + 16), Tools::Peek(dir + 17), Tools::Peek(dir + 18), Tools::Peek(dir + 19), Tools::Peek(dir + 20));

    setNote(id, Tools::Peek(dir + 21));
    
}
void AudioManager::setNote(uint8_t id, uint8_t note){

    float n = ((note - 21)/12.0);
    float f =  0.087875 * (sqrt(pow(2, n)));

    params[id].startFrequencyValue = f;
}
unsigned short AudioManager::GetSoundDir(uint8_t id){
    return (3376 + (id * 22));
}


//tsf
void AudioManager::GetTSFPresets(){
	for (int i = 0; i < tsf_get_presetcount(ptsf); i++){
		printf("Preset #%d '%s'\n", i, tsf_get_presetname(ptsf, i));
	}
} 
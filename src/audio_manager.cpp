#include "audio_manager.h"
#include <stdio.h>
#include <cstring>
#include <cmath>

tsf* ptsf;
MMLParser* mml[TRACK_COUNT];

void audioInputCallback(void *buffer, unsigned int frames){
	//int sampleCount = (frames / (0.5 * sizeof(short)));
	//tsf_render_short(ptsf, (short*)buffer, sampleCount, 0);
}

void mmlCallback(MMLEvent event, int ch, int num, int velocity, AudioManager* au){
    switch (event) {
        case MML_NOTE_ON:
            printf("(%d)%d,%d", ch, num, velocity);
            au->PlayNote(num, ch, velocity);
            //mml[x]->getTotalSteps()
            break;
        case MML_NOTE_OFF:
            printf("- ");
            au->StopNote(num, ch);
            break;
        case MML_PROGRAM_CHANGE:
            printf("\n[Voice:%d]\n", ch);
            break;
    }
}

AudioManager::AudioManager(){

    for (int i = 0; i < MAX_WAVE_SLOTS; i++){
        // Reset generation parameters
        // NOTE: Random seed for generation is set
        ResetWaveParams(&params[i]);
        //params[i] = GenRandomize();

        // Default wave values
        wave[i].sampleRate = RFXGEN_GEN_SAMPLE_RATE;
        wave[i].sampleSize = RFXGEN_GEN_SAMPLE_SIZE;
        wave[i].channels = RFXGEN_GEN_CHANNELS;
        wave[i].frameCount = wave[i].sampleRate;

        wave[i].data = GenerateWave(params[i], &wave[i].frameCount);
        sound[i] = LoadSoundFromWave(wave[i]);
    }
   
    //ptsf = tsf_load_filename("assets/keygen.sf2");
    SetAudioStreamBufferSizeDefault(MAX_SAMPLES_PER_UPDATE);
    AudioStream stream = LoadAudioStream(SAMPLERATE, SAMPLESIZE, CHANNELS);
    SetAudioStreamCallback(stream, audioInputCallback);

    PlayAudioStream(stream);

    //tsf_set_output(ptsf, TSF_STEREO_INTERLEAVED, SAMPLERATE, -7); 
    //GetPresets();
   
    for (int i = 0; i < TRACK_COUNT; i++) {
        mml[i] = new MMLParser(this);
        mml[i]->setCallback(mmlCallback);
        sequence[i] = "";
    }
}

AudioManager::~AudioManager(){}

//Music
void AudioManager::Update(){
    bool isPlaying = false;
    for (int i = 0; i < TRACK_COUNT; i++) {
        if (mml[i]->isPlaying()) {
            isPlaying = true;
            mml[i]->update(audioTick);
        }
    }
    if (isPlaying) audioTick++;
}

void AudioManager::SetSequence(unsigned char id, const char* newSequence){
    if (id > TRACK_COUNT - 1) 
        id = TRACK_COUNT - 1;
    sequence[id] = newSequence;
    audioTick = 0;
}
const char* AudioManager::GetSequence(unsigned char id){
        return sequence[id];
}

void AudioManager::PlayNote(int note, int voice, int volume){
    SFXRender(voice, note);
    SFXPlay(voice, volume);
}
void AudioManager::StopNote(int note, int voice){

    //StopSound(sound[voice]);
}
void AudioManager::MusicPlay(){
    audioTick = 0;
    for (int i = 0; i < TRACK_COUNT; i++) {
        size_t lenght = strlen(sequence[i]);
        if (lenght > 2){
            printf("Playing #%d '%s'\n", i, sequence[i]);
            mml[i]->play(sequence[i], true);
        }    
    }
}
void AudioManager::MusicStop(){
    for (int i = 0; i < TRACK_COUNT; i++) {
        mml[i]->stop();
    }
}

//Effects
void AudioManager::SFXRender(unsigned char id, unsigned char note){

    float n = ((note - 21)/12.0);
    float f =  0.087875 * (sqrt(pow(2, n)));

    params[id].startFrequencyValue = f;

    wave[id].data = GenerateWave(params[id], &wave[id].frameCount);
    sound[id] = LoadSoundFromWave(wave[id]);

    sfxBytes[id].startFrequency = note;
}
void AudioManager::SFXWave(unsigned char id, unsigned char waveType){

    params[id].waveTypeValue = waveType;
    sfxBytes[id].wave = waveType;
}
void AudioManager::SFXEnv(unsigned char id, unsigned char att, unsigned char susT, unsigned char susP, unsigned char dec){

    params[id].attackTimeValue   = (float) (att * AUDIO_STEP);
    params[id].sustainTimeValue  = (float) (susT * AUDIO_STEP);
    params[id].sustainPunchValue = (float) (susP * AUDIO_STEP);
    params[id].decayTimeValue    = (float) (dec * AUDIO_STEP);
    
    sfxBytes[id].attackTime   = att ;
    sfxBytes[id].sustainTime  = susT;
    sfxBytes[id].sustainPunch = susP;
    sfxBytes[id].decayTime    = dec ;

}
void AudioManager::SFXFreq(unsigned char id, unsigned char slide, unsigned char delta, unsigned char vibratoD, unsigned char vibratoS){

    signed char sl = (signed char)slide;
    signed char de = (signed char)delta;

    params[id].slideValue        = (float) (sl * AUDIO_STEP) * 2;
    params[id].deltaSlideValue   = (float) (de * AUDIO_STEP) * 2;
    params[id].vibratoDepthValue = (float) (vibratoD * AUDIO_STEP);
    params[id].vibratoSpeedValue = (float) (vibratoS * AUDIO_STEP);

    sfxBytes[id].slide        = slide;
    sfxBytes[id].deltaSlide   = delta;
    sfxBytes[id].vibratoDepth = vibratoD;
    sfxBytes[id].vibratoSpeed = vibratoS;
}
void AudioManager::SFXTone(unsigned char id, unsigned char amount, unsigned char speed, unsigned char square, unsigned char duty){

    signed char am = (signed char) amount;
    signed char du = (signed char) duty;

    params[id].changeAmountValue = (float) (am * AUDIO_STEP) * 2;
    params[id].changeSpeedValue  = (float) (speed * AUDIO_STEP);
    params[id].squareDutyValue   = (float) (square * AUDIO_STEP);
    params[id].dutySweepValue    = (float) (du * AUDIO_STEP) * 2;

    sfxBytes[id].changeAmount = amount;
    sfxBytes[id].changeSpeed  = speed;
    sfxBytes[id].squareDuty   = square;
    sfxBytes[id].dutySweep    = duty;

}
void AudioManager::SFXRepeat(unsigned char id, unsigned char speed, unsigned char offset, unsigned char sweep){

    signed char off = (signed char) offset;
    signed char sw = (signed char) sweep;

    params[id].repeatSpeedValue = (float) (speed * AUDIO_STEP);
    params[id].phaserOffsetValue = (float) (off * AUDIO_STEP) * 2;
    params[id].phaserSweepValue = (float) (sw * AUDIO_STEP) * 2;

    sfxBytes[id].repeatSpeed  = speed;
    sfxBytes[id].phaserOffset = offset;
    sfxBytes[id].phaserSweep  = sweep;

}
void AudioManager::SFXFilter(unsigned char id, unsigned char lpfCutoff, unsigned char lpfSweep, 
        unsigned char lpfRes, unsigned char hpfCutoff, unsigned char hpfSweep){

    signed char ls= (signed char)lpfSweep;
    signed char hs = (signed char)hpfSweep;

    params[id].lpfCutoffValue = (float) (lpfCutoff * AUDIO_STEP);
    params[id].lpfCutoffSweepValue = (float) (ls * AUDIO_STEP) * 2;
    params[id].lpfResonanceValue = (float) (lpfRes * AUDIO_STEP);
    params[id].hpfCutoffValue = (float) (hpfCutoff * AUDIO_STEP);
    params[id].hpfCutoffSweepValue = (float) (hs * AUDIO_STEP) * 2;

    sfxBytes[id].lpfCutoff      = lpfCutoff;
    sfxBytes[id].lpfCutoffSweep = lpfSweep;
    sfxBytes[id].lpfResonance   = lpfRes;
    sfxBytes[id].hpfCutoff      = hpfCutoff;
    sfxBytes[id].hpfCutoffSweep = hpfSweep;
}

void AudioManager::SFXPlay(unsigned char id, unsigned char vol){
        SetSoundVolume(sound[id], (float)(vol * 0.007874)); // 1/127
        PlaySound(sound[id]);
}

void AudioManager::LoadSoundData(unsigned char id){
    unsigned int dir = 3376 + (id * 22);

    sfxBytes[id].wave     = Tools::Peek(dir);

    sfxBytes[id].attackTime   = Tools::Peek(dir + 1);
    sfxBytes[id].sustainTime  = Tools::Peek(dir + 2);
    sfxBytes[id].sustainPunch = Tools::Peek(dir + 3);
    sfxBytes[id].decayTime    = Tools::Peek(dir + 4);

    sfxBytes[id].slide        = Tools::Peek(dir + 5); 
    sfxBytes[id].deltaSlide   = Tools::Peek(dir + 6);
    sfxBytes[id].vibratoDepth = Tools::Peek(dir + 7);
    sfxBytes[id].vibratoSpeed = Tools::Peek(dir + 8);

    sfxBytes[id].changeAmount = Tools::Peek(dir + 9); 
    sfxBytes[id].changeSpeed  = Tools::Peek(dir + 10);
    sfxBytes[id].squareDuty   = Tools::Peek(dir + 11);
    sfxBytes[id].dutySweep    = Tools::Peek(dir + 12);

    sfxBytes[id].repeatSpeed  = Tools::Peek(dir + 13); 
    sfxBytes[id].phaserOffset = Tools::Peek(dir + 14); 
    sfxBytes[id].phaserSweep  = Tools::Peek(dir + 15); 

    sfxBytes[id].lpfCutoff      = Tools::Peek(dir + 16); 
    sfxBytes[id].lpfCutoffSweep = Tools::Peek(dir + 17); 
    sfxBytes[id].lpfResonance   = Tools::Peek(dir + 18); 
    sfxBytes[id].hpfCutoff      = Tools::Peek(dir + 19); 
    sfxBytes[id].hpfCutoffSweep = Tools::Peek(dir + 20); 
    
    sfxBytes[id].startFrequency = Tools::Peek(dir + 21); 
    
}
void AudioManager::SaveSoundData(unsigned char id){
    unsigned int dir = 3376 + (id * 22);

    Tools::Poke(dir, sfxBytes[id].wave);
    Tools::Poke(dir + 1, sfxBytes[id].attackTime);
    Tools::Poke(dir + 2, sfxBytes[id].sustainTime);
    Tools::Poke(dir + 3, sfxBytes[id].sustainPunch);
    Tools::Poke(dir + 4, sfxBytes[id].decayTime);

    Tools::Poke(dir + 5, sfxBytes[id].slide);
    Tools::Poke(dir + 6, sfxBytes[id].deltaSlide);
    Tools::Poke(dir + 7, sfxBytes[id].vibratoDepth);
    Tools::Poke(dir + 8, sfxBytes[id].vibratoSpeed);

    Tools::Poke(dir + 9,  sfxBytes[id].changeAmount);
    Tools::Poke(dir + 10, sfxBytes[id].changeSpeed);
    Tools::Poke(dir + 11, sfxBytes[id].squareDuty);
    Tools::Poke(dir + 12, sfxBytes[id].dutySweep);

    Tools::Poke(dir + 13, sfxBytes[id].repeatSpeed);
    Tools::Poke(dir + 14, sfxBytes[id].phaserOffset);
    Tools::Poke(dir + 15, sfxBytes[id].phaserSweep);

    Tools::Poke(dir + 16, sfxBytes[id].lpfCutoff);
    Tools::Poke(dir + 17, sfxBytes[id].lpfCutoffSweep);
    Tools::Poke(dir + 18, sfxBytes[id].lpfResonance);
    Tools::Poke(dir + 19, sfxBytes[id].hpfCutoff);
    Tools::Poke(dir + 20, sfxBytes[id].hpfCutoffSweep);

    Tools::Poke(dir + 21, sfxBytes[id].startFrequency);
}

//tsf
void AudioManager::GetTSFPresets(){
	for (int i = 0; i < tsf_get_presetcount(ptsf); i++){
		printf("Preset #%d '%s'\n", i, tsf_get_presetname(ptsf, i));
	}
} 
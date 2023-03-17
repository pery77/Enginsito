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
        LoadSoundData(i);

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

    setNote(id, note);

    wave[id].data = GenerateWave(params[id], &wave[id].frameCount);
    sound[id] = LoadSoundFromWave(wave[id]);

    unsigned short dir = GetSoundDir(id);
    Tools::Poke(dir + 21, note);
}
void AudioManager::SFXWave(unsigned char id, unsigned char waveType){

    params[id].waveTypeValue = waveType;
    unsigned short dir = GetSoundDir(id);
    Tools::Poke(dir, waveType);
}
void AudioManager::SFXEnv(unsigned char id, unsigned char att, unsigned char susT, unsigned char susP, unsigned char dec){

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
void AudioManager::SFXFreq(unsigned char id, unsigned char slide, unsigned char delta, unsigned char vibratoD, unsigned char vibratoS){

    signed char sl = (signed char)slide;
    signed char de = (signed char)delta;

    params[id].slideValue        = (float) (sl * AUDIO_STEP) * 2;
    params[id].deltaSlideValue   = (float) (de * AUDIO_STEP) * 2;
    params[id].vibratoDepthValue = (float) (vibratoD * AUDIO_STEP);
    params[id].vibratoSpeedValue = (float) (vibratoS * AUDIO_STEP);

    unsigned short dir = GetSoundDir(id);
    Tools::Poke(dir + 5, slide);
    Tools::Poke(dir + 6, delta);
    Tools::Poke(dir + 7, vibratoD);
    Tools::Poke(dir + 8, vibratoS);
  
}
void AudioManager::SFXTone(unsigned char id, unsigned char amount, unsigned char speed, unsigned char square, unsigned char duty){

    signed char am = (signed char) amount;
    signed char du = (signed char) duty;

    params[id].changeAmountValue = (float) (am * AUDIO_STEP) * 2;
    params[id].changeSpeedValue  = (float) (speed * AUDIO_STEP);
    params[id].squareDutyValue   = (float) (square * AUDIO_STEP);
    params[id].dutySweepValue    = (float) (du * AUDIO_STEP) * 2;
    
    unsigned short dir = GetSoundDir(id);
    Tools::Poke(dir + 9,  amount);
    Tools::Poke(dir + 10, speed);
    Tools::Poke(dir + 11, square);
    Tools::Poke(dir + 12, duty);

}
void AudioManager::SFXRepeat(unsigned char id, unsigned char speed, unsigned char offset, unsigned char sweep){

    signed char off = (signed char) offset;
    signed char sw = (signed char) sweep;

    params[id].repeatSpeedValue = (float) (speed * AUDIO_STEP);
    params[id].phaserOffsetValue = (float) (off * AUDIO_STEP) * 2;
    params[id].phaserSweepValue = (float) (sw * AUDIO_STEP) * 2;

    unsigned short dir = GetSoundDir(id);
    Tools::Poke(dir + 13, speed);
    Tools::Poke(dir + 14, offset);
    Tools::Poke(dir + 15, sweep);
   

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

    unsigned short dir = GetSoundDir(id);
    Tools::Poke(dir + 16, lpfCutoff);
    Tools::Poke(dir + 17, lpfSweep);
    Tools::Poke(dir + 18, lpfRes);
    Tools::Poke(dir + 19, hpfCutoff);
    Tools::Poke(dir + 20, hpfSweep);
   
}

void AudioManager::SFXPlay(unsigned char id, unsigned char vol){
        SetSoundVolume(sound[id], (float)(vol * 0.007874)); // 1/127
        PlaySound(sound[id]);
}

void AudioManager::LoadSoundData(unsigned char id){
    unsigned int dir = 3376 + (id * 22);
    
    SFXWave(id, Tools::Peek(dir));
    SFXEnv(id, Tools::Peek(dir + 1), Tools::Peek(dir + 2), Tools::Peek(dir + 3), Tools::Peek(dir + 4));
    SFXFreq(id, Tools::Peek(dir + 5), Tools::Peek(dir + 6), Tools::Peek(dir + 7), Tools::Peek(dir + 8));
    SFXTone(id, Tools::Peek(dir + 9), Tools::Peek(dir + 10), Tools::Peek(dir + 11), Tools::Peek(dir + 12));
    SFXRepeat(id, Tools::Peek(dir + 13), Tools::Peek(dir + 14), Tools::Peek(dir + 15));
    SFXFilter(id, Tools::Peek(dir + 16), Tools::Peek(dir + 17), Tools::Peek(dir + 18), Tools::Peek(dir + 19), Tools::Peek(dir + 20));

    setNote(id, Tools::Peek(dir + 21));
    
}
void AudioManager::setNote(unsigned char id, unsigned char note){

    float n = ((note - 21)/12.0);
    float f =  0.087875 * (sqrt(pow(2, n)));

    params[id].startFrequencyValue = f;
}
unsigned short AudioManager::GetSoundDir(unsigned char id){
    return (3376 + (id * 22));
}
//tsf
void AudioManager::GetTSFPresets(){
	for (int i = 0; i < tsf_get_presetcount(ptsf); i++){
		printf("Preset #%d '%s'\n", i, tsf_get_presetname(ptsf, i));
	}
} 
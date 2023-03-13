#include "audio_manager.h"
#include <stdio.h>

tsf* ptsf;
MMLParser* mml;
/*
void audioInputCallback(void *buffer, unsigned int frames)
{
	int sampleCount = (frames / (0.5 * sizeof(short)));
	tsf_render_short(ptsf, (short*)buffer, sampleCount, 0);
}
*/


void mmlCallback(MMLEvent event, int ch, int num, int velocity, AudioManager* au) {
    switch (event) {
        case MML_NOTE_ON:
            printf("(%d)%d,%d >%d", ch, num, velocity, mml->getTotalSteps());
            au->PlayNote(num, ch, velocity);
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
    /*
    //ptsf = tsf_load_filename("assets/keygen.sf2");
    ptsf = tsf_load_filename("assets/ins.sf2");
    
    SetAudioStreamBufferSizeDefault(MAX_SAMPLES_PER_UPDATE);
    AudioStream stream = LoadAudioStream(SAMPLERATE, SAMPLESIZE, CHANNELS);
    SetAudioStreamCallback(stream, audioInputCallback);

    PlayAudioStream(stream);

    tsf_set_output(ptsf, TSF_STEREO_INTERLEAVED, SAMPLERATE, -7); 

    //sequence = "ML AA8G8E.D8C2P2 E.D8C<A8G8G2>P2 <G.A8G.A8>C.D8EG A.G8E8D8CD2";
    GetPresets();
   */

    mml = new MMLParser(this);
    mml->setCallback(mmlCallback);

}

AudioManager::~AudioManager(){}

void AudioManager::Update(){
/*
    tsf_play_async(ptsf, voice, sequence, 1.0f);

    if (sequence && *sequence && audioTick == WAIT_TICKS) {
        sequence = tsf_play_await(ptsf, GetFrameTime());
        audioTick = 0;
    }
    audioTick++;
*/
    if (mml->isPlaying()) {
        if (!mml->update(audioTick)) {
            puts("Error\r\n");
        }
        audioTick++;
    }
}
void AudioManager::GetPresets()
{
    int i;
	for (int i = 0; i < tsf_get_presetcount(ptsf); i++)
	{
		printf("Preset #%d '%s'\n", i, tsf_get_presetname(ptsf, i));
	}
} 
void AudioManager::SetSequence(const char* newSequence){
    sequence = newSequence;
    audioTick = 0;
}
const char* AudioManager::GetSequence(){
        return sequence;
}

void AudioManager::PlayNote(int note, int voice, int volume){
    //if (volume<0) volume = 0;
    //if (volume>100) volume = 100;
    //tsf_note_on(ptsf, voice, note, volume*0.01);
    SFXRender(voice, note);
    SFXPlay(voice, volume);
}
void AudioManager::StopNote(int note, int voice){
    ////tsf_note_off(ptsf, voice, note);
    //tsf_note_off_all(ptsf);
    //StopSound(sound[voice]);
}
void AudioManager::MusicPlay(){
    audioTick = 0;
    mml->play(sequence, true);
}
void AudioManager::MusicStop(){
    mml->stop();
}

void AudioManager::SFXRender(unsigned char id, unsigned char note){
    float n = ((note - 21)/12.0);
    float f =  0.087875 * (sqrt(pow(2, n)));
    params[id].startFrequencyValue = f;
    wave[id].data = GenerateWave(params[id], &wave[id].frameCount);
    sound[id] = LoadSoundFromWave(wave[id]);
}
void AudioManager::SFXWave(unsigned char id, unsigned char waveType){
    params[id].waveTypeValue = waveType;
}
void AudioManager::SFXEnv(unsigned char id, unsigned char att, unsigned char susT, unsigned char susP, unsigned char dec){
    params[id].attackTimeValue = (float) (att * AUDIO_STEP);
    params[id].sustainTimeValue = (float) (susT * AUDIO_STEP);
    params[id].sustainPunchValue = (float) (susP * AUDIO_STEP);
    params[id].decayTimeValue = (float) (dec * AUDIO_STEP);
}
void AudioManager::SFXFreq(unsigned char id, unsigned char slide, unsigned char delta, unsigned char vibratoD, unsigned char vibratoS){
    signed char sl = (signed char)slide;
    signed char de = (signed char)delta;
    params[id].slideValue = (float) (sl * AUDIO_STEP) * 2;
    params[id].deltaSlideValue = (float) (de * AUDIO_STEP) * 2;
    params[id].vibratoDepthValue = (float) (vibratoD * AUDIO_STEP);
    params[id].vibratoSpeedValue = (float) (vibratoS * AUDIO_STEP);
}
void AudioManager::SFXTone(unsigned char id, unsigned char amount, unsigned char speed, unsigned char square, unsigned char duty){
    signed char am = (signed char) amount;
    signed char du = (signed char) duty;
    params[id].changeAmountValue = (float) (am * AUDIO_STEP) * 2;
    params[id].changeSpeedValue = (float) (speed * AUDIO_STEP);
    params[id].squareDutyValue = (float) (square * AUDIO_STEP);
    params[id].dutySweepValue = (float) (du * AUDIO_STEP) * 2;  
}
void AudioManager::SFXRepeat(unsigned char id, unsigned char speed, unsigned char offset, unsigned char sweep){
    signed char off = (signed char) offset;
    signed char sw = (signed char) sweep;
    params[id].repeatSpeedValue = (float) (speed * AUDIO_STEP);
    params[id].phaserOffsetValue = (float) (off * AUDIO_STEP) * 2;
    params[id].phaserSweepValue = (float) (sw * AUDIO_STEP) * 2;
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
}

void AudioManager::SFXPlay(unsigned char id, unsigned char vol){
        if (vol > 0) 
            SetSoundVolume(sound[id], (float)(vol * 0.007874)); // 1/127

        PlaySound(sound[id]);
}

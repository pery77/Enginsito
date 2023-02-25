#include "audio_manager.h"
#include <stdio.h>

tsf* ptsf;

void audioInputCallback(void *buffer, unsigned int frames)
{
	int sampleCount = (frames / (0.5 * sizeof(short)));
	tsf_render_short(ptsf, (short*)buffer, sampleCount, 0);
}

AudioManager::AudioManager(){

    for (int i = 0; i < MAX_WAVE_SLOTS; i++)
    {
        // Reset generation parameters
        // NOTE: Random seed for generation is set
        ResetWaveParams(&params[i]);
        //params[i] = GenRandomize();

        // Default wave values
        wave[i].sampleRate = RFXGEN_GEN_SAMPLE_RATE;
        wave[i].sampleSize = RFXGEN_GEN_SAMPLE_SIZE;
        wave[i].channels = RFXGEN_GEN_CHANNELS;
        wave[i].frameCount = wave[i].sampleRate;
        //wave[i].data = (float *)RL_CALLOC(wave[i].frameCount, sizeof(float));

        //params[0].startFrequencyValue = 10; // C - 4
        //params[0].startFrequencyValue = 261.626; // C - 4
        //params[1].startFrequencyValue = 523.251; // C - 4

        wave[i].data = GenerateWave(params[i], &wave[i].frameCount);
        sound[i] = LoadSoundFromWave(wave[i]);
    }
    //ptsf = tsf_load_filename("assets/keygen.sf2");
    ptsf = tsf_load_filename("assets/ins.sf2");
    
    SetAudioStreamBufferSizeDefault(MAX_SAMPLES_PER_UPDATE);
    AudioStream stream = LoadAudioStream(SAMPLERATE, SAMPLESIZE, CHANNELS);
    SetAudioStreamCallback(stream, audioInputCallback);

    PlayAudioStream(stream);

    tsf_set_output(ptsf, TSF_STEREO_INTERLEAVED, SAMPLERATE, -7); 

    //sequence = "ML AA8G8E.D8C2P2 E.D8C<A8G8G2>P2 <G.A8G.A8>C.D8EG A.G8E8D8CD2";
    GetPresets();
   
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
}
void AudioManager::GetPresets()
{
    int i;
	for (int i = 0; i < tsf_get_presetcount(ptsf); i++)
	{
		printf("Preset #%d '%s'\n", i, tsf_get_presetname(ptsf, i));
	}
} 
void AudioManager::SetSequence(const char* newSequence, int newVoice){
    sequence = newSequence;
    voice = newVoice;
    audioTick = 0;
}
const char* AudioManager::GetSequence(){
        return sequence;
}

void AudioManager::PlayNote(int note, int voice, int volume){
    if (volume<0) volume = 0;
    if (volume>100) volume = 100;
    tsf_note_on(ptsf, voice, note, volume*0.01);
}
void AudioManager::StopNote(int note, int voice){
    //tsf_note_off(ptsf, voice, note);
    tsf_note_off_all(ptsf);
}

void AudioManager::Stop(){
    sequence = "";
    audioTick = 0;
}

void AudioManager::SFXRender(unsigned char id, unsigned char waveType, unsigned char freq){
    printf("SFX Render: [%i]\n",id);
    params[id].waveTypeValue = waveType;
    params[id].startFrequencyValue = (float) (freq * AUDIO_STEP);
    wave[id].data = GenerateWave(params[id], &wave[id].frameCount);
    sound[id] = LoadSoundFromWave(wave[id]);
}
void AudioManager::SFXEnv(unsigned char id, unsigned char att, unsigned char susT, unsigned char susP, unsigned char dec){
    printf("SFX Env: [%i]\n",id);
    params[id].attackTimeValue = (float) (att * AUDIO_STEP);
    params[id].sustainTimeValue = (float) (susT * AUDIO_STEP);
    params[id].sustainPunchValue = (float) (susP * AUDIO_STEP);
    params[id].decayTimeValue = (float) (dec * AUDIO_STEP);
}
void AudioManager::SFXFreq(unsigned char id, unsigned char slide, unsigned char delta, unsigned char vibratoD, unsigned char vibratoS){
    printf("SFX FQ: [%i]\n",id);
    signed char sl = (signed char)slide * 2;
    signed char de = (signed char)delta * 2;
    params[id].slideValue = (float) (sl * AUDIO_STEP);
    params[id].deltaSlideValue = (float) (de * AUDIO_STEP);
    params[id].vibratoDepthValue = (float) (vibratoD * AUDIO_STEP);
    params[id].vibratoSpeedValue = (float) (vibratoS * AUDIO_STEP);

}
void AudioManager::SFXTone(unsigned char id, unsigned char amount, unsigned char speed, unsigned char square, unsigned char duty){
    printf("SFX Tone: [%i]\n",id);
    signed char am = (signed char)amount * 2;
    signed char du = (signed char)duty * 2;
    params[id].changeAmountValue = (float) (am * AUDIO_STEP);
    params[id].changeSpeedValue = (float) (speed * AUDIO_STEP);
    params[id].squareDutyValue = (float) (square * AUDIO_STEP);
    params[id].dutySweepValue = (float) (du * AUDIO_STEP);
        
}
void AudioManager::SFXRepeat(unsigned char id, unsigned char speed, unsigned char offset, unsigned char sweep){
    printf("SFX Repeat: [%i]\n",id);
    signed char off = (signed char)offset * 2;
    signed char sw = (signed char)sweep * 2;
    params[id].repeatSpeedValue = (float) (speed * AUDIO_STEP);
    params[id].phaserOffsetValue = (float) (off * AUDIO_STEP);
    params[id].phaserSweepValue = (float) (sw * AUDIO_STEP);
}
void AudioManager::SFXFilter(unsigned char id, unsigned char lpfCutoff, unsigned char lpfSweep, 
        unsigned char lpfRes, unsigned char hpfCutoff, unsigned char hpfSweep){
    printf("SFX Filter: [%i]\n",id);
    signed char ls= (signed char)lpfSweep * 2;
    signed char hs = (signed char)hpfSweep * 2;
    params[id].lpfCutoffValue = (float) (lpfCutoff * AUDIO_STEP);
    params[id].lpfCutoffSweepValue = (float) (ls * AUDIO_STEP);
    params[id].lpfResonanceValue = (float) (lpfRes * AUDIO_STEP);
    params[id].hpfCutoffValue = (float) (hpfCutoff * AUDIO_STEP);
    params[id].hpfCutoffSweepValue = (float) (hs * AUDIO_STEP);
}


void AudioManager::SFXPlay(unsigned char id){
        PlaySound(sound[id]);
}
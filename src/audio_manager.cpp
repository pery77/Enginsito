#include "audio_manager.h"
#include <stdio.h>

tsf* ptsf;
/*
void audioInputCallback(void *buffer, unsigned int frames)
{
	int sampleCount = (frames / (0.5 * sizeof(short)));
	tsf_render_short(ptsf, (short*)buffer, sampleCount, 0);
}
*/

MMLParser mml;
const char* music = "T96 KIf+ L8 @0"
    "O5 c.d16ccfa a<b>gr<b4^ 16a16>f4^8<g>d <f4L32e+fgfL8e+rr"
    "O5 d.e=16d16.d32e32f.e16d <b+4.>crr L16c<b=a+b=a+b=>a=gfe+dc L8<b4a+a=rr"
    "O4 g=.a16g=g=4g= g=b>dg=b>d <<f.g+16fa4g f4r"
    "@48 [O4 a4.^8gf e+fg:c4. >c4.^8<bagabe+4.]2 >dc<b^ 8>c<a4Q4bgQ8f4rr4r";
/*
void AudioManager::mmlCallback(MMLEvent event, int ch, int num, int velocity) {
    switch (event) {
    case MML_NOTE_ON:
        //midiOutShortMsg(h, MIDIMSG(0x9, ch, num, velocity));
        printf("%d ", num);
        AudioManager::PlayNote(0,num,100);
        
        // printf("%d (%d)\r\n", num, mml.getTotalSteps());
        break;
    case MML_NOTE_OFF:
        //midiOutShortMsg(h, MIDIMSG(0x8, ch, num, 0));
        printf("- ");
        break;
    case MML_PROGRAM_CHANGE:
        //midiOutShortMsg(h, MIDIMSG(0xC, ch, num, 0));
        printf("CHP.");
        break;
    }
}
*/
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
    //mml.setCallback(mmlCallback);
    mml.play(music, true);

}

AudioManager::~AudioManager(){}
unsigned long tick = 0;
void AudioManager::Update(){
/*
    tsf_play_async(ptsf, voice, sequence, 1.0f);

    if (sequence && *sequence && audioTick == WAIT_TICKS) {
        sequence = tsf_play_await(ptsf, GetFrameTime());
        audioTick = 0;
    }
    audioTick++;
*/
    if (mml.isPlaying()) {
        if (!mml.update(tick)) {
            puts("Error\r\n");
            printf("EERROORORO");
        }
        tick++;
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
void AudioManager::SetSequence(const char* newSequence, int newVoice){
    //sequence = newSequence;
    //voice = newVoice;
    //audioTick = 0;
}
const char* AudioManager::GetSequence(){
        return sequence;
}

void AudioManager::PlayNote(int note, int voice, int volume){
    //if (volume<0) volume = 0;
    //if (volume>100) volume = 100;
    //tsf_note_on(ptsf, voice, note, volume*0.01);
    SFXRender(voice,0,note);
}
void AudioManager::StopNote(int note, int voice){
    ////tsf_note_off(ptsf, voice, note);
    //tsf_note_off_all(ptsf);
}

void AudioManager::Stop(){
    sequence = "";
    audioTick = 0;
}

void AudioManager::SFXRender(unsigned char id, unsigned char waveType, unsigned char note){
    params[id].waveTypeValue = waveType;
    float n = ((note - 21)/12.0);
    float f =  0.087875 * (sqrt(pow(2, n)));
    params[id].startFrequencyValue = f;
    wave[id].data = GenerateWave(params[id], &wave[id].frameCount);
    sound[id] = LoadSoundFromWave(wave[id]);
    printf("SFX Render: [%i] wave: [%i]\n",id,  params[id].waveTypeValue);
}
void AudioManager::SFXEnv(unsigned char id, unsigned char att, unsigned char susT, unsigned char susP, unsigned char dec){
    params[id].attackTimeValue = (float) (att * AUDIO_STEP);
    params[id].sustainTimeValue = (float) (susT * AUDIO_STEP);
    params[id].sustainPunchValue = (float) (susP * AUDIO_STEP);
    params[id].decayTimeValue = (float) (dec * AUDIO_STEP);
    printf("SFX Env: [%i] attack: [%f] susT: [%f] susP: [%f] dec: [%f]\n",id,
    params[id].attackTimeValue, params[id].sustainTimeValue, params[id].sustainPunchValue, params[id].decayTimeValue);
}
void AudioManager::SFXFreq(unsigned char id, unsigned char slide, unsigned char delta, unsigned char vibratoD, unsigned char vibratoS){
    signed char sl = (signed char)slide;
    signed char de = (signed char)delta;
    params[id].slideValue = (float) (sl * AUDIO_STEP) * 2;
    params[id].deltaSlideValue = (float) (de * AUDIO_STEP) * 2;
    params[id].vibratoDepthValue = (float) (vibratoD * AUDIO_STEP);
    params[id].vibratoSpeedValue = (float) (vibratoS * AUDIO_STEP);
    printf("SFX FQ: [%i]  slide: [%f] delta: [%f] vDep: [%f] vSpeed: [%f]\n",id ,
      params[id].slideValue, params[id].deltaSlideValue, params[id].vibratoDepthValue, params[id].vibratoSpeedValue);

}
void AudioManager::SFXTone(unsigned char id, unsigned char amount, unsigned char speed, unsigned char square, unsigned char duty){
    printf("SFX Tone: [%i]\n",id);
    signed char am = (signed char) amount;
    signed char du = (signed char) duty;
    params[id].changeAmountValue = (float) (am * AUDIO_STEP) * 2;
    params[id].changeSpeedValue = (float) (speed * AUDIO_STEP);
    params[id].squareDutyValue = (float) (square * AUDIO_STEP);
    params[id].dutySweepValue = (float) (du * AUDIO_STEP) * 2;
        
}
void AudioManager::SFXRepeat(unsigned char id, unsigned char speed, unsigned char offset, unsigned char sweep){
    printf("SFX Repeat: [%i]\n",id);
    signed char off = (signed char) offset;
    signed char sw = (signed char) sweep;
    params[id].repeatSpeedValue = (float) (speed * AUDIO_STEP);
    params[id].phaserOffsetValue = (float) (off * AUDIO_STEP) * 2;
    params[id].phaserSweepValue = (float) (sw * AUDIO_STEP) * 2;
}
void AudioManager::SFXFilter(unsigned char id, unsigned char lpfCutoff, unsigned char lpfSweep, 
        unsigned char lpfRes, unsigned char hpfCutoff, unsigned char hpfSweep){
    printf("SFX Filter: [%i]\n",id);
    signed char ls= (signed char)lpfSweep;
    signed char hs = (signed char)hpfSweep;
    params[id].lpfCutoffValue = (float) (lpfCutoff * AUDIO_STEP);
    params[id].lpfCutoffSweepValue = (float) (ls * AUDIO_STEP) * 2;
    params[id].lpfResonanceValue = (float) (lpfRes * AUDIO_STEP);
    params[id].hpfCutoffValue = (float) (hpfCutoff * AUDIO_STEP);
    params[id].hpfCutoffSweepValue = (float) (hs * AUDIO_STEP) * 2;
}

void AudioManager::SFXPlay(unsigned char id){
        //StopSound(sound[id]);
        PlaySound(sound[id]);
}
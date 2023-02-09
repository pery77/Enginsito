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
        wave[i].frameCount = 5*wave[i].sampleRate;    // Max frame count for 10 seconds
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

void AudioManager::SFXSet(int id, int waveType, int freq, int att, int susT, int susP, int dec){
        printf("SFX set: [%i] wave:%i\n");
        params[id].waveTypeValue = waveType;
        params[id].startFrequencyValue = (float) (freq * 0.01f);

        params[id].attackTimeValue = (float) (att * 0.01f);
        params[id].sustainTimeValue = (float) (susT * 0.01f);
        params[id].sustainPunchValue = (float) (susP * 0.01f);
        params[id].decayTimeValue = (float) (dec * 0.01f);

        wave[id].data = GenerateWave(params[id], &wave[id].frameCount);
        sound[id] = LoadSoundFromWave(wave[id]);
}
void AudioManager::SFXPlay(int id, int fq){
        SetSoundPitch(sound[id],(fq / 1000.0));
        PlaySound(sound[id]);
}
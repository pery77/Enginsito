#include "audio_manager.h"

tsf* ptsf;
//int audioTick = 0;


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
        params[i] = GenRandomize();

        // Default wave values
        wave[i].sampleRate = RFXGEN_GEN_SAMPLE_RATE;
        wave[i].sampleSize = RFXGEN_GEN_SAMPLE_SIZE;
        wave[i].channels = RFXGEN_GEN_CHANNELS;
        wave[i].frameCount = 10*wave[i].sampleRate;    // Max frame count for 10 seconds
        //wave[i].data = (float *)RL_CALLOC(wave[i].frameCount, sizeof(float));
        wave[i].data = GenerateWave(params[i], &wave[i].frameCount);
        sound[i] = LoadSoundFromWave(wave[i]);
    }
    ptsf = tsf_load_filename("assets/florestan-subset.sf2");

    SetAudioStreamBufferSizeDefault(MAX_SAMPLES_PER_UPDATE);
    AudioStream stream = LoadAudioStream(44100, 16, 2);
    SetAudioStreamCallback(stream, audioInputCallback);

    PlayAudioStream(stream);

    tsf_set_output(ptsf, TSF_STEREO_INTERLEAVED, 44100, -7); 
sequence = "ML AA8G8E.D8C2P2 E.D8C<A8G8G2>P2 <G.A8G.A8>C.D8EG A.G8E8D8CD2";
   
}

AudioManager::~AudioManager(){}

void AudioManager::Update(){

    tsf_play_async(ptsf, 0, sequence, 1.0f);

    if (sequence && *sequence && audioTick == WAIT_TICKS) {
        sequence = tsf_play_await(ptsf, GetFrameTime());
        audioTick = 0;
    }
    audioTick++;
}

void AudioManager::SetSequence(const char* newSequence){
    sequence = newSequence;
    audioTick = 0;
}
const char* AudioManager::GetSequence(){
        return sequence;
}

void AudioManager::PlayNote(int note, int volume){
    if (volume<0) volume = 0;
    if (volume>100) volume = 100;
    tsf_note_on(ptsf, 0, note, volume*0.01);
}

void AudioManager::Stop(){
    sequence = "";
    audioTick = 0;
}
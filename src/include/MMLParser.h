//
// MMLParser.h
// https://github.com/vcraftjp/MML-Parser/
// a subset of "mml2mid" http://hpc.jp/~mml2mid/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include "audio_manager.h"

typedef const char* MMLPTR;

#define OCTAVE_STEPS 12

enum MMLEvent {
	MML_NOTE_ON,
	MML_NOTE_OFF,
	MML_PROGRAM_CHANGE
};

struct MMLLoop {
	int index;
	int count;
};

#define MAX_LOOP_NEST 4
class AudioManager;
//
// MMLParser class
//
class MMLParser {
public:

	MMLParser(AudioManager* audioManager, int channel = 0);
	~MMLParser();

	void setCallback(void (*_pfnCallback)(MMLEvent, int, int, int, int, AudioManager*)) { pfnCallback = _pfnCallback; }
	void setChannel(int _channel) { channel = _channel; }
	void setMaxVelocity(int _maxVelocity) { maxVelocity = _maxVelocity;	}
	void play(MMLPTR mml, bool isLoop = false);
	void stop();
	void pause(bool b);
	bool isPlaying() { return _isPlaying; }
	bool isPaused() { return _isPaused; }
	bool update(float tick);
	unsigned int getSize() { return size; }
	MMLPTR getErrorPoint() { return p - 1; }
	int getTotalSteps() { return totalSteps; }
	
	static void stopAll();
	static void pauseAll(bool b);
	static bool updateAll(unsigned long tick);
	static void setTempo(int _tempo) { tempo = _tempo; }
protected:
	MMLParser *pNextMMLParser;
	static MMLParser *pFirstMMLParser;
	static MMLParser *pPrevMMLParser;

	AudioManager* audioM;
	uint8_t channel = 0;
	bool _isPlaying;
	bool _isPaused;
	bool isLoop;
	float startTick = 0;
	float prevTick = 0;
	float pauseTick = 0;
	unsigned int steps = 0;
	unsigned int stepsGate = 0;
	unsigned int totalSteps = 0;
	MMLPTR pStart;
	MMLPTR p;
	MMLLoop mmlLoops[MAX_LOOP_NEST];
	int loopNest;
	int8_t keyTable[OCTAVE_STEPS];

	int8_t note;
	int8_t octave;
	uint8_t length;
	uint8_t gate;
	uint8_t velocity;
	uint8_t maxVelocity;
	uint8_t prog;
	static int16_t tempo;
	int prevNum;
	bool isMeasuring;
	unsigned int size = 0;

	void(*pfnCallback)(MMLEvent, int, int, int, int, AudioManager*); // event channel, note, program, velocity

	void init();
	void startup();
	bool parse();
	unsigned int measure();

	void noteOn(int note, int volume = 64);
	void noteOff();
	void programChange(int _prog);

	bool parseTone(char c);
	int parseNote(int noteIndex, int* pOctave = NULL, bool* pNoTranspose = NULL);
	bool parseRest();
	int parseValue();
	int parseValue(int defaultValue);
	int parseSteps();
	bool parseKeyChange(char c);
	bool loopStart();
	bool loopEnd();
	bool loopSkip();
	void setKeyTable(int note, bool major = true);

	void skipSpace();
	bool skipComment();
};
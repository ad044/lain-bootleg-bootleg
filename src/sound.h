#pragma once

#include <sndfile.h>
#include <portaudio.h>

#define SOUND_COUNT 11

typedef enum {
	SND_110,
	SND_111,
	SND_112,
	SND_113,
	SND_114,
	SND_115,
	SND_116,
	SND_117,
	SND_118,
	SND_119,
	SND_120,
} SoundID;

typedef struct {
	sf_count_t offset, length;
	unsigned char *bytes;
	SF_INFO info;
	SNDFILE *file;
} SoundData;

int sounds_init(SoundData *sound_files);
void play_sound(SoundData *data, PaStream *stream);

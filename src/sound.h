#pragma once

#include <portaudio.h>
#include <sndfile.h>

#define SOUND_COUNT 12

#define MAX_SOUND_QUEUE_SIZE 32

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
	SND_MOVIE_AUDIO
} SoundID;

typedef struct {
	sf_count_t offset, length;
	unsigned char *bytes;
	SF_INFO info;
	SNDFILE *file;
	_Bool paused;
} SoundData;

typedef struct {
	int size;
	SoundID arr[MAX_SOUND_QUEUE_SIZE];
} SoundQueue;

int sounds_init(SoundData *sound_files);
void close_audio_stream(void *user_data);
void enqueue_sound(SoundQueue *queue, SoundID id);
void dequeue_sound(SoundQueue *queue, SoundID *target);
void *sound_loop(void *data);

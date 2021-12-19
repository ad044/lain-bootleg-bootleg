#include "sound.h"
#include "engine.h"

static int init_audio(SoundData *data, char *file_path)
{
	data->offset = 0;
	data->paused = false;

	memset(&data->info, 0, sizeof(data->info));
	data->info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	data->info.channels = 1;
	data->info.samplerate = 22050;

	data->file = sf_open(file_path, SFM_READ, &data->info);

	if (sf_error(data->file) != SF_ERR_NO_ERROR) {
		printf("Error initializing %s.\n", sf_strerror(data->file));
		return 0;
	}

	return 1;
}

int sounds_init(SoundData *sounds)
{
	for (int i = 0; i < SOUND_COUNT - 1; i++) {
		char file_path[32];
		sprintf(file_path, "./res/sounds/%d.wav", i);
		if (!init_audio(&sounds[i], file_path)) {
			goto fail;
			return 0;
		}
	}

	if (!init_audio(&sounds[SND_MOVIE_AUDIO], "./res/movie_audio.opus")) {
		goto fail;
	};

	return 1;
fail:
	printf("Failed to initialize sounds.\n");
	return 0;
}

static int callback(const void *input, void *output, unsigned long frame_count,
		    const PaStreamCallbackTimeInfo *time_info,
		    PaStreamCallbackFlags status_flags, void *user_data)
{
	SoundData *sound_data = (SoundData *)user_data;

	float *out = (float *)output;
	memset(out, 0, sizeof(float) * frame_count * sound_data->info.channels);

	sf_count_t num_read = sf_read_float(
	    sound_data->file, out, frame_count * sound_data->info.channels);

	if (num_read < frame_count || sound_data->paused) {
		sf_seek(sound_data->file, 0, SEEK_SET);
		return paComplete;
	}

	return paContinue;
}

static void play_sound(SoundData *data, PaStream *stream)
{
	if (Pa_OpenDefaultStream(&stream, 0, data->info.channels, paFloat32,
				 data->info.samplerate,
				 paFramesPerBufferUnspecified, callback,
				 data) != paNoError) {
		printf("Problem opening default audio stream.\n");
		return;
	}

	if (Pa_StartStream(stream) != paNoError) {
		printf("Problem starting audio stream.\n");
		return;
	}
}

void close_audio_stream(void *user_data)
{
	PaStream *stream = (PaStream *)user_data;
	if (Pa_CloseStream(stream) != paNoError) {
		printf("Problem closing audio stream.\n");
	}
}

void enqueue_sound(SoundQueue *queue, SoundID id)
{
	if (queue->size < MAX_SOUND_QUEUE_SIZE) {
		queue->size++;
		queue->arr[queue->size - 1] = id;
	}
}

void dequeue_sound(SoundQueue *queue, SoundID *target)
{
	if (queue->size > 0) {
		*target = queue->arr[queue->size - 1];
		queue->size--;
	}
}

_Bool sound_queue_empty(SoundQueue *queue) { return queue->size == 0; }

static _Bool sound_thread_should_stop(Engine *engine)
{
	if (!pthread_mutex_trylock(&engine->running)) {
		pthread_mutex_unlock(&engine->running);
		return 1;
	} else {
		return 0;
	}

	return 1;
}

void *sound_loop(void *data)
{
	Engine *engine = (Engine *)data;
	while (!sound_thread_should_stop(engine)) {
		if (!sound_queue_empty(&engine->game_state.queued_sounds)) {
			SoundID id;
			dequeue_sound(&engine->game_state.queued_sounds, &id);

			play_sound(&engine->resources.sounds[id],
				   engine->audio_stream);
		}
	}

	return NULL;
}

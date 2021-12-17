#include "sound.h"
#include "engine.h"

static sf_count_t vfget_filelen(void *user_data)
{
	SoundData *vf = (SoundData *)user_data;

	return vf->length;
}

static sf_count_t vfseek(sf_count_t offset, int whence, void *user_data)
{
	SoundData *vf = (SoundData *)user_data;

	switch (whence) {
	case SEEK_SET:
		vf->offset = offset;
		break;

	case SEEK_CUR:
		vf->offset = vf->offset + offset;
		break;

	case SEEK_END:
		vf->offset = vf->length + offset;
		break;
	default:
		break;
	};

	return vf->offset;
}

static sf_count_t vfread(void *ptr, sf_count_t count, void *user_data)
{
	SoundData *vf = (SoundData *)user_data;

	if (vf->offset + count > vf->length)
		count = vf->length - vf->offset;

	memcpy(ptr, vf->bytes + vf->offset, count);
	vf->offset += count;

	return count;
}

static sf_count_t vfwrite(const void *ptr, sf_count_t count, void *user_data)
{
	SoundData *vf = (SoundData *)user_data;

	if (vf->offset >= (sf_count_t)sizeof(vf->bytes))
		return 0;

	if (vf->offset + count > (sf_count_t)sizeof(vf->bytes))
		count = sizeof(vf->bytes) - vf->offset;

	memcpy(vf->bytes + vf->offset, ptr, (size_t)count);
	vf->offset += count;

	if (vf->offset > vf->length)
		vf->length = vf->offset;

	return count;
}

static sf_count_t vftell(void *user_data)
{
	SoundData *vf = (SoundData *)user_data;

	return vf->offset;
}

int sounds_init(SoundData *sounds)
{
	EmbeddedResource files[SOUND_COUNT];
	load_sounds(files);

	SF_VIRTUAL_IO vio;
	vio.get_filelen = vfget_filelen;
	vio.seek = vfseek;
	vio.read = vfread;
	vio.write = vfwrite;
	vio.tell = vftell;

	for (int i = 0; i < SOUND_COUNT; i++) {
		SoundData *data = &sounds[i];

		data->bytes = files[i].bytes;
		data->length = files[i].size;
		data->offset = 0;
		data->stopped = false;

		memset(&data->info, 0, sizeof(data->info));
		data->info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
		data->info.channels = 1;
		data->info.samplerate = 22050;

		data->file = sf_open_virtual(&vio, SFM_READ, &data->info, data);

		if (sf_error(sounds[i].file) != SF_ERR_NO_ERROR) {
			printf("Error initializing %s.\n",
			       sf_strerror(data->file));
			return 0;
		}
	}

	return 1;
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

	if (num_read < frame_count || sound_data->stopped) {
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
		printf("Problem opening audio stream.\n");
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

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "sound.h"

void play_sound(ma_engine *audio_engine, SoundID id)
{
	char file_path[32];
	sprintf(file_path, "./res/sounds/%d.wav", id);
	ma_engine_play_sound(audio_engine, file_path, NULL);
}

int sounds_init(ma_engine *audio_engine)
{
	if (ma_engine_init(NULL, audio_engine) != MA_SUCCESS) {
		printf("Failed to initialize audio engine.\n");
		return 0;
	};

	return 1;
}

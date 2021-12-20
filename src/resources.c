#include "resources.h"
#include "sound.h"
#include "texture.h"

#include <stdio.h>
#include <string.h>

int init_resources(Resources *resources)
{
	textures_init(resources);

	if (!shaders_init(resources->shaders)) {
		printf("Failed to initialize shaders.\n");
		return 0;
	};

	fonts_init(resources);

	animations_init(resources);

	if (!sounds_init(&resources->audio_engine)) {
		return 0;
	};

	return 1;
}

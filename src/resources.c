#include "resources.h"
#include "embedded.h"
#include "texture.h"

#include <stdio.h>
#include <string.h>

void init_resources(Resources *resources)
{
	load_textures(resources->embedded);

	textures_init(resources);

	shaders_init(resources->shaders);

	fonts_init(resources);

	animations_init(resources);
}

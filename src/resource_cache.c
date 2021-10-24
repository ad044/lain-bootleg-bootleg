#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resource_cache.h"
#include "sprite.h"
#include "text.h"

int init_resource_cache(ResourceCache **resource_cache)
{
	*resource_cache = malloc(sizeof(ResourceCache));
	if (resource_cache == NULL) {
		printf("Failed to allocate memory for resource cache.\n");
		return 0;
	}

	// initialize shader cache
	if (!(shader_cache_init(&(*resource_cache)->shaders))) {
		printf("Failed to initialize shader cache.\n");
		return 0;
	}

	// initialize texture cache and fill it
	// (for now i will try to preload every texture, which is currently)
	// around 9 mb. if this proves to be problematic we can change the
	// approach.
	if (!(texture_cache_init(&(*resource_cache)->textures))) {
		printf("Failed to initialize texture cache.\n");
		return 0;
	}

	(*resource_cache)->fonts = malloc(sizeof(Font*) * FONT_COUNT);
	if ((*resource_cache)->fonts == NULL) {
		printf("Failed to allocate memory for font cache.\n");
		return 0;
	}

	fonts_init((*resource_cache)->fonts, (*resource_cache)->textures);

	return 1;
}

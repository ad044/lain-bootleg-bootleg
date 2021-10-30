#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resource_cache.h"
#include "sprite.h"
#include "text.h"

int init_resource_cache(ResourceCache *resource_cache)
{
	if (!(texture_cache_init(&resource_cache->textures))) {
		printf("Failed to initialize texture cache.\n");
		return 0;
	}

	if (!shaders_init(resource_cache->shaders)) {
		printf("Failed to initialize shaders.\n");
		return 0;
	};

	fonts_init(resource_cache->fonts, resource_cache->textures);

	return 1;
}

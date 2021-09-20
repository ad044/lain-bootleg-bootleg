#include <stdio.h>
#include <string.h>

#include "resource_cache.h"
#include "text_renderer.h"
#include "texture.h"

void draw_text(ResourceCache *resource_cache, char *text, char *font)
{
	GLuint texture_id =
	    texture_cache_get(resource_cache->textures, "white_font");

	if (!texture_id) {
		printf("Couldn't load white font.\n");
		exit(1);
	}

	for (int i = 0; i < strlen(text); i++) {
		printf("%c", text[i]);
	};
}

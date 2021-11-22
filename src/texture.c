#include <stdio.h>

#include "sprite.h"
#include "stb_image.h"

static void init_texture(Texture *texture, const unsigned char *bytes,
			 size_t length)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, nr_channels;
	unsigned char *data = stbi_load_from_memory(bytes, length, &width,
						    &height, &nr_channels, 0);
	if (data == NULL) {
		printf("Failed to load texture.\n");
		exit(1);
	}

	texture->size = (Vector2D){width, height};
	texture->nr_channels = nr_channels;

	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->size.x,
		     texture->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(data);
}

void textures_init(Texture *textures) { load_textures(init_texture, textures); }

#include <stdio.h>

#include "embedded.h"
#include "resources.h"
#include "sprite.h"
#include "stb_image.h"
#include "texture.h"

static int init_texture(Texture *texture, const unsigned char *bytes,
			size_t length, TextureID texture_id)
{
	texture->id = texture_id;

	stbi_set_flip_vertically_on_load(true);

	int width, height, nr_channels;
	unsigned char *data = stbi_load_from_memory(bytes, length, &width,
						    &height, &nr_channels, 0);
	if (data == NULL) {
		printf("Failed to load texture.\n");
		return 0;
	}

	texture->size = (Vector2D){width, height};

	glGenTextures(1, &texture->gl_id);
	glBindTexture(GL_TEXTURE_2D, texture->gl_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->size.x,
		     texture->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(data);

	return 1;
}

Texture *texture_get(Resources *resources, int texture_id)
{
	Texture *texture = &resources->textures[texture_id];

	if (!texture->gl_id) {
		EmbeddedResource rsrc = resources->embedded[texture_id];
		if (!init_texture(texture, rsrc.bytes, rsrc.size, texture_id)) {
			printf("Failed to initialize texture %d.\n",
			       texture_id);
			// TODO not sure what to do here
		};
	}

	return texture;
}

void textures_init(Resources *resources)
{
	for (int i = 0; i < MAX_TEXTURE_COUNT; i++) {
		resources->textures[i] = (Texture){0};
	};
}

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "stb_image.h"
#include "texture.h"

int init_texture(Texture *texture)
{
	stbi_set_flip_vertically_on_load(true);

	if (!(access(texture->image_path, F_OK) == 0)) {
		printf("Can't find texture %s.\n", texture->image_path);
		return 0;
	}
	int width, height, nr_channels;
	unsigned char *data =
	    stbi_load(texture->image_path, &width, &height, &nr_channels, 0);
	if (!data) {
		printf("Failed to load texture.\n");
		return 0;
	}

	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		     GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(data);
	return 1;
}

Texture make_texture(char *image_path)
{
	Texture texture = (Texture){.image_path = image_path};
	init_texture(&texture);
	return texture;
}

#pragma once

#include "vector2d.h"

#define MAX_TEXTURE_COUNT 512

typedef struct {
	GLuint id;
	int nr_channels;
	Vector2D size;
	char *name;
} Texture;

#include "embedded.h"

Texture make_texture(char *image_path, char *name);
void textures_init(Texture *textures);

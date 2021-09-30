#pragma once
#include <GL/glew.h>
#include <stdint.h>

#include "hashmap.h"

typedef struct {
	GLuint id;
	int nr_channels;
	GLfloat width;
	GLfloat height;
	char *name;
} Texture;

typedef struct hashmap TextureCache;

int init_texture(Texture *texture, char *image_path);
Texture make_texture(char *image_path, char *name);
int texture_cache_init(TextureCache **cache);
Texture *texture_cache_get(TextureCache *cache, char *texture_name);
void texture_cache_put(TextureCache *cache, Texture texture);
void preload_textures(TextureCache *cache);
// todo
void texture_cache_free(TextureCache *cache);

#pragma once
#include <stdint.h>
#include <GLFW/glfw3.h>

#include "hashmap.h"

typedef GLuint TextureID;
typedef struct {
  GLuint id;
  char *name;
  char *image_path;
} Texture;
typedef struct hashmap TextureCache;

int init_texture(Texture *texture);
Texture make_texture(char *image_path, char *name);
int texture_cache_init(TextureCache **cache);
int texture_cache_get(TextureCache *cache, char *texture_name);
void texture_cache_put(TextureCache *cache, char *image_path,
		       char *texture_name);
void preload_textures(TextureCache *cache);
//todo
void texture_cache_free(TextureCache *cache);


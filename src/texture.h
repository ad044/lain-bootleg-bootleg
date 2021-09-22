#pragma once
#include <GL/glew.h>
#include <stdint.h>

#include "hashmap.h"

typedef struct {
	char *name;
	GLuint texture_id;
	unsigned int texture_index;
} TextureSlot;

typedef struct hashmap TextureSlotMap;

typedef struct {
	GLuint id;
	int nr_channels;
	GLfloat width;
	GLfloat height;
	char *name;
} Texture;

typedef struct hashmap TextureCache;

int init_texture(Texture *texture, char *image_path);
Texture *make_texture(char *image_path, char *name);
int texture_cache_init(TextureCache **cache);
Texture *texture_cache_get(TextureCache *cache, char *texture_name);
void texture_cache_put(TextureCache *cache, Texture *texture);
void preload_textures(TextureCache *cache);
void texture_slot_map_put(TextureSlotMap *map, TextureSlot slot);
int texture_slot_map_init(TextureSlotMap **map);
void texture_slot_map_bind(TextureSlotMap *map);
TextureSlot texture_slot_map_get(TextureSlotMap *map, char *texture_slot_name);
// todo
void texture_cache_free(TextureCache *cache);
void texture_slot_map_free(TextureSlotMap *map);
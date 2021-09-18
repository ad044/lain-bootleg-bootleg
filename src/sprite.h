#pragma once
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/types.h>

#include "resource_cache.h"
#include "shader.h"
#include "texture.h"

typedef struct hashmap SpriteMap;
typedef struct {
	GLuint texture_id;
	char *name;
	ShaderProgram shader_program;
	vec2 pos;
	vec2 size;
} Sprite;

Sprite *make_sprite(ResourceCache *resource_cache, char *texture_name,
		   char *name);
void draw_spritemap(SpriteMap *sprite_map, unsigned int *VAO);
void sprite_set_size(Sprite *sprite, vec2 size);
void sprite_set_pos(Sprite *sprite, vec2 pos);
void draw_sprite(const Sprite *sprite, GLuint VAO);
int sprite_map_init(SpriteMap **cache);
Sprite sprite_map_get(SpriteMap *cache, char *sprite_name);
void sprite_map_put(SpriteMap *cache, Sprite *sprite);
// todo
void sprite_map_free(SpriteMap *cache);

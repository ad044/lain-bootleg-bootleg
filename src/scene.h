#pragma once

#include "hashmap.h"
#include "shader.h"
#include "stdlib.h"
#include "texture.h"

#include <cglm/types.h>

typedef struct {
	vec2 pos;
	vec2 size;
	unsigned int texture_index;
	// optional parameters for when offsets/tiling are needed
	vec2 texture_size;
	vec2 texture_offset;
} Sprite;

typedef struct {
	GLuint VAO;
	unsigned int index_count;
	TextureSlotMap *texture_slot_map;
	ShaderProgram current_shader;
} Scene;

int init_scene(Scene **scene, Sprite *sprites, unsigned int sprite_count,
	       TextureSlot *texture_slots, unsigned int texture_slot_count,
	       ShaderProgram shader);
void draw_scene(Scene *scene);
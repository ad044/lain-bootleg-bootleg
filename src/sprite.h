#pragma once

#include "vector2d.h"
#include <stdbool.h>

typedef struct {
	Vector2D pos;
	Vector2D size;
	unsigned int z_index;
	unsigned int texture_index;
	_Bool visible;
	// in case of a spritesheet, texture_size denotes size of a single
	// sprite from the atlas.
	Vector2D texture_size;
	// always 0 If non-spritesheet texture
	unsigned int current_frame;
	unsigned int max_frame;
} Sprite;

void depth_sort(Sprite **sprites, unsigned int sprite_count);
void generate_sprite_indices(GLuint *buffer, unsigned int index_count);
GLfloat *get_sprite_vertices(GLfloat *buffer, Sprite *sprite);
unsigned int get_sprite_index_count(unsigned int sprite_count);
unsigned int get_sprite_vertex_buffer_size(unsigned int sprite_count);
_Bool is_sprite_within_bounds(const Sprite *sprite, const Vector2D point);
void update_sprite_buffers(GLuint VBO, GLuint IBO, GLfloat *vertices,
			   GLfloat vertices_size, unsigned int sprite_count);
void sprite_hide(Sprite *sprite);
void sprite_show(Sprite *sprite);
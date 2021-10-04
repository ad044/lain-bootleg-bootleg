#pragma once

#include <stdbool.h>
#include "vector2d.h"

typedef struct {
	Vector2D pos;
	Vector2D size;
	// in case of a spritesheet, texture_size denotes size of a single
	// sprite from the atlas.
	Vector2D texture_size;
	Vector2D texture_offset;
	unsigned int z_index;
	unsigned int texture_index;
} Sprite;

void depth_sort(Sprite **sprites, unsigned int sprite_count);
void generate_sprite_indices(GLuint *buffer, unsigned int index_count);
GLfloat *get_sprite_vertices(GLfloat *buffer, Sprite *sprite);
unsigned int get_sprite_index_count(unsigned int sprite_count);
unsigned int get_sprite_vertex_buffer_size(unsigned int sprite_count);
int make_sprite(Sprite **target, Sprite sprite);
_Bool spritesheet_is_last_frame(const Sprite *sprite);
_Bool spritesheet_is_first_frame(const Sprite *sprite);
_Bool is_sprite_within_bounds(const Sprite *sprite, const Vector2D point);
void update_sprite_buffers(GLuint VBO, GLuint IBO, GLfloat *vertices,
			   GLfloat vertices_size, unsigned int sprite_count);

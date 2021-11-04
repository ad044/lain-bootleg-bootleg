#pragma once

#include "texture.h"

#include <stdbool.h>


#define SPRITE_VERTEX_COUNT 4
#define SPRITE_INDEX_COUNT 6
#define ROWS_PER_SPRITE_VERTEX 5

#define SPRITE_VBO_SIZE SPRITE_VERTEX_COUNT *ROWS_PER_SPRITE_VERTEX

typedef struct {
	Vector2D pos;
	Vector2D origin_pos; // keeps track of initially passed position.
	Vector2D size;
	unsigned int z_index;
	Texture *texture; // TODO this is only used during definition, hacky
	unsigned int texture_index; // 0 until fed into a scene initializer
	_Bool visible;
	_Bool is_spritesheet;
	_Bool mirrored;
	// in case of a spritesheet, texture_size denotes size of a single
	// sprite from the atlas.
	Vector2D texture_size;
	unsigned int current_frame; // 0 if non-spritesheet texture
	unsigned int max_frame;	    // 0 if non-spritesheet texture
} Sprite;

void depth_sort(Sprite **sprites, unsigned int sprite_count);
void initialize_sprite(Sprite *sprite);
_Bool is_sprite_within_bounds(const Sprite *sprite, const Vector2D point);
Vector2D get_sprite_center_coords(Sprite *sprite);
GLfloat *get_quad_vertices(GLfloat *buffer, Sprite *sprite);

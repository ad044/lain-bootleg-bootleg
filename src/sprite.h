#pragma once

#include "vector2d.h"
#include <stdbool.h>

typedef struct {
	Vector2D pos;
	// keeps track of initially passed position.
	Vector2D origin_pos;
	Vector2D size;
	unsigned int z_index;
	unsigned int texture_index;
	_Bool visible;
	// in case of a spritesheet, texture_size denotes size of a single
	// sprite from the atlas.
	Vector2D texture_size;
	// undefined if non-spritesheet texture
	unsigned int current_frame;
	unsigned int max_frame;
} Sprite;

void depth_sort(Sprite **sprites, unsigned int sprite_count);
_Bool is_sprite_within_bounds(const Sprite *sprite, const Vector2D point);
void sprite_hide(Sprite *sprite);
void sprite_show(Sprite *sprite);

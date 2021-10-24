#pragma once

#include "texture.h"
#include "util.h"

#include <stdbool.h>

typedef struct {
	Vector2D pos;
	Vector2D origin_pos; // keeps track of initially passed position.
	Vector2D size;
	unsigned int z_index;
	Texture *texture; // TODO this is only used during definition, hacky
	unsigned int texture_index; // 0 until fed into a scene initializer
	_Bool visible;
	_Bool is_spritesheet;
	// in case of a spritesheet, texture_size denotes size of a single
	// sprite from the atlas.
	Vector2D texture_size;
	unsigned int current_frame; // 0 if non-spritesheet texture
	unsigned int max_frame;
} Sprite;

void depth_sort(Sprite **sprites, unsigned int sprite_count);
_Bool is_sprite_within_bounds(const Sprite *sprite, const Vector2D point);

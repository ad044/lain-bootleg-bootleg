#include "sprite.h"
#include "engine.h"
#include "input.h"
#include "quad.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int depth_sort_cmp(const void *a, const void *b)
{
	const Sprite *sprite_a = *(Sprite **)a;
	const Sprite *sprite_b = *(Sprite **)b;
	return sprite_a->z_index - sprite_b->z_index;
}

void depth_sort(Sprite **sprites, unsigned int sprite_count)
{
	qsort(sprites, sprite_count, sizeof(Sprite *), depth_sort_cmp);
}

_Bool is_sprite_within_bounds(const Sprite *sprite, const Vector2D point)
{
	float left_x = sprite->pos.x;
	float right_x = sprite->pos.x + sprite->size.x;

	float top_y = sprite->pos.y;
	float bottom_y = sprite->pos.y + sprite->size.y;

	return (left_x <= point.x && point.x <= right_x) &&
	       (top_y <= point.y && point.y <= bottom_y);
}

void initialize_sprite(Sprite *sprite)
{
	sprite->origin_pos = sprite->pos;

	// if spritesheet, set size for single sprite (of what will be
	// displayed), and initialize current frame to 0
	if (sprite->is_spritesheet) {
		float size_x = 1.0f / ((float)sprite->max_frame + 1.0f);
		sprite->texture_size = (Vector2D){size_x, 1.0f};
		sprite->current_frame = 0;
	} else {
		sprite->texture_size = (Vector2D){1.0f, 1.0f};
	}
}

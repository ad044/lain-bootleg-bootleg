#include "sprite.h"
#include "engine.h"
#include "hashmap.h"
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

void sprite_hide(Sprite *sprite) { sprite->visible = false; }

void sprite_show(Sprite *sprite) { sprite->visible = true; }

void sprite_set_pos(Sprite *sprite, Vector2D pos) { sprite->pos = pos; }

void sprite_set_to_origin_pos(Sprite *sprite)
{
	sprite->pos = sprite->origin_pos;
}
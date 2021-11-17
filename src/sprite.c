#include "sprite.h"
#include "engine.h"
#include "vector2d.h"

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

void get_hitbox_range(Vector2D pos, Vector2D hitbox_size, float *top,
		      float *left, float *right, float *bottom)
{
	*left = pos.x - hitbox_size.x / 2.0f;
	*right = pos.x + hitbox_size.x / 2.0f;

	*top = pos.y - hitbox_size.y / 2.0f;
	*bottom = pos.y + hitbox_size.y / 2.0f;
}

_Bool is_sprite_within_bounds(const Sprite *sprite, const Vector2D point)
{
	float top, left, right, bottom;

	if (sprite->hitbox_size.x == 0 || sprite->hitbox_size.y == 0) {
		return false;
	}

	if (sprite->pivot_centered) {
		get_hitbox_range(sprite->pos, sprite->hitbox_size, &top, &left,
				 &right, &bottom);
	} else {
		Vector2D center_coords = get_sprite_center_coords(sprite);
		get_hitbox_range(center_coords, sprite->hitbox_size, &top,
				 &left, &right, &bottom);
	}

	return (left <= point.x && point.x <= right) &&
	       (top <= point.y && point.y <= bottom);
}

void init_sprite(Sprite *sprite)
{
	sprite->origin_pos = sprite->pos;

	// if spritesheet, set size for single sprite (of what will be
	// displayed), and initialize current frame to 0
	if (sprite->is_spritesheet) {
		float size_x = 1.0f / ((float)sprite->max_frame + 1.0f);
		sprite->texture_size = (Vector2D){size_x, 1.0f};
		sprite->frame_index = 0;
	} else {
		sprite->texture_size = (Vector2D){1.0f, 1.0f};
	}
}

Vector2D get_sprite_center_coords(const Sprite *sprite)
{
	if (sprite->pivot_centered) {
		return sprite->pos;
	} else {
		return (Vector2D){sprite->pos.x + sprite->size.x / 2.0f,
				  sprite->pos.y + sprite->size.y / 2.0f};
	}
}

GLfloat *get_sprite_vertices(GLfloat *buffer, Sprite *sprite)
{
	GLfloat vertices[] = {
	    // top right
	    sprite->pos.x + sprite->size.x,
	    sprite->pos.y,
	    sprite->frame_index * sprite->texture_size.x +
		sprite->texture_size.x * (sprite->mirrored ? -1.0f : 1.0f),
	    sprite->texture_size.y,
	    sprite->texture_index,

	    // bottom right
	    sprite->pos.x + sprite->size.x,
	    sprite->pos.y + sprite->size.y,
	    sprite->frame_index * sprite->texture_size.x +
		sprite->texture_size.x * (sprite->mirrored ? -1.0f : 1.0f),
	    0.0f,
	    sprite->texture_index,

	    // bottom left
	    sprite->pos.x,
	    sprite->pos.y + sprite->size.y,
	    sprite->frame_index * sprite->texture_size.x,
	    0.0f,
	    sprite->texture_index,

	    // top left
	    sprite->pos.x,
	    sprite->pos.y,
	    sprite->frame_index * sprite->texture_size.x,
	    sprite->texture_size.y,
	    sprite->texture_index,
	};

	memcpy(buffer, vertices, sizeof(vertices));
	buffer += sizeof(vertices) / sizeof(vertices[0]);
	return buffer;
}

GLfloat *get_pivot_centered_sprite_vertices(GLfloat *buffer, Sprite *sprite)
{
	GLfloat vertices[] = {
	    // top right
	    sprite->pos.x + (sprite->size.x / 2),
	    sprite->pos.y - (sprite->size.y / 2),
	    sprite->frame_index * sprite->texture_size.x +
		sprite->texture_size.x * (sprite->mirrored ? -1.0f : 1.0f),
	    sprite->texture_size.y,
	    sprite->texture_index,

	    // bottom right
	    sprite->pos.x + (sprite->size.x / 2),
	    sprite->pos.y + (sprite->size.y / 2),
	    sprite->frame_index * sprite->texture_size.x +
		sprite->texture_size.x * (sprite->mirrored ? -1.0f : 1.0f),
	    0.0f,
	    sprite->texture_index,

	    // bottom left
	    sprite->pos.x - (sprite->size.x / 2),
	    sprite->pos.y + (sprite->size.y / 2),
	    sprite->frame_index * sprite->texture_size.x,
	    0.0f,
	    sprite->texture_index,

	    // top left
	    sprite->pos.x - (sprite->size.x / 2),
	    sprite->pos.y - (sprite->size.y / 2),
	    sprite->frame_index * sprite->texture_size.x,
	    sprite->texture_size.y,
	    sprite->texture_index,
	};

	memcpy(buffer, vertices, sizeof(vertices));
	buffer += sizeof(vertices) / sizeof(vertices[0]);
	return buffer;
}

static Sprite make_transparent_sprite(GLfloat left, GLfloat top, GLfloat right,
				      GLfloat bottom, int8_t z_index)
{
	return (Sprite){
	    .pos = {left, top},
	    .size = {right - left, bottom - top},
	    .z_index = z_index,
	    .texture_index = -1,
	};
}

// a barrier block is simply a transparent sprite with an arbitrarily high z
// index. this way we can also visualize them to see whether its misplaced/etc.
Sprite make_click_barrier(GLfloat left, GLfloat top, GLfloat right,
			  GLfloat bottom)
{
	return make_transparent_sprite(left, top, right, bottom, 127);
}

_Bool sprite_is_max_frame(Sprite *sprite)
{
	return sprite->frame_index == sprite->max_frame;
}

void sprite_try_next_frame(double now, Sprite *sprite)
{
	if (sprite->animation_frame->next == NULL) {
		return;
	}

	if (now - sprite->last_updated >
	    sprite->animation_frame->timing / 60.0) {
		sprite->animation_frame = sprite->animation_frame->next;

		sprite->frame_index = sprite->animation_frame->frame_index;
		sprite->last_updated = now;
	}
}

void sprite_set_animation(Sprite *sprite, Animation *animation)
{
	sprite->animation = animation;
	sprite->animation_frame = animation->first;
}

void sprite_set_to_origin_pos(Sprite *sprite)
{
	sprite->pos = sprite->origin_pos;
}

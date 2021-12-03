#include "sprite.h"
#include "animations.h"
#include "engine.h"
#include "vector2d.h"

#include <stdint.h>
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

Vector2D get_sprite_center_coords(const Sprite *sprite)
{
	if (sprite->pivot_centered) {
		return sprite->pos;
	} else {
		return (Vector2D){
		    sprite->pos.x + sprite->texture->size.x / 2.0f,
		    sprite->pos.y + sprite->texture->size.y / 2.0f};
	}
}

GLfloat *get_sprite_vertices(GLfloat *buffer, Sprite *sprite)
{
	GLfloat vertices[] = {
	    // top right
	    sprite->pos.x + sprite->texture->size.x,
	    sprite->pos.y,
	    sprite->mirrored ? -1.0f : 1.0f,
	    1.0f,
	    sprite->texture_index,

	    // bottom right
	    sprite->pos.x + sprite->texture->size.x,
	    sprite->pos.y + sprite->texture->size.y,
	    sprite->mirrored ? -1.0f : 1.0f,
	    0.0f,
	    sprite->texture_index,

	    // bottom left
	    sprite->pos.x,
	    sprite->pos.y + sprite->texture->size.y,
	    0.0f,
	    0.0f,
	    sprite->texture_index,

	    // top left
	    sprite->pos.x,
	    sprite->pos.y,
	    0.0f,
	    1.0f,
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
	    sprite->pos.x + (sprite->texture->size.x / 2),
	    sprite->pos.y - (sprite->texture->size.y / 2),
	    sprite->mirrored ? -1.0f : 1.0f,
	    1.0f,
	    sprite->texture_index,

	    // bottom right
	    sprite->pos.x + (sprite->texture->size.x / 2),
	    sprite->pos.y + (sprite->texture->size.y / 2),
	    sprite->mirrored ? -1.0f : 1.0f,
	    0.0f,
	    sprite->texture_index,

	    // bottom left
	    sprite->pos.x - (sprite->texture->size.x / 2),
	    sprite->pos.y + (sprite->texture->size.y / 2),
	    0.0f,
	    0.0f,
	    sprite->texture_index,

	    // top left
	    sprite->pos.x - (sprite->texture->size.x / 2),
	    sprite->pos.y - (sprite->texture->size.y / 2),
	    0.0f,
	    1.0f,
	    sprite->texture_index,
	};

	memcpy(buffer, vertices, sizeof(vertices));
	buffer += sizeof(vertices) / sizeof(vertices[0]);
	return buffer;
}

_Bool sprite_animation_is_last_frame(Sprite *sprite)
{
	return sprite->animation->last == sprite->animation_frame;
}

static void sprite_set_frame(AnimationFrame *frame, Sprite *sprite)
{
	sprite->animation_frame = frame;

	if (sprite->animation_frame->visible) {
		sprite->visible = true;
		sprite->texture = sprite->animation_frame->texture;
		Vector2D pos_offset = sprite->animation_frame->pos_offset;

		if (pos_offset.x != -1) {
			sprite->pos.x = pos_offset.x;
		}

		if (pos_offset.y != -1) {
			sprite->pos.y = pos_offset.y;
		}
	} else {
		sprite->visible = false;
	}
}

void sprite_try_next_frame(double now, Sprite *sprite)
{
	if (sprite->animation_frame->next == NULL) {
		if (sprite->animation->looped) {
			sprite->animation_frame = sprite->animation->first;
		} else {
			sprite->animation = NULL;
			sprite->animation_frame = NULL;
		}
		return;
	}

	if (now - sprite->animation_start_time >
	    sprite->animation_frame->next->timing / 60.0) {
		sprite_set_frame(sprite->animation_frame->next, sprite);
	}
}

void sprite_set_animation_direct(double now, Sprite *sprite,
				 Animation *animation)
{
	sprite->animation = animation;
	sprite->animation_start_time = now;
	sprite_set_frame(animation->first, sprite);
}

void sprite_set_animation(Resources *resources, double now, Sprite *sprite,
			  AnimationID animation_id)
{
	Animation *animation = animation_get(resources, animation_id);
	sprite_set_animation_direct(now, sprite, animation);
}

void sprite_set_to_origin_pos(Sprite *sprite)
{
	sprite->pos = sprite->origin_pos;
}

void make_sprite(Sprite *target, Sprite sprite)
{
	*target = sprite;

	target->origin_pos = sprite.pos;
	target->animation = NULL;
	target->animation_frame = NULL;
}

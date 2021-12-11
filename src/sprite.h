#pragma once

#include "animations.h"
#include "sprite.h"
#include "texture.h"

#include <stdbool.h>
#include <stdint.h>

#define SPRITE_VERTEX_COUNT 4
#define SPRITE_INDEX_COUNT 6
#define ROWS_PER_SPRITE_VERTEX 5

#define SPRITE_VBO_SIZE SPRITE_VERTEX_COUNT *ROWS_PER_SPRITE_VERTEX

typedef struct {
	_Bool visible;
	_Bool mirrored;

	Vector2D pos;
	Vector2D origin_pos; // keeps track of initially passed position.
	unsigned int z_index;
	Texture *texture;
	int texture_index;
	Vector2D hitbox_size;
	_Bool pivot_centered; // if true, sprite's position points to its
			      // center. else, it points to the top left corner.
	Animation *animation;
	AnimationFrame *animation_frame;
	double animation_start_time;
} Sprite;

struct resources;

void depth_sort(Sprite **sprites, unsigned int sprite_count);
_Bool is_sprite_within_bounds(const Sprite *sprite, const Vector2D point);
Vector2D get_sprite_center_coords(const Sprite *sprite);
GLfloat *get_sprite_vertices(GLfloat *buffer, Sprite *sprite);
GLfloat *get_pivot_centered_sprite_vertices(GLfloat *buffer, Sprite *sprite);
void sprite_try_next_frame(struct resources *resources, double now, Sprite *sprite);
void sprite_set_animation(struct resources *resources, double now,
			  Sprite *sprite, AnimationID animation_id);
void sprite_set_animation_direct(struct resources *resources, double now,
				 Sprite *sprite, Animation *animation);
void get_hitbox_range(Vector2D pos, Vector2D hitbox_size, float *top,
		      float *left, float *right, float *bottom);
void sprite_set_to_origin_pos(Sprite *sprite);
void make_sprite(Sprite *target, Sprite sprite);
_Bool sprite_animation_is_last_frame(Sprite *sprite);

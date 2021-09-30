#pragma once

#include <GL/glew.h>
#include <cglm/types.h>
#include <stdbool.h>

typedef struct {
	GLfloat x;
	GLfloat y;
} Vector2D;
typedef struct {
	Vector2D pos;
	Vector2D size;
	unsigned int z_index;
	unsigned int texture_index;
	// optional parameters for when offsets/tiling are needed
	Vector2D texture_size;
	Vector2D texture_offset;
	char *name;
	void (*on_click)(void *);
} Sprite;

void generate_sprite_indices(GLuint *buffer, unsigned int index_count);
void update_sprite_vertices(Sprite *sprites, unsigned int sprite_count,
			    unsigned int index_count);
unsigned int get_sprite_index_count(unsigned int sprite_count);
void depth_sort(Sprite *sprites, unsigned int sprite_count);
_Bool is_sprite_within_bounds(const Sprite *sprite, const Vector2D *point);

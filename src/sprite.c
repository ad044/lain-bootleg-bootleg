#include "sprite.h"
#include "engine.h"
#include "hashmap.h"
#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUAD_VERTEX_COUNT 4
#define ROWS_PER_QUAD_VERTEX 5
#define QUAD_INDEX_COUNT 6

static int depth_sort_cmp(const void *a, const void *b);

void generate_sprite_indices(GLuint *buffer, unsigned int index_count)
{
	unsigned int offset = 0;
	for (int i = 0; i < index_count; i += 6) {
		GLuint indices[] = {
		    0 + offset, 1 + offset, 3 + offset,
		    1 + offset, 2 + offset, 3 + offset,

		};

		memcpy(buffer, indices, sizeof(indices));
		buffer += sizeof(indices) / sizeof(GLuint);

		offset += 4;
	};
}

GLfloat *get_sprite_vertices(GLfloat *buffer, Sprite *sprite)
{
	GLfloat vertices[] = {
	    // top right
	    sprite->pos.x + (sprite->size.x / 2),
	    sprite->pos.y + (sprite->size.y / 2),
	    sprite->current_frame * sprite->texture_size.x +
		sprite->texture_size.x,
	    sprite->texture_size.y,
	    sprite->texture_index,

	    // bottom right
	    sprite->pos.x + (sprite->size.x / 2),
	    sprite->pos.y - (sprite->size.y / 2),
	    sprite->current_frame * sprite->texture_size.x +
		sprite->texture_size.x,
	    0.0f,
	    sprite->texture_index,

	    // bottom left
	    sprite->pos.x - (sprite->size.x / 2),
	    sprite->pos.y - (sprite->size.y / 2),
	    sprite->current_frame * sprite->texture_size.x,
	    0.0f,
	    sprite->texture_index,

	    // top left
	    sprite->pos.x - (sprite->size.x / 2),
	    sprite->pos.y + (sprite->size.y / 2),
	    sprite->current_frame * sprite->texture_size.x,
	    sprite->texture_size.y,
	    sprite->texture_index,
	};

	memcpy(buffer, vertices, sizeof(vertices));
	buffer += sizeof(vertices) / sizeof(vertices[0]);
	return buffer;
}

void update_sprite_buffers(GLuint VBO, GLuint IBO, GLfloat *vertices,
			   GLfloat vertices_size, unsigned int sprite_count)
{
	// fill vbo
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);

	// calculate indices, fill ibo
	unsigned int index_count = get_sprite_index_count(sprite_count);

	GLuint indices[index_count];

	generate_sprite_indices(indices, index_count);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		     GL_DYNAMIC_DRAW);
}

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

unsigned int get_sprite_index_count(unsigned int sprite_count)
{
	return sprite_count * QUAD_INDEX_COUNT;
}

unsigned int get_sprite_vertex_buffer_size(unsigned int sprite_count)

{
	return sprite_count * ROWS_PER_QUAD_VERTEX * QUAD_VERTEX_COUNT;
}

_Bool is_sprite_within_bounds(const Sprite *sprite, const Vector2D point)
{
	float left_x = sprite->pos.x - sprite->size.x / 2;
	float right_x = sprite->pos.x + sprite->size.x / 2;

	float left_y = sprite->pos.y - sprite->size.y / 2;
	float right_y = sprite->pos.y + sprite->size.y / 2;

	return (left_x <= point.x && point.x <= right_x) &&
	       (left_y <= point.y && point.y <= right_y);
}

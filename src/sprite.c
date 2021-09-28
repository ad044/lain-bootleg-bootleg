#include "sprite.h"

#include <stdlib.h>
#include <string.h>

static GLfloat *get_sprite_vertices(GLfloat *buffer, Sprite *sprite);
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

static GLfloat *get_sprite_vertices(GLfloat *buffer, Sprite *sprite)
{
	GLfloat vertices[] = {
	    // top right
	    sprite->pos.x + (sprite->size.x / 2),
	    sprite->pos.y + (sprite->size.y / 2),
	    sprite->texture_offset.x +
		(sprite->texture_size.x ? 1.0f / sprite->texture_size.x : 1.0f),
	    sprite->texture_offset.y +
		(sprite->texture_size.y ? 1.0f / sprite->texture_size.y : 1.0f),
	    sprite->texture_index,

	    // bottom right
	    sprite->pos.x + (sprite->size.x / 2),
	    sprite->pos.y - (sprite->size.y / 2),
	    sprite->texture_offset.x +
		(sprite->texture_size.x ? 1.0f / sprite->texture_size.x : 1.0f),
	    sprite->texture_offset.y,
	    sprite->texture_index,

	    // bottom left
	    sprite->pos.x - (sprite->size.x / 2),
	    sprite->pos.y - (sprite->size.y / 2),
	    sprite->texture_offset.x,
	    sprite->texture_offset.y,
	    sprite->texture_index,

	    // top left
	    sprite->pos.x - (sprite->size.x / 2),
	    sprite->pos.y + (sprite->size.y / 2),
	    sprite->texture_offset.x,
	    sprite->texture_offset.y +
		(sprite->texture_size.y ? 1.0f / sprite->texture_size.y : 1.0f),
	    sprite->texture_index,
	};

	memcpy(buffer, vertices, sizeof(vertices));
	buffer += sizeof(vertices) / sizeof(vertices[0]);
	return buffer;
}

// loads sprite data into vbo/ibo
void fill_sprite_buffer_data(Sprite *sprites, unsigned int sprite_count,
			     unsigned int index_count)
{
	unsigned int vertex_buffer_size = sprite_count * 4 * 5;

	GLfloat vertices[vertex_buffer_size];
	GLfloat *buffer_p = vertices;
	for (int i = 0; i < sprite_count; i++) {
		buffer_p = get_sprite_vertices(buffer_p, &sprites[i]);
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		     GL_STATIC_DRAW);

	GLuint indices[index_count];
	generate_sprite_indices(indices, index_count);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		     GL_STATIC_DRAW);
}

static int depth_sort_cmp(const void *a, const void *b)
{
	return ((Sprite *)a)->z_index - ((Sprite *)b)->z_index;
}

void depth_sort(Sprite *sprites, unsigned int sprite_count)
{
	qsort(sprites, sprite_count, sizeof(Sprite), depth_sort_cmp);
}

unsigned int get_sprite_index_count(unsigned int sprite_count)
{
	return sprite_count * 6;
}

_Bool is_sprite_within_bounds(Sprite sprite, Vector2D point)
{
	float left_x_bound = sprite.pos.x - sprite.size.x / 2;
	float right_x_bound = sprite.pos.x + sprite.size.x / 2;

	float left_y_bound = sprite.pos.y - sprite.size.y / 2;
	float right_y_bound = sprite.pos.y + sprite.size.y / 2;

	return (left_x_bound <= point.x && point.x <= right_x_bound) &&
	       (left_y_bound <= point.y && point.y <= right_y_bound);
}

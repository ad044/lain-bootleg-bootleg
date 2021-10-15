#include "quad.h"

#include <string.h>

void generate_quad_indices(GLuint *buffer, unsigned int index_count)
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

unsigned int get_quad_index_count(unsigned int quad_count)
{
	return quad_count * QUAD_INDEX_COUNT;
}

GLfloat *get_quad_vertices(GLfloat *buffer, Sprite *sprite)
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

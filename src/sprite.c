#include "sprite.h"

#include <string.h>
#include <stdlib.h>

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

void init_sprite_buffers(GLuint *VAO, GLuint *VBO, GLuint *IBO)
{
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);

	glGenBuffers(1, IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
			      (void *)0);
	// texture coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
			      (void *)(2 * sizeof(GLfloat)));

	// texture id
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
			      (void *)(4 * sizeof(GLfloat)));
}

static GLfloat *get_sprite_vertices(GLfloat *buffer, Sprite *sprite)
{
	GLfloat vertices[] = {
	    // top right
	    sprite->pos.x + sprite->size.x,
	    sprite->pos.y + sprite->size.y,
	    sprite->texture_offset.x +
		(sprite->texture_size.x ? 1.0f / sprite->texture_size.x : 1.0f),
	    sprite->texture_offset.y +
		(sprite->texture_size.y ? 1.0f / sprite->texture_size.y : 1.0f),
	    sprite->texture_index,

	    // bottom right
	    sprite->pos.x + sprite->size.x,
	    sprite->pos.y,
	    sprite->texture_offset.x +
		(sprite->texture_size.x ? 1.0f / sprite->texture_size.x : 1.0f),
	    sprite->texture_offset.y,
	    sprite->texture_index,

	    // bottom left
	    sprite->pos.x,
	    sprite->pos.y,
	    sprite->texture_offset.x,
	    sprite->texture_offset.y,
	    sprite->texture_index,

	    // top left
	    sprite->pos.x,
	    sprite->pos.y + sprite->size.y,
	    sprite->texture_offset.x,
	    sprite->texture_offset.y +
		(sprite->texture_size.y ? 1.0f / sprite->texture_size.y : 1.0f),
	    sprite->texture_index,
	};

	memcpy(buffer, vertices, sizeof(vertices));
	buffer += sizeof(vertices) / sizeof(vertices[0]);
	return buffer;
}

// translates array of sprite definitions into vertex buffer
void load_sprite_vertex_data(GLfloat *buffer, Sprite *sprites,
			     unsigned int sprite_count)
{
	for (int i = 0; i < sprite_count; i++) {
		buffer = get_sprite_vertices(buffer, &sprites[i]);
	};
}

// loads sprite data into vbo/ibo
void fill_buffer_data(Sprite *sprites, unsigned int sprite_count,
		      unsigned int index_count)
{
	unsigned int vertex_buffer_size = sprite_count * 4 * 5;

	GLfloat vertices[vertex_buffer_size];
	load_sprite_vertex_data(vertices, sprites, sprite_count);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		     GL_STATIC_DRAW);

	GLuint indices[index_count];
	generate_sprite_indices(indices, index_count);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		     GL_STATIC_DRAW);
}

static int depth_sort_cmp(const void *a, const void *b)
{
	return ((Sprite*)a)->z_index - ((Sprite*)b)->z_index;
}

void depth_sort(Sprite *sprites, unsigned int sprite_count)
{
	qsort(sprites, sprite_count, sizeof(Sprite), depth_sort_cmp);
}

unsigned int get_sprite_index_count(unsigned int sprite_count)
{
	return sprite_count * 6;
}

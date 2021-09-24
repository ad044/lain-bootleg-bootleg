#include "sprite.h"

void init_sprite_vao(GLuint *VAO)
{
}

void generate_sprite_indices(GLuint *buffer, unsigned int index_count)
{
	unsigned int offset = 0;
	for (int i = 0; i < index_count; i += 6) {
		buffer[i + 0] = 0 + offset;
		buffer[i + 1] = 1 + offset;
		buffer[i + 2] = 3 + offset;

		buffer[i + 3] = 1 + offset;
		buffer[i + 4] = 2 + offset;
		buffer[i + 5] = 3 + offset;

		offset += 4;
	};
}

// translates array of sprite definitions into vertex buffer
void load_sprite_vertex_data(GLfloat *buffer, Sprite *sprites,
			     unsigned int sprite_count)
{
	unsigned int offset = 0;
	for (int i = 0; i < sprite_count; i++) {
		Sprite curr = sprites[i];
		// top right
		buffer[offset + 0] = curr.pos[0] + curr.size[0];
		buffer[offset + 1] = curr.pos[1] + curr.size[1];
		buffer[offset + 2] =
		    curr.texture_offset[0] +
		    (curr.texture_size[0] ? 1.0f / curr.texture_size[0] : 1.0f);
		buffer[offset + 3] =
		    curr.texture_offset[1] +
		    (curr.texture_size[1] ? 1.0f / curr.texture_size[1] : 1.0f);
		buffer[offset + 4] = curr.texture_index;

		// bottom right
		buffer[offset + 5] = curr.pos[0] + curr.size[0];
		buffer[offset + 6] = curr.pos[1];
		buffer[offset + 7] =
		    curr.texture_offset[0] +
		    (curr.texture_size[0] ? 1.0f / curr.texture_size[0] : 1.0f);
		buffer[offset + 8] = curr.texture_offset[1];
		buffer[offset + 9] = curr.texture_index;

		// bottom left
		buffer[offset + 10] = curr.pos[0];
		buffer[offset + 11] = curr.pos[1];
		buffer[offset + 12] = curr.texture_offset[0];
		buffer[offset + 13] = curr.texture_offset[1];
		buffer[offset + 14] = curr.texture_index;

		// top left
		buffer[offset + 15] = curr.pos[0];
		buffer[offset + 16] = curr.pos[1] + curr.size[1];
		buffer[offset + 17] = curr.texture_offset[0];
		buffer[offset + 18] =
		    curr.texture_offset[1] +
		    (curr.texture_size[1] ? 1.0f / curr.texture_size[1] : 1.0f);
		buffer[offset + 19] = curr.texture_index;

		offset += 20;
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

unsigned int get_sprite_index_count(unsigned int sprite_count)
{
	return sprite_count * 6;
}

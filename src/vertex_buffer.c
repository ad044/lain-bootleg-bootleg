#include <cglm/types.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "vertex_buffer.h"

// some of the enums will be hardcoded so i dont have to clutter user code with
// gl things. will abstract as needed, if i wont need to, i wont.

void put_sprites_into_buffer(GLfloat *buffer, Sprite *sprites,
			     unsigned int sprite_count)
{
	unsigned int offset = 0;
	for (size_t i = 0; i < sprite_count; i++) {
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

void init_vertex_buffer(GLuint *VBO, const GLfloat *data, GLuint size)
{
	glGenBuffers(1, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

// enables by default
void set_vertex_attrib_pointer(GLuint index, GLint size, GLsizei stride,
			       const void *pointer)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
}

void bind_vertex_buffer(GLuint VBO) { glBindBuffer(GL_ARRAY_BUFFER, VBO); }

void unbind_vertex_buffer(GLuint VBO) { glBindBuffer(GL_ARRAY_BUFFER, 0); }

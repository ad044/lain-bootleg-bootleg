#include "index_buffer.h"

void generate_quad_indices(GLuint *buffer, unsigned int index_count)
{
	unsigned int offset = 0;
	for (size_t i = 0; i < index_count; i += 6) {
		buffer[i + 0] = 0 + offset;
		buffer[i + 1] = 1 + offset;
		buffer[i + 2] = 3 + offset;

		buffer[i + 3] = 1 + offset;
		buffer[i + 4] = 2 + offset;
		buffer[i + 5] = 3 + offset;

		offset += 4;
	};
}
void init_index_buffer(GLuint *IBO, const GLuint *data, GLuint size)
{
	glGenBuffers(1, IBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void bind_index_buffer(GLuint IBO)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
}

void unbind_index_buffer(GLuint IBO)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
}

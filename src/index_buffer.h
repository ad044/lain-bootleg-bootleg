#pragma once
#include "GL/glew.h"

void generate_quad_indices(GLuint *buffer, unsigned int index_count);
void init_index_buffer(GLuint *IBO, const GLuint *data, GLuint size);
void bind_index_buffer(GLuint IBO);
void unbind_index_buffer(GLuint IBO);

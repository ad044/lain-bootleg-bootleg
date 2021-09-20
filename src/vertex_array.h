#pragma once
#include "GL/glew.h"
#include <cglm/types.h>

void init_vertex_array(GLuint *VAO, GLfloat vertices[],
			    GLuint vertices_size, GLuint indices[],
			    GLuint indices_size);
void bind_vertex_array(GLuint VAO);
void unbind_vertex_array(GLuint VAO);
#pragma once
#include "GL/glew.h"
#include <cglm/types.h>

#include "scene.h"

void put_sprites_into_buffer(GLfloat *buffer, Sprite *sprites,
			     unsigned int sprite_count);
void init_vertex_buffer(GLuint *VBO, const GLfloat *data, GLuint size);
void set_vertex_attrib_pointer(GLuint index, GLint size, GLsizei stride,
			       const void *pointer);
void bind_vertex_buffer(GLuint VBO);
void unbind_vertex_buffer(GLuint VBO);

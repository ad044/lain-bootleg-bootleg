#pragma once

#include "sprite.h"

void generate_quad_indices(GLuint *buffer, unsigned int index_count);
unsigned int get_quad_index_count(unsigned int quad_count);
GLfloat *get_sprite_vertices(GLfloat *buffer, Sprite *sprite);

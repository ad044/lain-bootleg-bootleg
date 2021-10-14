#pragma once

#include "sprite.h"

#define QUAD_VERTEX_COUNT 4
#define QUAD_INDEX_COUNT 6
#define ROWS_PER_QUAD_VERTEX 5

#define QUAD_VBO_SIZE QUAD_VERTEX_COUNT *ROWS_PER_QUAD_VERTEX

void generate_quad_indices(GLuint *buffer, unsigned int index_count);
unsigned int get_quad_index_count(unsigned int quad_count);
GLfloat *get_quad_vertices(GLfloat *buffer, Sprite *sprite);
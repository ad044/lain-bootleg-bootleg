#pragma once

#include <GL/glew.h>
#include <cglm/types.h>

typedef struct {
	GLfloat x;
	GLfloat y;
} Vector2D;
typedef struct {
	Vector2D pos;
	Vector2D size;
	unsigned int z_index;
	unsigned int texture_index;
	// optional parameters for when offsets/tiling are needed
	Vector2D texture_size;
	Vector2D texture_offset;
} Sprite;

void generate_sprite_indices(GLuint *buffer, unsigned int index_count);
void load_sprite_vertex_data(GLfloat *buffer, Sprite *sprites,
			     unsigned int sprite_count);
void fill_buffer_data(Sprite *sprites, unsigned int sprite_count,
		      unsigned int index_count);
void init_sprite_buffers(GLuint *VAO, GLuint *VBO, GLuint *IBO);
unsigned int get_sprite_index_count(unsigned int sprite_count);
void depth_sort(Sprite *sprites, unsigned int sprite_count);

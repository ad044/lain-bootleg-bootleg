#pragma once

#include <GL/glew.h>
#include <cglm/types.h>
#include <stdbool.h>

typedef struct {
	GLfloat x;
	GLfloat y;
} Vector2D;
typedef struct {
	Vector2D pos;
	Vector2D size;
	Vector2D texture_size;
	Vector2D texture_offset;
	unsigned int z_index;
	unsigned int texture_index;
	void (*on_click)(void *);
} Sprite;

void depth_sort(Sprite **sprites, unsigned int sprite_count);
void generate_sprite_indices(GLuint *buffer, unsigned int index_count);
GLfloat *get_sprite_vertices(GLfloat *buffer, Sprite *sprite);
unsigned int get_sprite_index_count(unsigned int sprite_count);
unsigned int get_sprite_vertex_buffer_size(unsigned int sprite_count);
int init_sprite(Sprite *target, Sprite sprite);
int make_sprite(Sprite **target, Sprite sprite);
_Bool is_sprite_within_bounds(const Sprite *sprite, const Vector2D *point);
void update_sprite_buffers(GLuint VBO, GLuint IBO, GLfloat *vertices,
			   GLfloat vertices_size, unsigned int sprite_count);

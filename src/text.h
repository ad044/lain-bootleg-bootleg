#pragma once

#include "resource_cache.h"
#include "sprite.h"
#include "texture.h"

#include <GLFW/glfw3.h>

typedef struct {
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	Vector2D pos;
	ShaderProgram shader;
	Texture *texture;
	Vector2D glyph_size;
	Vector2D glyph_texture_size;
	// h_padding (horizontal padding) will be multiplied by the current
	// iterator value (i =1,2,3..)
	float h_padding;
	unsigned char *current_text;
	unsigned int sprite_count;
} Text;
int init_text_obj(Text **text_obj, Texture *texture,
		  ResourceCache *resource_cache);
void update_text(Text *text_obj, unsigned char *text,
		 unsigned int sprite_count);
_Bool text_obj_needs_update(Text *text_obj, unsigned char *text);
void draw_text(Text *text_obj, GLFWwindow *window);

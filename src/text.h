#pragma once

#include "resource_cache.h"
#include "sprite.h"
#include "texture.h"

#include <GLFW/glfw3.h>

typedef struct {
	Vector2D pos;
	unsigned int texture_index;
	unsigned int texture_glyph_count;
} TextDefinition;

typedef struct {
	Vector2D pos;
	ShaderProgram shader;
	unsigned int texture_index;
	Vector2D glyph_size;
	Vector2D glyph_texture_size;
	// h_padding (horizontal padding) will be multiplied by the current
	// iterator value (i =1,2,3..)
	float h_padding;
	unsigned int glyph_count;
	char *current_text;
} Text;

int init_text_obj(Text *text_obj, TextDefinition text_obj_def,
		  Texture *texture);
void set_text(Text *text_obj, char *text);
Sprite get_glyph(Text *text_obj, char letter, unsigned int nth);

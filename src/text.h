#pragma once

#include "resource_cache.h"
#include "texture.h"
#include "sprite.h"

typedef struct {
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	Vector2D pos;
	Vector2D size;
	ShaderProgram shader;
	Texture *texture;
	unsigned char *current_text;
	unsigned int sprite_count;
} Text;
int init_text_obj(Text **text_obj, ShaderProgram shader, Texture *texture);
void update_text_obj(Text *text_obj, unsigned char *text);
void draw_text(Text *text_obj);

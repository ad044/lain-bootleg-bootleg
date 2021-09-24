#pragma once

#include "resource_cache.h"
#include "texture.h"
#include <GL/glew.h>

typedef struct {
	GLuint VAO;
	ShaderProgram shader;
	Texture *texture;
} TextSprite;
int init_text_sprite(TextSprite **text_sprite, ShaderProgram shader,
		     Texture *texture);
void draw_text(TextSprite *text_sprite, char *time);

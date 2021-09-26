#pragma once

#include "shader.h"
#include "sprite.h"
#include "texture.h"

#include <cglm/types.h>

typedef struct {
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	ShaderProgram shader;
	Texture *textures;
	unsigned int texture_count;
	GLint *samplers;
	Sprite *sprites;
	unsigned int sprite_count;
} Scene;

int init_scene(Scene **scene, Sprite *sprites, unsigned int sprite_count,
	       Texture **textures, unsigned int texture_count,
	       ShaderProgram shader);
void draw_scene(Scene *scene);

#pragma once
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/types.h>

#include "texture.h"
#include "shader.h"

typedef struct {
  Texture texture;
  char *name;
  vec2 pos;
  vec2 size;
} Sprite;

Sprite make_sprite(TextureCache *cache, char *texture_name, char *name,
		   vec2 initial_pos, vec2 size);
void draw_sprite(Sprite *sprite, ShaderProgram shader, GLuint VAO);

unsigned int init_sprite_vao();

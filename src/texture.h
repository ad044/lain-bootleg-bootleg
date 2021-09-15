#pragma once
#include <stdint.h>
#include <GLFW/glfw3.h>

#include "hashmap.h"

typedef GLuint TextureID;
typedef struct {
  GLuint id;
  char *image_path;
} Texture;
typedef struct hashmap texture_cache;

int init_texture(Texture *texture);
Texture make_texture(char *image_path);
  

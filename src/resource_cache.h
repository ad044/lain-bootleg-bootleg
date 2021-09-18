#pragma once
#include "shader.h"
#include "texture.h"

typedef struct {
	ShaderCache *shaders;
	TextureCache *textures;
	unsigned int quad_VAO;
} ResourceCache;
void init_quad_vao();
int init_resource_cache(ResourceCache **resource_cache);

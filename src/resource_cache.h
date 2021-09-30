#pragma once
#include "shader.h"
#include "texture.h"

typedef struct {
	ShaderProgram *shaders;
	TextureCache *textures;
} ResourceCache;
int init_resource_cache(ResourceCache **resource_cache);

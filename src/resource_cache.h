#pragma once
#include "shader.h"
#include "texture.h"
#include "text.h"

typedef struct {
	ShaderProgram *shaders;
	TextureCache *textures;
	Font **fonts;
} ResourceCache;
int init_resource_cache(ResourceCache **resource_cache);

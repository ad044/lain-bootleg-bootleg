#pragma once
#include "shader.h"
#include "text.h"
#include "texture.h"

typedef struct {
	ShaderProgram shaders[MAX_SHADER_COUNT];
	Font fonts[2];
	TextureCache *textures;
} ResourceCache;

int init_resource_cache(ResourceCache *resource_cache);

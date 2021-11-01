#pragma once
#include "shader.h"
#include "text.h"
#include "texture.h"

typedef struct {
	ShaderProgram shaders[MAX_SHADER_COUNT];
	Font fonts[MAX_FONT_COUNT];
	Texture textures[MAX_TEXTURE_COUNT];
} Resources;

#pragma once

#include "animations.h"
#include "shader.h"
#include "text.h"

typedef struct {
	ShaderProgram shaders[MAX_SHADER_COUNT];
	Font fonts[MAX_FONT_COUNT];
	Texture textures[MAX_TEXTURE_COUNT];
	Animation animations[MAX_ANIMATION_COUNT];
} Resources;

#pragma once

#include "menu.h"
#include "minigame.h"
#include "scene.h"
#include "shader.h"
#include "state.h"
#include "texture.h"

typedef struct {
	ShaderProgram shaders[MAX_SHADER_COUNT];
	Font fonts[MAX_FONT_COUNT];
	Texture textures[MAX_TEXTURE_COUNT];

	GLFWwindow *main_window;
	Menu menu;
	GLFWwindow *minigame_window;
	Minigame minigame;
	GameState game_state;
} Engine;

int engine_init(Engine *engine);
void engine_run(Engine *engine);

#pragma once
#include <GLFW/glfw3.h>

#include "texture.h"
#include "scene.h"
#include "shader.h"
#include "timeutil.h"

typedef struct {
  ShaderCache *shaders;
  TextureCache *textures;
  Scene current_scene;
  unsigned int sprite_VAO;
  GLFWwindow *menu_window;
  GameTime current_time;
} Engine;

void engine_load_scene(Scene *scene, Engine *engine);
int engine_init(Engine *engine, GLFWwindow *menu_window);
void engine_render(Engine *engine);


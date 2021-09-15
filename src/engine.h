#pragma once
#include "texture.h"
#include "scene.h"
#include <GLFW/glfw3.h>
#include "shader.h"

typedef struct {
  shader_cache *shaders;
  Scene current_scene;
  unsigned int sprite_VAO;
  GLFWwindow *window;
} Engine;

void engine_load_scene(Scene *scene, Engine *engine);
int engine_init(Engine *engine, GLFWwindow *window);
void engine_render(Engine *engine);


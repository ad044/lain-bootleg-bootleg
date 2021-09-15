#pragma once
#include "sprite.h"

typedef struct {
    int sprite_count;
    Sprite *sprites;
} Scene;

Scene *get_initial_scene();
void draw_scene(Scene *scene, shader_cache *shaders, unsigned int VAO);


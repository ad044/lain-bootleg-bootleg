#pragma once
#include "shader.h"
#include "sprite.h"
#include "timeutil.h"

typedef struct {
    int sprite_count;
    Sprite *sprites;
} Scene;

Scene *get_menu();
void draw_scene(Scene *scene, ShaderCache *shaders, unsigned int VAO);
void update_menu(Scene *scene, GameTime *current_time);


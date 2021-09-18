#pragma once
#include "resource_cache.h"
#include "shader.h"
#include "sprite.h"
#include "timeutil.h"

typedef struct {
	SpriteMap *sprite_map;
} Scene;

//todo
void draw_scene(Scene *scene, ResourceCache *resource_cache);

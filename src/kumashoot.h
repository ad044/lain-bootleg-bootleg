#pragma once

#include "scene.h"
#include "sprite.h"

typedef struct {
	Scene scene;

	Sprite background;
	Sprite bush_overlay;
} KumaShoot;

void start_kumashoot(void *ctx, Sprite *clicked_sprite, Vector2D click_pos);

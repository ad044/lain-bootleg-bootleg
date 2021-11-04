#pragma once

#include "engine.h"
#include "minigame.h"
#include "scene.h"
#include "sprite.h"

typedef enum { CHARACTER_CLICK } KumaShootEvent;

typedef enum {
	WHITE_BEAR,
	BROWN_BEAR,
	YASUO,
	MIHO,
	MIKA,
	LAIN,
	LAIN_WITH_SCREWDRIVER
} KumaShootSpriteType;

typedef struct {
	_Bool vaporized;
	KumaShootSpriteType type;
	Sprite sprite;
	double animation_start_time;
	int dx;
	int dy;
} KumaShootSprite;

typedef struct {
	Scene scene;

	Sprite background;
	Sprite bush_overlay;

	KumaShootSprite characters[3];
} KumaShoot;

void start_kumashoot(Minigame *minigame, GLFWwindow **minigame_window,
		     GLFWwindow *main_window, Texture *textures);
void handle_kumashoot_event(KumaShootEvent event, KumaShootSprite *character,
			    Engine *engine);

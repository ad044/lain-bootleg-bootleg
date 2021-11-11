#pragma once

#include "engine.h"
#include "minigame.h"
#include "scene.h"
#include "sprite.h"

typedef enum { CHARACTER_CLICK } KumaShootEvent;

typedef enum { WHITE_BEAR, BROWN_BEAR } BearType;

typedef enum {
	NO_CHARACTER,
	YASUO,
	MIHO,
	MIKA,
	SCHOOL_LAIN,
	DEFAULT_LAIN,
	SCREWDRIVER_LAIN,
} CharacterType;

typedef struct {
	_Bool scored;
	_Bool exploding;
	_Bool is_smoke;

	CharacterType type;

	Sprite sprite;

	double time_revealed;
	double time_scored;

	int score_value;
} Character;

typedef struct {
	_Bool needs_reset;
	_Bool revealed;
	_Bool is_smoke;

	Character hidden_character;
	BearType type;

	Sprite sprite;

	int vel_x, vel_y;
} Bear;

typedef struct {
	Scene scene;

	Bear bears[3];
	Text score_displays[3];

	Sprite background;
	Sprite bush_overlay;
} KumaShoot;

void start_kumashoot(Resources *resources, GameState *game_state,
		     Minigame *minigame, GLFWwindow **minigame_window,
		     GLFWwindow *main_window);
void handle_kumashoot_event(KumaShootEvent event, Bear *bear, Engine *engine);

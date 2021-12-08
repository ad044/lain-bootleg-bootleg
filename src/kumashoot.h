#pragma once

#include "menu.h"
#include "scene.h"
#include "sprite.h"

typedef enum { CHARACTER_CLICK } KumaShootEvent;

typedef enum { BEAR_WHITE, BEAR_BROWN } BearType;

typedef enum {
	NO_CHARACTER,
	YASUO,
	MIHO,
	MIKA,
	SCHOOL_LAIN_STANDING,
	SCHOOL_LAIN,
	DEFAULT_LAIN,
	SCREWDRIVER_LAIN,
} CharacterType;

typedef struct {
	_Bool scored;
	_Bool is_smoke;
	_Bool has_additional_sprite;

	CharacterType type;

	Sprite sprite;
	Sprite additional_sprite;

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

struct minigame;
struct engine;

int start_kumashoot(Menu *menu, Resources *resources, GameState *game_state,
		    struct minigame *minigame, GLFWwindow **minigame_window,
		    GLFWwindow *main_window);
void handle_kumashoot_event(KumaShootEvent event, Bear *bear, struct engine *engine);
void update_kumashoot(Resources *resources, Menu *menu, GameState *game_state,
		      GLFWwindow *window, struct minigame *minigame);

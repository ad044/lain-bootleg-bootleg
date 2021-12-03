#pragma once

#include "animations.h"
#include "menu.h"
#include "minigame.h"
#include "texture.h"
#include "vector2d.h"

#include <stdint.h>

typedef enum {
	THEATER_CLASSROOM,
	THEATER_SCHOOL,
	THEATER_LAIN_ROOM_NIGHT,
	THEATER_ARISU_ROOM,
	THEATER_CYBERIA,
	THEATER_STREET,
	THEATER_BRIDGE
} TheaterType;

typedef struct {
	TheaterType type;

	uint8_t layer_count;
	Sprite layers[5];

	double last_updated;
	Scene scene;
} Theater;

int start_theater(Menu *menu, Resources *resources, GameState *game_state,
		  Minigame *minigame, GLFWwindow **minigame_window,
		  GLFWwindow *main_window);
void update_theater(Resources *resources, Menu *menu, GameState *game_state,
		    GLFWwindow *window, Minigame *minigame);

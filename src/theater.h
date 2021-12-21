#pragma once

#include "animations.h"
#include "menu.h"
#include "texture.h"
#include "vector2d.h"
#include "movie.h"

typedef enum {
	THEATER_CLASSROOM,
	THEATER_SCHOOL,
	THEATER_LAIN_ROOM_NIGHT,
	THEATER_ARISU_ROOM,
	THEATER_CYBERIA,
	THEATER_STREET,
	THEATER_BRIDGE,
	THEATER_MOVIE
} TheaterType;

typedef struct {
	TheaterType type;

	uint8_t layer_count;
	Sprite layers[5];

	double last_updated;
	Scene scene;

	Movie movie;
} Theater;

struct minigame;

int start_theater(Menu *menu, Resources *resources, GameState *game_state,
		  struct minigame *minigame, GLFWwindow **minigame_window,
		  GLFWwindow *main_window);
void update_theater(Resources *resources, Menu *menu, GameState *game_state,
		    GLFWwindow *window, struct minigame *minigame);

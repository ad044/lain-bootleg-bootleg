#pragma once

#include "menu.h"
#include "scene.h"
#include "state.h"

typedef enum { NO_MINIGAME, KUMASHOOT, DRESSUP, THEATER } MinigameType;

struct minigame;

typedef void (*UpdateMinigameFunc)(Resources *resources, Menu *menu,
				   GameState *game_state, GLFWwindow *window,
				   struct minigame *minigame);
typedef void (*StartMinigameFunc)(Menu *menu, Resources *resources,
				  GameState *game_state,
				  struct minigame *minigame,
				  GLFWwindow **minigame_window,
				  GLFWwindow *main_window);

struct minigame {
	MinigameType type;
	UpdateMinigameFunc update;
	Scene *scene;
	void *current;

	double last_updated;

	StartMinigameFunc queued_start;
};

typedef struct minigame Minigame;

void kill_minigame(Texture *textures, Menu *menu, Minigame *minigame,
		   GLFWwindow *minigame_window);
_Bool can_refresh(double time, Minigame *minigame);

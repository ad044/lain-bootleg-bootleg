#pragma once

#include "menu.h"
#include "scene.h"
#include "state.h"

typedef enum { NO_MINIGAME, KUMASHOOT, DRESSUP, THEATER } MinigameType;

struct minigame;

struct minigame {
	MinigameType type;
	Scene *scene;
	void *current;

	double last_updated;

	MinigameType queued_minigame;
};

typedef struct minigame Minigame;

void kill_minigame(Texture *textures, Menu *menu, Minigame *minigame,
		   GLFWwindow *minigame_window);
void free_minigame(Minigame *minigame, GLFWwindow *minigame_window);
_Bool can_refresh(double time, Minigame *minigame);

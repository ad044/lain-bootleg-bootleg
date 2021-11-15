#pragma once

#include "vector2d.h"
typedef enum {
	DEFAULT_OUTFIT,
	PAJAMA_OUTFIT,
	CYBERIA_OUTFIT,
	ALIEN_OUTFIT,
	SCHOOL_OUTFIT,
	BEAR_OUTFIT,
} LainOutfit;

typedef struct {
	int score;
	double time;
	LainOutfit lain_outfit;
} GameState;

int init_game_state(GameState *game_state);

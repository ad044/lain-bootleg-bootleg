#pragma once

typedef struct {
	int score;
	double time;
} GameState;

int init_game_state(GameState *game_state);

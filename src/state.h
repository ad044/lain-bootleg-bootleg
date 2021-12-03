#pragma once

#include "resources.h"
#include "texture.h"
#include "vector2d.h"

typedef enum { NO_TOOLS, HOLDING_SCREWDRIVER, HOLDING_NAVI } LainToolState;

typedef enum {
	OUTFIT_DEFAULT,
	OUTFIT_SCHOOL,
	OUTFIT_CYBERIA,
	OUTFIT_BEAR,
	OUTFIT_SWEATER,
	OUTFIT_ALIEN,
} LainOutfit;

typedef struct {
	LainToolState tool_state;
	LainOutfit outfit;
	Texture *standing_texture;
	int walk_animation;
	int leave_animation;
} Lain;

typedef struct {
	int score;
	double time;
	Lain lain;
} GameState;

struct engine;
int init_game_state(Resources *resources, GameState *game_state);
int write_save_file(struct engine *engine);
int load_save_file(struct engine *engine);

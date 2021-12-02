#include "state.h"
#include "dressup.h"

int init_game_state(Resources *resources, GameState *game_state)
{
	game_state->score = 0;

	game_state->lain = (Lain){0};
	lain_set_outfit(resources, OUTFIT_DEFAULT, &game_state->lain);

	return 1;
}

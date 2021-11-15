#include "state.h"

int init_game_state(GameState *game_state)
{
	game_state->score = 5000;
	game_state->lain_outfit = CYBERIA_OUTFIT;
	return 1;
}

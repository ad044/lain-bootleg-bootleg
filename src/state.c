#include "state.h"

int init_game_state(GameState *game_state)
{
	game_state->score = 5000;
	game_state->lain_outfit = DEFAULT_OUTFIT;
	game_state->lain_tool_state = NO_TOOLS;
	return 1;
}

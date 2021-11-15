#include "minigame.h"

void kill_minigame(Menu *menu, Minigame *minigame, GLFWwindow **minigame_window,
		   Texture *textures)
{
	minigame->type = NONE;

	menu->bear_icon.texture = &textures[BEAR_ICON_INACTIVE];
	menu->dressup_button.texture = &textures[DRESSUP_BUTTON_INACTIVE];

	glfwDestroyWindow(*minigame_window);

	free(minigame->current);
}

_Bool can_refresh(double time, Minigame *minigame)
{
	return time - minigame->last_updated > 1.0 / minigame->refresh_rate;
}

#include "minigame.h"

void kill_minigame(Texture *textures, Menu *menu, Minigame *minigame,
		   GLFWwindow *minigame_window)
{
	minigame->type = NO_MINIGAME;

	menu->bear_icon.texture = &textures[BEAR_ICON];
	menu->dressup_button.texture = &textures[DRESSUP_BUTTON];

	glfwDestroyWindow(minigame_window);

	free(minigame->current);
}

_Bool can_refresh(double time, Minigame *minigame)
{
	return time - minigame->last_updated > 1.0 / 25.0;
}

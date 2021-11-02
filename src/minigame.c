#include "minigame.h"

void kill_minigame(Menu *menu, Minigame *minigame, GLFWwindow **minigame_window,
		   Texture *textures)
{
	minigame->type = NONE;

	menu->bear_icon.texture = &textures[BEAR_ICON_INACTIVE];

	glfwDestroyWindow(*minigame_window);

	free(minigame->current);
}

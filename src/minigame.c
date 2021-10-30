#include "minigame.h"

void kill_minigame(Minigame *minigame, GLFWwindow **minigame_window)
{
	minigame->running = false;

	glfwDestroyWindow(*minigame_window);

	free(minigame->current);
}

#include "minigame.h"

void kill_minigame(GLFWwindow **minigame_window, Minigame *minigame)
{
	glfwDestroyWindow(*minigame_window);
	*minigame_window = NULL;
}

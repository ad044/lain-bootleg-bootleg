#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "window.h"

#define SHRINKED_MENU_WIDTH 161
#define SHRINKED_MENU_HEIGHT 74

int make_menu_window(GLFWwindow **window)
{
	if (!(make_window(window, SHRINKED_MENU_WIDTH, SHRINKED_MENU_HEIGHT,
			  "lain"))) {
		printf("Failed to start menu.\n");
		return 0;
	}

	return 1;
}

int make_window(GLFWwindow **window, int width, int height, char *name)
{
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	*window = glfwCreateWindow(width, height, name, NULL, NULL);

	if (window == NULL) {
		printf("Failed to initialize window.\n");
		glfwTerminate();
		return 0;
	}

	glfwMakeContextCurrent(*window);
	glfwSwapInterval(1);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize glew.\n");
		glfwTerminate();
		return 0;
	}

	return 1;
}

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "scene.h"
#include "window.h"

#define SHRINKED_MENU_WIDTH 161
#define SHRINKED_MENU_HEIGHT 74

#define EXPANDED_MENU_WIDTH 196
#define EXPANDED_MENU_HEIGHT 196

static void GLAPIENTRY gl_debug_message_callback(GLenum source, GLenum type,
						 GLuint id, GLenum severity,
						 GLsizei length,
						 const GLchar *message,
						 const void *user_param)
{
	fprintf(stderr, "\ntype = 0x%x, severity = 0x%x, message = %s\n", type,
		severity, message);
}

int make_menu_window(GLFWwindow **window)
{
	if (!(make_window(window, SHRINKED_MENU_WIDTH, SHRINKED_MENU_HEIGHT,
			  "lain"))) {
		printf("Failed to start menu.\n");
		return 0;
	}

	return 1;
}

void expand_main_window(Engine *engine)
{
	glfwSetWindowSize(engine->main_window, EXPANDED_MENU_WIDTH,
			  EXPANDED_MENU_HEIGHT);
	glViewport(0, 0, EXPANDED_MENU_WIDTH, EXPANDED_MENU_HEIGHT);

	if (!(load_expanded_menu_scene(&engine->menu->scene,
				       engine->resource_cache))) {
		printf("Failed to load maximized menu.\n");
		exit(1);
	};
}

void shrink_main_window(Engine *engine)
{
	glfwSetWindowSize(engine->main_window, SHRINKED_MENU_WIDTH,
			  SHRINKED_MENU_HEIGHT);
	glViewport(0, 0, SHRINKED_MENU_WIDTH, SHRINKED_MENU_HEIGHT);

	if (!(load_shrinked_menu_scene(&engine->menu->scene,
				       engine->resource_cache))) {
		printf("Failed to load minimized menu.\n");
		exit(1);
	};
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

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_debug_message_callback, 0);

	return 1;
}

#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <string.h>

#include "engine.h"
#include "hashmap.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"

#define INIT_WIDTH 161 // 161
#define INIT_HEIGHT 74 // 74

void error_callback(int error, const char *description)
{
	fprintf(stderr, "Code: %d\nError: %s\n", error, description);
}

void GLAPIENTRY gl_debug_message_callback(GLenum source, GLenum type, GLuint id,
					  GLenum severity, GLsizei length,
					  const GLchar *message,
					  const void *userParam)
{
	fprintf(stderr, "\ntype = 0x%x, severity = 0x%x, message = %s\n", type,
		severity, message);
}

int main(void)
{
	if (!glfwInit()) {
		printf("Failed to initialize glfw.\n");
		return -1;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow *menu_window =
	    glfwCreateWindow(INIT_WIDTH, INIT_HEIGHT, "lain", NULL, NULL);
	if (!menu_window) {
		printf("Failed to initialize window.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(menu_window);
	glfwSwapInterval(1);
	glfwSetErrorCallback(error_callback);
	glEnable(GL_DEBUG_OUTPUT);

	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize glew.\n");
		glfwTerminate();
		return -1;
	}

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set debug message cb
	glDebugMessageCallback(gl_debug_message_callback, 0);

	Engine engine;
	if (!engine_init(&engine, menu_window)) {
		printf("Failed to initialize engine. Exiting.\n");
		return -1;
	};

	while (!glfwWindowShouldClose(menu_window)) {
		engine_render(&engine);
	}

	glfwTerminate();
	return 0;
}

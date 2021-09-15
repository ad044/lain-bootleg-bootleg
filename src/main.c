#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <string.h>

#include "engine.h"
#include "hashmap.h"
#include "scene.h"
#include "shader.h"
#include "sprite.h"
#include "texture.h"

#define INIT_WIDTH 161 // 161
#define INIT_HEIGHT 74 // 74

void error_callback(int error, const char *description)
{
	fprintf(stderr, "Code: %d\nError: %s\n", error, description);
}

int main(void)
{
	if (!glfwInit()) {
		printf("Failed to initialize glfw.\n");
		return -1;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow *window =
	    glfwCreateWindow(INIT_WIDTH, INIT_HEIGHT, "lain", NULL, NULL);
	if (!window) {
		printf("Failed to initialize window.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetErrorCallback(error_callback);

	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize glew.\n");
		glfwTerminate();
		return -1;
	}

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Engine engine;
	if (!engine_init(&engine, window)) {
		printf("Failed to initialize engine. Exiting.\n");
		return -1;
	};

	while (!glfwWindowShouldClose(window)) {
		engine_render(&engine);
	}

	glfwTerminate();
	return 0;
}

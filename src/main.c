#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <string.h>

#include "engine.h"
#include "hashmap.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"

static void error_callback(int error, const char *description)
{
	fprintf(stderr, "Code: %d\nError: %s\n", error, description);
}

static int init_glfw()
{
	if (!glfwInit()) {
		printf("Failed to initialize glfw.\n");
		return -1;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwSetErrorCallback(error_callback);

	return 1;
}

int main(void)
{
	if (!init_glfw()) {
		return -1;
	}

	Engine engine;
	if (!engine_init(&engine)) {
		printf("Failed to initialize engine. Exiting.\n");
		return -1;
	};

	engine_run(&engine);

	return 0;
}

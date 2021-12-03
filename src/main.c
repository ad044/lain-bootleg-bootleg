#include <stdio.h>

#include "engine.h"

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

	engine_stop(&engine);

	return 0;
}

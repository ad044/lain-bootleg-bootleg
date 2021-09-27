#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <string.h>

#include "engine.h"
#include "hashmap.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"

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

	// set debug messages
	glfwSetErrorCallback(error_callback);
	glEnable(GL_DEBUG_OUTPUT);
	glfwSetErrorCallback(error_callback);

	Engine engine;
	if (!engine_init(&engine)) {
		printf("Failed to initialize engine. Exiting.\n");
		return -1;
	};

	engine_run(&engine);

	return 0;
}

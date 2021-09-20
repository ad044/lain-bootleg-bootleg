#include <stdio.h>
#include <time.h>

#include "engine.h"
#include "menu.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
				GLenum severity, GLsizei length,
				const GLchar *message, const void *userParam)
{
	fprintf(stderr, "\ntype = 0x%x, severity = 0x%x, message = %s\n", type,
		severity, message);
}

int engine_init(Engine *engine, GLFWwindow *menu_window)
{
	// initialize resource cache
	if (!(init_resource_cache(&engine->resource_cache))) {
		printf("Failed to initialize resource cache.\n");
		return 0;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	// set current context window inside engine
	engine->menu_window = menu_window;

	// init menu
	if (!(init_menu(engine->resource_cache, &engine->menu))) {
		printf("Failed to initialize menu.\n");
		return 0;
	}

	float endTime;
	float timeElapsed;

	return 1;
}

void engine_render(Engine *engine)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	update_menu(engine->menu);
	draw_scene(engine->menu->scene);

	glfwPollEvents();
	glfwSwapBuffers(engine->menu_window);
}

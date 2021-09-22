#include <stdio.h>

#include "engine.h"
#include "menu.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"

int engine_init(Engine *engine, GLFWwindow *menu_window)
{
	// initialize resource cache
	if (!(init_resource_cache(&engine->resource_cache))) {
		printf("Failed to initialize resource cache.\n");
		return 0;
	}

	// set current context window inside engine
	engine->menu_window = menu_window;

	// init menu
	if (!(init_menu(engine->resource_cache, &engine->menu))) {
		printf("Failed to initialize menu.\n");
		return 0;
	}

	return 1;
}

void engine_render(Engine *engine)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	update_menu(engine->menu);
	draw_menu(engine->resource_cache, engine->menu);

	glfwPollEvents();
	glfwSwapBuffers(engine->menu_window);
}

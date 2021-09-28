#include <stdio.h>

#include "engine.h"
#include "input.h"
#include "menu.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"
#include "window.h"

static void engine_renderloop(Engine *engine);
// todo
static void engine_stop(Engine *engine);

int engine_init(Engine *engine)
{
	// init menu window
	if (!(make_menu_window(&engine->menu_window))) {
		printf("Failed to create menu window.\n");
		return 0;
	}

	// initialize resource cache
	if (!(init_resource_cache(&engine->resource_cache))) {
		printf("Failed to initialize resource cache.\n");
		return 0;
	}

	// init menu
	if (!(init_menu(engine->resource_cache, &engine->menu))) {
		printf("Failed to initialize menu.\n");
		return 0;
	}

	// set sticky buttons to not miss state
	glfwSetInputMode(engine->menu_window, GLFW_STICKY_MOUSE_BUTTONS,
			 GLFW_TRUE);

	return 1;
}

static void engine_renderloop(Engine *engine)
{
	while (!glfwWindowShouldClose(engine->menu_window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		update_menu(engine->menu);
		handle_menu_click(engine->menu, engine->menu_window);
		draw_menu(engine->resource_cache, engine->menu);

		glfwPollEvents();
		glfwSwapBuffers(engine->menu_window);
	}
}

void engine_run(Engine *engine)
{
	engine_renderloop(engine);
	glfwTerminate();
}

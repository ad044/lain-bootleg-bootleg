#include <stdio.h>
#include <time.h>

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

#define FRAMERATE_CAP 0.033

int engine_init(Engine *engine)
{
	// init main (menu) window
	if (!(make_main_window(&engine->main_window))) {
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

	// set user pointer to access engine inside callback function
	glfwSetWindowUserPointer(engine->main_window, engine);
	// set callbacks
	glfwSetMouseButtonCallback(engine->main_window, handle_menu_click);

	return 1;
}

static void engine_renderloop(Engine *engine)
{
	double last_frame_time = glfwGetTime();

	while (!glfwWindowShouldClose(engine->main_window)) {
		double curr_time = glfwGetTime();
		double delta = curr_time - last_frame_time;
		if (delta >= FRAMERATE_CAP) {
			last_frame_time = curr_time;
			// temporary benchmark to see how changes affect things
			float startTime = (float)clock() / CLOCKS_PER_SEC;

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			update_menu(engine->menu);
			draw_menu(engine->menu, engine->main_window);

			glfwPollEvents();
			glfwSwapBuffers(engine->main_window);

			float endTime = (float)clock() / CLOCKS_PER_SEC;
			/* printf("%f\n", endTime - startTime); */
		}
	}
}

void engine_run(Engine *engine)
{
	engine_renderloop(engine);
	glfwTerminate();
}

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
	if (!(make_window(&engine->main_window, SHRINKED_MENU_WIDTH,
			  SHRINKED_MENU_HEIGHT, "lain", NULL))) {
		printf("Failed to create main window.\n");
		return 0;
	}

	// initialize resource cache
	if (!(init_resource_cache(&engine->resource_cache))) {
		printf("Failed to initialize resource cache.\n");
		return 0;
	}

	// init game state
	engine->game_state = malloc(sizeof(GameState));
	if (engine->game_state == NULL) {
		printf("Failed to allocate memory for game state.\n");
		return 0;
	}
	if (!init_game_state(engine->game_state)) {
		printf("Failed to initialize game state.\n");
		return 0;
	}

	// init menu
	if (!(init_menu(engine->resource_cache, engine->game_state,
			&engine->menu))) {
		printf("Failed to initialize menu.\n");
		return 0;
	}

	// malloc minigame
	engine->minigame_window = NULL;
	engine->minigame = malloc(sizeof(Minigame));
	if (engine->minigame == NULL) {
		printf("Failed to allocate memory for minigame struct.\n");
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

			glfwMakeContextCurrent(engine->main_window);

			update_menu(engine->menu, engine->game_state,
				    engine->main_window,
				    engine->resource_cache);

			draw_scene(engine->menu->scene, engine->main_window);

			if (engine->minigame_window != NULL) {
				glfwMakeContextCurrent(engine->minigame_window);

				engine->minigame->updater(
				    engine->game_state,
				    engine->minigame->minigame_struct);

				draw_scene(engine->minigame->scene,
					   engine->minigame_window);
				glfwSwapBuffers(engine->minigame_window);
			}

			glfwPollEvents();
			glfwSwapBuffers(engine->main_window);

			float endTime = (float)clock() / CLOCKS_PER_SEC;
			// printf("%f\n", endTime - startTime);
		}
	}
}

void engine_run(Engine *engine)
{
	engine_renderloop(engine);
	glfwTerminate();
}

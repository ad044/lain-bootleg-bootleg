#include <stdio.h>
#include <time.h>

#include "engine.h"
#include "input.h"
#include "menu.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"
#include "window.h"

// todo
static void engine_stop(Engine *engine);

#define FRAMERATE_CAP 30

int engine_init(Engine *engine)
{
	// init main (menu) window
	if (!(make_window(&engine->main_window, SHRINKED_MENU_WIDTH,
			  SHRINKED_MENU_HEIGHT, "lain", NULL, false))) {
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

static void engine_render(Engine *engine)
{
	glfwMakeContextCurrent(engine->main_window);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	update_menu(engine->menu, engine->game_state, engine->main_window,
		    engine->resource_cache);

	draw_scene(engine->menu->scene, engine->main_window);

	glfwSwapBuffers(engine->main_window);

	if (engine->minigame_window != NULL) {
		if (glfwWindowShouldClose(engine->minigame_window)) {
			kill_minigame(&engine->minigame_window,
				      engine->minigame);
		} else {
			glfwMakeContextCurrent(engine->minigame_window);

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			engine->minigame->updater(
			    engine->game_state,
			    engine->minigame->minigame_struct);

			draw_scene(engine->minigame->scene,
				   engine->minigame_window);

			glfwSwapBuffers(engine->minigame_window);
		}
	}

	glfwPollEvents();
}

static void engine_renderloop(Engine *engine, float framerate)
{
	double last_frame_time = glfwGetTime();

	while (!glfwWindowShouldClose(engine->main_window)) {
		engine_render(engine);
		while (glfwGetTime() < last_frame_time + 1.0 / framerate)
			;
		last_frame_time += 1.0 / framerate;
	}
}

void engine_run(Engine *engine)
{
	engine_renderloop(engine, FRAMERATE_CAP);
	glfwTerminate();
}

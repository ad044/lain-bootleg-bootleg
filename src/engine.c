#include <stdio.h>
#include <time.h>

#include "animations.h"
#include "engine.h"
#include "menu.h"
#include "minigame.h"
#include "resources.h"
#include "scene.h"
#include "shader.h"
#include "state.h"
#include "texture.h"
#include "window.h"

#include "input.h"

// todo
static void engine_stop(Engine *engine);

int engine_init(Engine *engine)
{
	// init main (menu) window
	if (!(make_window(&engine->main_window, COLLAPSED_MENU_WIDTH,
			  COLLAPSED_MENU_HEIGHT, "lain", NULL, false))) {
		printf("Failed to create main window.\n");
		return 0;
	}

	init_game_state(&engine->resources, &engine->game_state);

	init_resources(&engine->resources);

	init_menu(&engine->menu, &engine->game_state, &engine->resources);

	engine->minigame_window = NULL;
	engine->minigame.queued_start = NULL;
	engine->minigame.type = NO_MINIGAME;

	// set user pointer to access engine inside callback function
	glfwSetWindowUserPointer(engine->main_window, engine);
	// set callbacks
	glfwSetMouseButtonCallback(engine->main_window, handle_menu_click);

	return 1;
}

static void engine_render(Engine *engine, double now)
{
	GLFWwindow *main_window = engine->main_window;
	Resources *resources = &engine->resources;
	Menu *menu = &engine->menu;
	GameState *game_state = &engine->game_state;

	GLFWwindow *minigame_window = engine->minigame_window;
	Minigame *minigame = &engine->minigame;

	glfwMakeContextCurrent(main_window);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	update_menu(menu, game_state, main_window, resources);

	draw_scene(&menu->scene, main_window,
		   resources->shaders[SPRITE_SHADER]);

	glfwSwapBuffers(main_window);

	if (minigame->type == NO_MINIGAME && minigame->queued_start != NULL) {
		minigame->queued_start(menu, resources, game_state, minigame,
				       &engine->minigame_window, main_window);
		minigame->queued_start = NULL;
	}

	if (minigame->type != NO_MINIGAME && can_refresh(now, minigame)) {
		glfwMakeContextCurrent(minigame_window);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		minigame->update(resources, menu, game_state, minigame_window,
				 minigame);

		if (minigame->type != NO_MINIGAME) {
			draw_scene(minigame->scene, minigame_window,
				   resources->shaders[SPRITE_SHADER]);

			glfwSwapBuffers(minigame_window);

			minigame->last_updated = now;
		}
	}

	glfwPollEvents();

	game_state->time = now;
}

static void engine_renderloop(Engine *engine)
{
	while (!glfwWindowShouldClose(engine->main_window)) {
		engine_render(engine, glfwGetTime());
	}
}

void engine_run(Engine *engine)
{
	if (!load_save_file(engine)) {
		printf("Found a save file, but failed to load it.\n");
		exit(1);
	};

	engine_renderloop(engine);

	if (!write_save_file(engine)) {
		printf("Failed to write save file.\n");
	};

	glfwTerminate();
}

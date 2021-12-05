#include <portaudio.h>
#include <stdio.h>
#include <time.h>

#include "animations.h"
#include "cJSON.h"
#include "cvector.h"
#include "dressup.h"
#include "engine.h"
#include "kumashoot.h"
#include "menu.h"
#include "minigame.h"
#include "resources.h"
#include "scene.h"
#include "shader.h"
#include "state.h"
#include "texture.h"
#include "theater.h"
#include "window.h"

#include "input.h"

int engine_init(Engine *engine)
{
	// init main (menu) window
	if (!(make_window(&engine->main_window, COLLAPSED_MENU_WIDTH,
			  COLLAPSED_MENU_HEIGHT, "lain", NULL, false))) {
		printf("Failed to create main window.\n");
		return 0;
	}

	if (!init_resources(&engine->resources)) {
		printf("Failed to initialize resources.\n");
		return 0;
	};

	init_game_state(&engine->resources, &engine->game_state);

	init_menu(&engine->menu, &engine->game_state, &engine->resources);

	engine->minigame_window = NULL;
	engine->minigame.queued_minigame = NO_MINIGAME;
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

	if (minigame->type != NO_MINIGAME && can_refresh(now, minigame)) {
		glfwMakeContextCurrent(minigame_window);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		switch (minigame->type) {
		case KUMASHOOT:
			update_kumashoot(resources, menu, game_state,
					 minigame_window, minigame);
			break;
		case DRESSUP:
			update_dressup(resources, menu, game_state,
				       minigame_window, minigame);
			break;
		case THEATER:
			update_theater(resources, menu, game_state,
				       minigame_window, minigame);
			break;
		default:
			break;
		}

		if (minigame->type != NO_MINIGAME) {
			draw_scene(minigame->scene, minigame_window,
				   resources->shaders[SPRITE_SHADER]);

			glfwSwapBuffers(minigame_window);

			minigame->last_updated = now;
		}
	}

	if (minigame->type == NO_MINIGAME &&
	    minigame->queued_minigame != NO_MINIGAME) {
		// NOTE:
		// start functions here return 0 if they fail
		// i am not quite sure what to do in that case :D
		switch (minigame->queued_minigame) {
		case KUMASHOOT:
			start_kumashoot(menu, resources, game_state, minigame,
					&engine->minigame_window, main_window);
			break;
		case DRESSUP:
			start_dressup(menu, resources, game_state, minigame,
				      &engine->minigame_window, main_window);
			break;
		case THEATER:
			start_theater(menu, resources, game_state, minigame,
				      &engine->minigame_window, main_window);
			break;
		default:
			break;
		}
		minigame->queued_minigame = NO_MINIGAME;
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

void engine_stop(Engine *engine)
{
	Resources *resources = &engine->resources;
	Menu *menu = &engine->menu;
	Minigame *minigame = &engine->minigame;

	cJSON_Delete(resources->animation_data);

	free_scene(&menu->scene);
	if (minigame->type != NO_MINIGAME) {
		free_minigame(minigame, engine->minigame_window);
	}

	for (int i = 0; i < MAX_ANIMATION_COUNT; i++) {
		animation_free(&resources->animations[i]);
	}

	Pa_Terminate();
	glfwTerminate();
}

void engine_run(Engine *engine)
{
	if (!load_save_file(engine)) {
		printf("Found a save file, but failed to load it. Starting "
		       "from a fresh save state.\n");
	};

	engine_renderloop(engine);

	if (!write_save_file(engine)) {
		printf("Failed to write save file.\n");
	};
}

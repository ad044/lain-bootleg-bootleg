#include <portaudio.h>
#include <pthread.h>
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
#include "sound.h"
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

	// init audio
	if (Pa_OpenDefaultStream(&engine->audio_stream, 0, 1, paFloat32, 22050,
				 paFramesPerBufferUnspecified, NULL,
				 NULL) != paNoError) {
		printf("Problem opening audio stream.\n");
		return 0;
	};
	Pa_SetStreamFinishedCallback(engine->audio_stream, close_audio_stream);

	pthread_mutex_init(&engine->running, NULL);
	pthread_mutex_lock(&engine->running);
	pthread_create(&engine->audio_thread, NULL, sound_loop, engine);

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

		update_minigame(resources, game_state, menu, minigame_window,
				minigame);

		if (minigame->type != NO_MINIGAME) {
			draw_minigame(resources, minigame_window, minigame);

			glfwSwapBuffers(minigame_window);

			minigame->last_updated = now;
		}
	}

	if (minigame->type == NO_MINIGAME &&
	    minigame->queued_minigame != NO_MINIGAME) {
		start_queued_minigame(resources, game_state, menu, main_window,
				      &engine->minigame_window, minigame);
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
	pthread_mutex_unlock(&engine->running);
	pthread_join(engine->audio_thread, NULL);

	Resources *resources = &engine->resources;
	Menu *menu = &engine->menu;
	Minigame *minigame = &engine->minigame;

	cJSON_Delete(resources->animation_data);

	if (minigame->type != NO_MINIGAME) {
		destroy_minigame(resources->textures, menu, minigame,
				 engine->minigame_window);
	}

	free_scene(&menu->scene);

	for (int i = 0; i < MAX_ANIMATION_COUNT; i++) {
		animation_free(&resources->animations[i]);
	}

	close_audio_stream(engine->audio_stream);
	for (int i = 0; i < SOUND_COUNT; i++) {
		sf_close(resources->sounds[i].file);
	}

	Pa_Terminate();
	glfwTerminate();
}

void engine_run(Engine *engine)
{
	engine_renderloop(engine);

	if (!write_save_file(engine)) {
		printf("Failed to write save file.\n");
	};
}

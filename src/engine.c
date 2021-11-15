#include <stdio.h>
#include <time.h>

#include "animations.h"
#include "engine.h"
#include "menu.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"
#include "window.h"

#include "input.h"

// todo
static void engine_stop(Engine *engine);

int engine_init(Engine *engine)
{
	// init main (menu) window
	if (!(make_window(&engine->main_window, SHRINKED_MENU_WIDTH,
			  SHRINKED_MENU_HEIGHT, "lain", NULL, false))) {
		printf("Failed to create main window.\n");
		return 0;
	}

	shaders_init(engine->resources.shaders);

	textures_init(engine->resources.textures);

	fonts_init(engine->resources.fonts, engine->resources.textures);

	init_game_state(&engine->game_state);

	init_menu(&engine->menu, &engine->game_state, &engine->resources);

	init_animations(engine->resources.animations);

	engine->minigame_window = NULL;
	engine->minigame.type = NONE;

	// set user pointer to access engine inside callback function
	glfwSetWindowUserPointer(engine->main_window, engine);
	// set callbacks
	glfwSetMouseButtonCallback(engine->main_window, handle_menu_click);

	return 1;
}

static void engine_render(Engine *engine, double now)
{
	glfwMakeContextCurrent(engine->main_window);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	update_menu(&engine->menu, &engine->game_state, engine->main_window,
		    &engine->resources);

	draw_scene(&engine->menu.scene, engine->main_window,
		   engine->resources.shaders[SPRITE_SHADER]);

	glfwSwapBuffers(engine->main_window);

	GLFWwindow *minigame_window = engine->minigame_window;
	Minigame *minigame = &engine->minigame;

	if (minigame->type != NONE && can_refresh(now, minigame)) {
		minigame->last_updated = now;
		if (glfwWindowShouldClose(minigame_window)) {
			kill_minigame(&engine->menu, minigame, &minigame_window,
				      engine->resources.textures);
		} else {
			glfwMakeContextCurrent(minigame_window);

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			minigame->update(&engine->resources,
					 &engine->game_state, minigame_window,
					 minigame->current);

			draw_scene(minigame->scene, minigame_window,
				   engine->resources.shaders[SPRITE_SHADER]);

			glfwSwapBuffers(minigame_window);
		}
	}

	glfwPollEvents();

	engine->game_state.time = now;
}

static void engine_renderloop(Engine *engine)
{
	while (!glfwWindowShouldClose(engine->main_window)) {
		engine_render(engine, glfwGetTime());
	}
}

void engine_run(Engine *engine)
{
	engine_renderloop(engine);
	glfwTerminate();
}

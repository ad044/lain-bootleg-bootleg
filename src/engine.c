#include <stdio.h>

#include "engine.h"
#include "scene.h"
#include "shader.h"
#include "sprite.h"
#include "texture.h"

void engine_load_scene(Scene *scene, Engine *engine)
{
	engine->current_scene = *scene;
}

int engine_init(Engine *engine, GLFWwindow *menu_window)
{
	// initialize shader cache
	if (!(shader_cache_init(&engine->shaders))) {
		printf("Failed to initialize shader cache.\n");
		return 0;
	}

	// initialize texture cache and fill it
	// (for now i will try to preload every texture, which is currently)
	// around 9 mb. if this proves to be problematic we can change the
	// approach.
	if (!(texture_cache_init(&engine->textures))) {
		printf("Failed to initialize texture cache.\n");
		return 0;
	}
	preload_textures(engine->textures);

	// initialize sprite vao
	engine->sprite_VAO = init_sprite_vao();

	// set current context window inside engine
	engine->menu_window = menu_window;

	// initialize gametime
	engine->current_time = get_current_time();

	// load menu
	Scene *scene = get_menu(engine->textures);
	engine_load_scene(scene, engine);

	return 1;
}

void engine_render(Engine *engine)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	update_time(&engine->current_time);

	draw_scene(&engine->current_scene, engine->shaders, engine->sprite_VAO);

	glfwPollEvents();
	glfwSwapBuffers(engine->menu_window);
}

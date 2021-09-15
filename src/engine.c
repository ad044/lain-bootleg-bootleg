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

int engine_init(Engine *engine, GLFWwindow *window)
{
	// initialize shader cache
	if (!(shader_cache_init(&engine->shaders))) {
		printf("Failed to initialize shader cache.\n");
		return 0;
	}

	// initialize sprite vao
	engine->sprite_VAO = init_sprite_vao();

	// set current context window inside engine
	engine->window = window;

	// load initial scene
	Scene *scene = get_initial_scene();
	engine_load_scene(scene, engine);

	return 1;
}

void engine_render(Engine *engine)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	draw_scene(&engine->current_scene, engine->shaders, engine->sprite_VAO);

	glfwPollEvents();
	glfwSwapBuffers(engine->window);
}

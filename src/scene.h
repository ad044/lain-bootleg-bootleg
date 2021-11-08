#pragma once

#include "cvector.h"
#include "shader.h"
#include "sprite.h"
#include "text.h"
#include "texture.h"

#include "input.h"

typedef struct {
	Sprite *sprite;
	int click_event;
	// in some cases sprite is contained within a parent object. if we need
	// access to that we can store it here
	void *object;
} SpriteBehavior;

typedef struct {
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	unsigned int quad_count;
	cvector_vector_type(Sprite *) sprites;
	cvector_vector_type(Sprite) click_barriers;
	cvector_vector_type(Text *) text_objects;
	cvector_vector_type(SpriteBehavior) sprite_behaviors;
	_Bool draw_barriers;
} Scene;

void init_scene(Scene *scene, Sprite **sprites, uint8_t sprite_count,
		SpriteBehavior *sprite_behaviors, uint8_t sprite_behavior_count,
		Text **text_objs, uint8_t text_obj_count,
		Sprite *click_barriers, uint8_t click_barrier_count);
void update_scene(Scene *scene);
void draw_scene(Scene *scene, GLFWwindow *window, ShaderProgram shader);

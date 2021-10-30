#pragma once

#include "cvector.h"
#include "input.h"
#include "resource_cache.h"
#include "shader.h"
#include "sprite.h"
#include "text.h"
#include "texture.h"

typedef struct {
	GLint texture_index;
	Texture *texture;
} SceneTextureSlot;

typedef struct {
	Sprite *sprite;
	OnClickFunc on_click;
} SpriteBehavior;

typedef struct {
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	ShaderProgram shader;
	unsigned int quad_count;
	cvector_vector_type(SceneTextureSlot) texture_slots;
	cvector_vector_type(Sprite *) sprites;
	cvector_vector_type(Text *) text_objects;
	cvector_vector_type(SpriteBehavior) sprite_behaviors;
} Scene;

void init_scene(Scene *scene, Sprite **sprites, uint8_t sprite_count,
		SpriteBehavior *sprite_behaviors, uint8_t sprite_behavior_count,
		Text **text_objs, uint8_t text_obj_count, ShaderProgram shader);
void update_scene(Scene *scene);
void draw_scene(Scene *scene, GLFWwindow *window);
void update_texture_slot(Scene *scene, Sprite *sprite, Texture *texture);

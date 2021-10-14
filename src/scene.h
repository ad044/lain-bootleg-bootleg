#pragma once

#include "cvector.h"
#include "input.h"
#include "resource_cache.h"
#include "shader.h"
#include "sprite.h"
#include "texture.h"
#include "text.h"

typedef struct {
	GLint texture_index;
	Texture *texture;
} SceneTextureSlot;

typedef struct {
	Text **loc;
	TextDefinition text_def;
} SceneText;

typedef struct {
	Sprite **loc;
	Sprite sprite;
} SceneSprite;

typedef struct {
	Sprite **sprite;
	OnClickFunc on_click;
} SpriteBehavior;

typedef struct {
	SceneSprite *sprites;
	unsigned int sprite_count;
	SpriteBehavior *behaviors;
	unsigned int behavior_count;
	SceneTextureSlot **texture_slots;
	unsigned int texture_slot_count;
	SceneText *text_objects;
	unsigned int text_obj_count;
} SceneDefinition;

typedef struct {
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	ShaderProgram shader;
	unsigned int quad_count;
	cvector_vector_type(SceneTextureSlot *) texture_slots;
	cvector_vector_type(Sprite *) sprites;
	cvector_vector_type(Text *) text_objects;
	cvector_vector_type(SpriteBehavior) sprite_behaviors;
} Scene;

int init_scene(Scene *scene, SceneDefinition *scene_definition,
	       ResourceCache *resource_cache);
void update_scene(Scene *scene);
void draw_scene(Scene *scene, GLFWwindow *window);
SceneTextureSlot *make_texture_slot(unsigned int index, Texture *texture);
unsigned int get_scene_texture_count(Scene *scene);
SceneTextureSlot *get_texture_slot(Scene *scene, unsigned int index);
void update_texture_slot(Scene *scene, Sprite *sprite, Texture *texture);

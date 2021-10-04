#pragma once

#include "cvector.h"
#include "input.h"
#include "resource_cache.h"
#include "shader.h"
#include "sprite.h"
#include "texture.h"

#include <GLFW/glfw3.h>

typedef struct {
	GLint slot_id;
	Texture *texture;
} SceneTextureSlot;

typedef struct {
	Sprite **sprite_loc;
	Sprite sprite;
} SceneSprite;

typedef struct {
	Sprite **sprite;
	OnClickFunc on_click;
} SpriteBehavior;

typedef struct {
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	ShaderProgram shader;
	SceneTextureSlot *textures;
	unsigned int texture_count;
	GLint *samplers;
	cvector_vector_type(Sprite *) sprites;
	cvector_vector_type(SpriteBehavior) sprite_behaviors;
} Scene;

int init_scene(Scene **scene, SceneSprite *sprites, unsigned int sprite_count,
	       SceneTextureSlot *textures, unsigned int texture_count,
	       SpriteBehavior *behaviors, unsigned int behavior_count,
	       ResourceCache *resource_cache);
SceneTextureSlot make_texture_slot(int slot_id, Texture *texture);
void update_scene(Scene *scene);
void draw_scene(Scene *scene, GLFWwindow *window);
unsigned int get_scene_sprite_count(Scene *scene);

#pragma once

#include "resource_cache.h"
#include "shader.h"
#include "sprite.h"
#include "texture.h"

#include <cglm/types.h>

typedef struct {
	GLint slot_id;
	Texture *texture;
} SceneTextureSlot;

typedef struct {
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	ShaderProgram shader;
	SceneTextureSlot *textures;
	unsigned int texture_count;
	GLint *samplers;
	Sprite *sprites;
	unsigned int sprite_count;
} Scene;

int load_scene(Scene **scene, Sprite *sprites, unsigned int sprite_count,
	       SceneTextureSlot *textures, unsigned int texture_count,
	       ResourceCache *resource_cache);
SceneTextureSlot make_texture_slot(int slot_id, Texture *texture);
void update_scene(Scene *scene);
void draw_scene(Scene *scene);

#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CVECTOR_LOGARITHMIC_GROWTH

#include "resource_cache.h"
#include "scene.h"
#include "shader.h"
#include "sprite.h"

static void register_sprite_behavior(Scene *scene, SpriteBehavior behavior);
static int register_sprite(Scene *scene, SceneSprite *scene_sprite);
static void register_texture_slot(Scene *scene, SceneTextureSlot *texture_slot);
static int texture_slot_cmp(const void *a, const void *b);
static void sort_texture_slots(Scene *scene);

static void init_scene_buffers(Scene *scene, unsigned int sprite_count)
{

	glGenVertexArrays(1, &scene->VAO);
	glBindVertexArray(scene->VAO);

	glGenBuffers(1, &scene->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, scene->VBO);

	glGenBuffers(1, &scene->IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene->IBO);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
			      (void *)0);
	// texture coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
			      (void *)(2 * sizeof(GLfloat)));

	// texture id
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
			      (void *)(4 * sizeof(GLfloat)));
}

int init_scene(Scene *scene, SceneDefinition *scene_definition,
	       ResourceCache *resource_cache)
{

	SceneSprite *sprites = scene_definition->sprites;
	unsigned int sprite_count = scene_definition->sprite_count;
	unsigned int visible_sprite_count =
	    scene_definition->visible_sprite_count;

	SpriteBehavior *behaviors = scene_definition->behaviors;
	unsigned int behavior_count = scene_definition->behavior_count;

	SceneTextureSlot **texture_slots = scene_definition->texture_slots;
	unsigned int texture_slot_count = scene_definition->texture_slot_count;

	// initialize vao, vbo, ibo
	init_scene_buffers(scene, visible_sprite_count);

	// load sprites
	scene->sprites = NULL;
	for (int i = 0; i < sprite_count; i++) {
		if (!register_sprite(scene, &sprites[i])) {
			return 0;
		}
	}
	// sort sprites by z index
	depth_sort(scene->sprites, visible_sprite_count);

	// register behaviors
	scene->sprite_behaviors = NULL;
	for (int i = 0; i < behavior_count; i++) {
		register_sprite_behavior(scene, behaviors[i]);
	}

	// set up textures and samplers
	scene->texture_slots = NULL;
	for (int i = 0; i < texture_slot_count; i++) {
		register_texture_slot(scene, texture_slots[i]);
	}
	sort_texture_slots(scene);

	scene->shader = resource_cache->shaders[SCENE_SHADER];

	// update it once to fill vbo/ibo with initial sprite vertices
	update_scene(scene);

	return 1;
}

void update_scene(Scene *scene)
{
	glBindVertexArray(scene->VAO);
	unsigned int sprite_count = get_scene_visible_sprite_count(scene);

	GLfloat vertices[get_sprite_vertex_buffer_size(sprite_count)];
	GLfloat *buffer_ptr = vertices;

	for (int i = 0; i < sprite_count; i++) {
		if (!scene->sprites[i]->visible) {
			continue;
		}
		buffer_ptr = get_sprite_vertices(buffer_ptr, scene->sprites[i]);
	};

	update_sprite_buffers(scene->VBO, scene->IBO, vertices,
			      sizeof(vertices), sprite_count);
}

void draw_scene(Scene *scene, GLFWwindow *window)
{
	// window data
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);

	unsigned int texture_count = get_scene_texture_count(scene);

	// bind appropriate textures
	for (int i = 0; i < texture_count; i++) {
		glActiveTexture(GL_TEXTURE0 +
				scene->texture_slots[i]->texture_index);
		glBindTexture(GL_TEXTURE_2D,
			      scene->texture_slots[i]->texture->id);
	}

	// bind shader and set texture samplers
	glUseProgram(scene->shader);
	GLint samplers[texture_count];
	for (int i = 0; i < texture_count; i++) {
		samplers[i] = scene->texture_slots[i]->texture_index;
	}
	shader_program_set_texture_samplers(scene->shader, samplers,
					    texture_count);

	// set up matrices
	mat4 proj, model, view;

	glm_ortho(-w / 2.0f, w / 2.0f, -h / 2.0f, h / 2.0f, -1.0f, 1.0f, proj);

	glm_mat4_identity(model);
	glm_mat4_identity(view);

	shader_program_set_mat4(scene->shader, "u_Projection", proj);
	shader_program_set_mat4(scene->shader, "u_Model", model);
	shader_program_set_mat4(scene->shader, "u_View", view);

	// bind vao
	glBindVertexArray(scene->VAO);

	// draw
	unsigned int sprite_count = get_scene_visible_sprite_count(scene);
	glDrawElements(GL_TRIANGLES, get_sprite_index_count(sprite_count),
		       GL_UNSIGNED_INT, 0);
}

unsigned int get_scene_visible_sprite_count(Scene *scene)
{
	unsigned int count = 0;
	for (int i = 0; i < cvector_size(scene->sprites); ++i) {
		if (scene->sprites[i]->visible) {
			count++;
		}
	}
	return count;
}

static int texture_slot_cmp(const void *a, const void *b)
{

	const SceneTextureSlot *slot_a = *(SceneTextureSlot **)a;
	const SceneTextureSlot *slot_b = *(SceneTextureSlot **)b;
	return slot_a->texture_index - slot_b->texture_index;
}

unsigned int get_scene_texture_count(Scene *scene)
{
	return cvector_size(scene->texture_slots);
}

SceneTextureSlot *get_texture_slot(Scene *scene, Sprite *sprite)
{

	return scene->texture_slots[sprite->texture_index];
}

static void sort_texture_slots(Scene *scene)
{
	qsort(scene->texture_slots, get_scene_texture_count(scene),
	      sizeof(SceneTextureSlot *), texture_slot_cmp);
}

static int register_sprite(Scene *scene, SceneSprite *scene_sprite)
{
	*scene_sprite->loc = malloc(sizeof(Sprite));
	if (scene_sprite->loc == NULL) {
		printf("Failed to allocate memory for sprite\n");
		return 0;
	}

	memcpy(*scene_sprite->loc, &scene_sprite->sprite, sizeof(Sprite));

	cvector_push_back(scene->sprites, *scene_sprite->loc);

	return 1;
}

static void register_sprite_behavior(Scene *scene, SpriteBehavior behavior)
{
	cvector_push_back(scene->sprite_behaviors, behavior);
}

static void register_texture_slot(Scene *scene, SceneTextureSlot *texture_slot)
{
	cvector_push_back(scene->texture_slots, texture_slot);
}

SceneTextureSlot *make_texture_slot(unsigned int index, Texture *texture)
{

	SceneTextureSlot *texture_slot;
	texture_slot = (SceneTextureSlot *)malloc(sizeof(SceneTextureSlot));

	if (texture_slot == NULL) {
		printf("Failed to allocate memory for texture slot.\n");
		exit(1);
	}

	texture_slot->texture_index = index;
	texture_slot->texture = texture;

	return texture_slot;
}

void update_texture_slot(Scene *scene, Sprite *sprite, Texture *texture)
{
	SceneTextureSlot *slot = get_texture_slot(scene, sprite);

	slot->texture = texture;
}
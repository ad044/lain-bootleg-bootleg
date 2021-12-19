#include "animations.h"
#include "cJSON.h"
#include "resources.h"
#include "texture.h"
#include "vector2d.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static const char *ANIMATION_STRING[] = {
    FOREACH_SPRITE_ANIMATION(GENERATE_ANIMATION_STRING)};

static const char *THEATER_ANIMATION_STRING[] = {
    FOREACH_THEATER_ANIMATION(GENERATE_ANIMATION_STRING)};

static cJSON *parse_animations(char *filename)
{
	FILE *f = NULL;
	long len = 0;
	char *data = NULL;

	f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	len = ftell(f);
	fseek(f, 0, SEEK_SET);

	data = (char *)malloc(len + 1);

	fread(data, 1, len, f);
	data[len] = '\0';
	fclose(f);

	return cJSON_Parse(data);
}

static int animation_load(Animation *animation, const cJSON *frames)
{
	animation->first = NULL;
	animation->last = NULL;
	animation->looped = false;

	cJSON *curr;
	cJSON_ArrayForEach(curr, frames)
	{
		const cJSON *timing = NULL;
		const cJSON *rsrc_id = NULL;
		const cJSON *pos_x = NULL;
		const cJSON *pos_y = NULL;
		const cJSON *index = NULL;

		timing = cJSON_GetObjectItem(curr, "timing");
		rsrc_id = cJSON_GetObjectItem(curr, "rsrc_id");
		pos_x = cJSON_GetObjectItem(curr, "x_offset");
		pos_y = cJSON_GetObjectItem(curr, "y_offset");
		index = cJSON_GetObjectItem(curr, "index");

		AnimationFrame *frame = malloc(sizeof(AnimationFrame));
		if (frame == NULL) {
			printf("Failed to alloc memory for animation frame.\n");
			return 0;
		}

		if (rsrc_id->valueint == -1) {
			*frame = (AnimationFrame){.visible = false,
						  .index = index->valueint,
						  .timing = timing->valueint};
		} else {
			*frame = (AnimationFrame){
			    .index = index->valueint,
			    .timing = timing->valueint,
			    .texture_id = rsrc_id->valueint,
			    .pos_offset =
				(Vector2D){pos_x->valueint, pos_y->valueint},
			    .visible = true,
			};
		}

		frame->next = NULL;

		if (animation->first == NULL) {
			animation->first = frame;
			animation->last = frame;
		} else {
			animation->last->next = frame;
			animation->last = frame;
		}
	}

	return 1;
}

Animation *animation_get(Resources *resources, AnimationID animation_id)
{
	Animation *animation = &resources->animations[animation_id];

	if (animation->first == NULL) {
		const cJSON *sprite_animations = cJSON_GetObjectItem(
		    resources->animation_data, "sprite_animations");

		const cJSON *frames = cJSON_GetObjectItem(
		    sprite_animations, ANIMATION_STRING[animation_id]);

		if (frames == NULL) {
			printf("Failed to find animation %s in JSON file.\n ",
			       ANIMATION_STRING[animation_id]);
			return 0;
		}

		if (!animation_load(animation, frames)) {
			printf("Failed to load animation %s.\n ",
			       ANIMATION_STRING[animation_id]);
			return 0;
		};
	}

	return animation;
}

TheaterAnimation *theater_animation_get(Resources *resources,
					TheaterAnimationID animation_id)
{
	TheaterAnimation *animation =
	    &resources->theater_animations[animation_id];

	if (!animation->initialized) {
		const cJSON *theater_animations = cJSON_GetObjectItem(
		    resources->animation_data, "theater_scenes");

		const cJSON *layers = cJSON_GetObjectItem(
		    theater_animations, THEATER_ANIMATION_STRING[animation_id]);

		uint8_t layer_count = 0;
		cJSON *layer = layers->child;
		while (layer) {
			if (layer == NULL) {
				break;
			}

			if (!animation_load(&animation->layers[layer_count],
					    layer)) {
				printf("Failed to load animation %s.\n ",
				       THEATER_ANIMATION_STRING[animation_id]);
				return 0;
			};

			layer = layer->next;
			layer_count++;
		}

		if (animation->layers[0].first == NULL) {
			printf("Failed to load animation %s.\n",
			       ANIMATION_STRING[animation_id]);
			return 0;
		}

		animation->initialized = true;
		animation->layer_count = layer_count;
	}

	return animation;
}

void animations_init(Resources *resources)
{
	resources->animation_data = parse_animations("./res/animations.json");

	for (int i = 0; i < MAX_ANIMATION_COUNT; i++) {
		resources->animations[i] = (Animation){0};
	};

	for (int i = 0; i < MAX_THEATER_ANIMATION_COUNT; i++) {
		resources->theater_animations[i] = (TheaterAnimation){0};
	};
}

void animation_free(Animation *animation)
{
	AnimationFrame *curr = animation->first;
	while (curr != NULL) {
		AnimationFrame *next = curr->next;
		free(curr);
		curr = next;
	}
}

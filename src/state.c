#include "state.h"
#include "cJSON.h"
#include "dressup.h"
#include "texture.h"

#include <stdio.h>

#ifdef __linux__
#include <unistd.h>
#else
#define access _access
#endif

int init_game_state(Resources *resources, GameState *game_state)
{

	if (!load_save_file(resources, game_state)) {
		printf("Found a save file, but failed to load it. Starting "
		       "from a fresh save state.\n");

		game_state->score = 0;

		game_state->current_theater_preview = CLASSROOM_PREVIEW;

		game_state->lain = (Lain){.tool_state = NO_TOOLS};
		lain_set_outfit(resources, OUTFIT_DEFAULT, &game_state->lain);

		game_state->queued_sounds.size = 0;
	}

	return 1;
}

int write_save_file(Engine *engine)
{
	char *string = NULL;
	cJSON *score = NULL;
	cJSON *theater_preview_texture_id = NULL;
	cJSON *lain_tool_state = NULL;
	cJSON *lain_outfit = NULL;

	cJSON *save_state = cJSON_CreateObject();
	if (save_state == NULL) {
		goto fail;
	}

	score = cJSON_CreateNumber(engine->game_state.score);
	if (score == NULL) {
		goto fail;
	}

	theater_preview_texture_id =
	    cJSON_CreateNumber(engine->menu.theater_preview.texture->id);
	if (theater_preview_texture_id == NULL) {
		goto fail;
	}

	lain_outfit = cJSON_CreateNumber(engine->game_state.lain.outfit);
	if (lain_outfit == NULL) {
		goto fail;
	}

	lain_tool_state =
	    cJSON_CreateNumber(engine->game_state.lain.tool_state);
	if (lain_tool_state == NULL) {
		goto fail;
	}

	cJSON_AddItemToObject(save_state, "score", score);
	cJSON_AddItemToObject(save_state, "theater_preview",
			      theater_preview_texture_id);
	cJSON_AddItemToObject(save_state, "tool_state", lain_tool_state);
	cJSON_AddItemToObject(save_state, "outfit", lain_outfit);

	string = cJSON_Print(save_state);
	if (string == NULL) {
		printf("Failed to print save state.\n");
		goto fail;
	}

	FILE *f = fopen("./lain_save.json", "w");
	if (f == NULL) {
		printf("Failed to open save file.\n");
		free(string);
		goto fail;
	}

	fprintf(f, "%s\n", string);

	free(string);
	cJSON_Delete(save_state);

	return 1;
fail:
	cJSON_Delete(save_state);
	return 0;
}

int load_save_file(Resources *resources, GameState *game_state)
{
	if (access("./lain_save.json", 0) == 0) {
		FILE *f = NULL;
		long len = 0;
		char *data = NULL;

		f = fopen("./lain_save.json", "rb");
		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);

		data = (char *)malloc(len + 1);

		fread(data, 1, len, f);
		data[len] = '\0';
		fclose(f);

		cJSON *save_state = cJSON_Parse(data);

		cJSON *score = NULL;
		cJSON *theater_preview_texture_id = NULL;
		cJSON *lain_tool_state = NULL;
		cJSON *lain_outfit = NULL;

		score = cJSON_GetObjectItem(save_state, "score");
		if (score == NULL) {
			goto fail;
		}

		theater_preview_texture_id =
		    cJSON_GetObjectItem(save_state, "theater_preview");
		if (theater_preview_texture_id == NULL) {
			goto fail;
		}

		lain_tool_state = cJSON_GetObjectItem(save_state, "tool_state");
		if (lain_tool_state == NULL) {
			goto fail;
		}

		lain_outfit = cJSON_GetObjectItem(save_state, "outfit");
		if (lain_outfit == NULL) {
			goto fail;
		}

		game_state->score = score->valueint;
		game_state->lain.tool_state = lain_tool_state->valueint;

		game_state->lain.outfit = lain_outfit->valueint;
		lain_set_outfit(resources, game_state->lain.outfit,
				&game_state->lain);

		game_state->current_theater_preview =
		    theater_preview_texture_id->valueint;

		cJSON_Delete(save_state);
		free(data);

		return 1;
	fail:
		cJSON_Delete(save_state);
		free(data);
		return 0;
	}

	return 1;
}

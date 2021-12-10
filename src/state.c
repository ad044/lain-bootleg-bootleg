#include "state.h"
#include "cJSON.h"
#include "dressup.h"
#include "engine.h"
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
		printf(
		    "Failed to load save file. Starting from a fresh state.\n");

		game_state->score = 0;

		game_state->current_theater_preview = CLASSROOM_PREVIEW;

		game_state->lain = (Lain){.tool_state = NO_TOOLS};
		lain_set_outfit(resources, OUTFIT_DEFAULT, &game_state->lain);

		game_state->school_outfit_unlocked = false;
		game_state->alien_outfit_unlocked = false;
		game_state->bear_outfit_unlocked = false;
		game_state->sweater_outfit_unlocked = false;
		game_state->cyberia_outfit_unlocked = false;
		game_state->screwdriver_unlocked = false;
		game_state->navi_unlocked = false;
	}

	game_state->queued_sounds.size = 0;

	return 1;
}

int write_save_file(Engine *engine)
{
	char *string = NULL;
	cJSON *score = NULL;
	cJSON *theater_preview_texture_id = NULL;
	cJSON *lain_tool_state = NULL;
	cJSON *lain_outfit = NULL;
	cJSON *school_outfit_unlocked = NULL;
	cJSON *cyberia_outfit_unlocked = NULL;
	cJSON *sweater_outfit_unlocked = NULL;
	cJSON *bear_outfit_unlocked = NULL;
	cJSON *alien_outfit_unlocked = NULL;
	cJSON *screwdriver_unlocked = NULL;
	cJSON *navi_unlocked = NULL;

	cJSON *save_state = cJSON_CreateObject();

	score = cJSON_CreateNumber(engine->game_state.score);

	theater_preview_texture_id =
	    cJSON_CreateNumber(engine->menu.theater_preview.texture->id);

	lain_outfit = cJSON_CreateNumber(engine->game_state.lain.outfit);

	lain_tool_state =
	    cJSON_CreateNumber(engine->game_state.lain.tool_state);

	school_outfit_unlocked =
	    cJSON_CreateNumber(engine->game_state.school_outfit_unlocked);

	cyberia_outfit_unlocked =
	    cJSON_CreateNumber(engine->game_state.cyberia_outfit_unlocked);

	sweater_outfit_unlocked =
	    cJSON_CreateNumber(engine->game_state.sweater_outfit_unlocked);

	bear_outfit_unlocked =
	    cJSON_CreateNumber(engine->game_state.bear_outfit_unlocked);

	alien_outfit_unlocked =
	    cJSON_CreateNumber(engine->game_state.alien_outfit_unlocked);

	screwdriver_unlocked =
	    cJSON_CreateNumber(engine->game_state.screwdriver_unlocked);

	navi_unlocked = cJSON_CreateNumber(engine->game_state.navi_unlocked);

	cJSON *cjson_objs[] = {score,
			       theater_preview_texture_id,
			       lain_tool_state,
			       lain_outfit,
			       school_outfit_unlocked,
			       cyberia_outfit_unlocked,
			       sweater_outfit_unlocked,
			       bear_outfit_unlocked,
			       alien_outfit_unlocked,
			       screwdriver_unlocked,
			       navi_unlocked};
	for (int i = 0; i < sizeof(cjson_objs) / sizeof(cjson_objs[0]); i++) {
		if (cjson_objs[i] == NULL) {
			goto fail;
		}
	}

	cJSON_AddItemToObject(save_state, "score", score);
	cJSON_AddItemToObject(save_state, "theater_preview",
			      theater_preview_texture_id);
	cJSON_AddItemToObject(save_state, "tool_state", lain_tool_state);
	cJSON_AddItemToObject(save_state, "outfit", lain_outfit);
	cJSON_AddItemToObject(save_state, "school_outfit_unlocked",
			      school_outfit_unlocked);
	cJSON_AddItemToObject(save_state, "cyberia_outfit_unlocked",
			      cyberia_outfit_unlocked);
	cJSON_AddItemToObject(save_state, "sweater_outfit_unlocked",
			      sweater_outfit_unlocked);
	cJSON_AddItemToObject(save_state, "bear_outfit_unlocked",
			      bear_outfit_unlocked);
	cJSON_AddItemToObject(save_state, "alien_outfit_unlocked",
			      alien_outfit_unlocked);
	cJSON_AddItemToObject(save_state, "screwdriver_unlocked",
			      screwdriver_unlocked);
	cJSON_AddItemToObject(save_state, "navi_unlocked", navi_unlocked);

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
	if (access("./lain_save.json", 0) != 0) {
		return 0;
	}
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
	cJSON *school_outfit_unlocked = NULL;
	cJSON *cyberia_outfit_unlocked = NULL;
	cJSON *sweater_outfit_unlocked = NULL;
	cJSON *bear_outfit_unlocked = NULL;
	cJSON *alien_outfit_unlocked = NULL;
	cJSON *screwdriver_unlocked = NULL;
	cJSON *navi_unlocked = NULL;

	score = cJSON_GetObjectItem(save_state, "score");

	theater_preview_texture_id =
	    cJSON_GetObjectItem(save_state, "theater_preview");

	lain_tool_state = cJSON_GetObjectItem(save_state, "tool_state");

	lain_outfit = cJSON_GetObjectItem(save_state, "outfit");

	school_outfit_unlocked =
	    cJSON_GetObjectItem(save_state, "school_outfit_unlocked");

	cyberia_outfit_unlocked =
	    cJSON_GetObjectItem(save_state, "cyberia_outfit_unlocked");

	sweater_outfit_unlocked =
	    cJSON_GetObjectItem(save_state, "sweater_outfit_unlocked");

	bear_outfit_unlocked =
	    cJSON_GetObjectItem(save_state, "bear_outfit_unlocked");

	alien_outfit_unlocked =
	    cJSON_GetObjectItem(save_state, "alien_outfit_unlocked");

	screwdriver_unlocked =
	    cJSON_GetObjectItem(save_state, "screwdriver_unlocked");

	navi_unlocked = cJSON_GetObjectItem(save_state, "navi_unlocked");

	cJSON *cjson_objs[] = {score,
			       theater_preview_texture_id,
			       lain_tool_state,
			       lain_outfit,
			       school_outfit_unlocked,
			       cyberia_outfit_unlocked,
			       sweater_outfit_unlocked,
			       bear_outfit_unlocked,
			       alien_outfit_unlocked,
			       screwdriver_unlocked,
			       navi_unlocked};
	for (int i = 0; i < sizeof(cjson_objs) / sizeof(cjson_objs[0]); i++) {
		if (cjson_objs[i] == NULL) {
			goto fail;
		}
	}

	game_state->score = score->valueint;
	game_state->lain.tool_state = lain_tool_state->valueint;

	game_state->lain.outfit = lain_outfit->valueint;
	lain_set_outfit(resources, game_state->lain.outfit, &game_state->lain);

	game_state->current_theater_preview =
	    theater_preview_texture_id->valueint;

	game_state->school_outfit_unlocked = school_outfit_unlocked->valueint;
	game_state->alien_outfit_unlocked = alien_outfit_unlocked->valueint;
	game_state->bear_outfit_unlocked = bear_outfit_unlocked->valueint;
	game_state->sweater_outfit_unlocked = sweater_outfit_unlocked->valueint;
	game_state->cyberia_outfit_unlocked = cyberia_outfit_unlocked->valueint;
	game_state->screwdriver_unlocked = screwdriver_unlocked->valueint;
	game_state->navi_unlocked = navi_unlocked->valueint;

	cJSON_Delete(save_state);
	free(data);

	return 1;
fail:
	cJSON_Delete(save_state);
	free(data);
	return 0;
}

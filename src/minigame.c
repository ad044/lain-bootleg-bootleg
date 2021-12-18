#include "minigame.h"
#include "animations.h"
#include "resources.h"
#include "scene.h"

void destroy_minigame(Texture *textures, Menu *menu, Minigame *minigame,
		      GLFWwindow *minigame_window)
{
	menu->bear_icon.texture = &textures[BEAR_ICON];
	menu->dressup_button.texture = &textures[DRESSUP_BUTTON];

	glfwDestroyWindow(minigame_window);

	switch (minigame->type) {
	case KUMASHOOT:
		free_scene(&minigame->current.kumashoot.scene);
		break;
	case DRESSUP:
		free_scene(&minigame->current.dressup.scene);
		break;
	case THEATER: {
		Theater *theater = &minigame->current.theater;
		free_scene(&theater->scene);
		if (theater->type == THEATER_MOVIE) {
			movie_video_free(&theater->video);
		}

		break;
	}
	default:
		break;
	}

	minigame->type = NO_MINIGAME;
}

void update_minigame(Resources *resources, GameState *game_state, Menu *menu,
		     GLFWwindow *minigame_window, Minigame *minigame)
{
	switch (minigame->type) {
	case KUMASHOOT:
		update_kumashoot(resources, menu, game_state, minigame_window,
				 minigame);
		break;
	case DRESSUP:
		update_dressup(resources, menu, game_state, minigame_window,
			       minigame);
		break;
	case THEATER:
		update_theater(resources, menu, game_state, minigame_window,
			       minigame);
		break;
	default:
		break;
	}
}

void draw_minigame(Resources *resources, GLFWwindow *minigame_window,
		   Minigame *minigame)
{
	switch (minigame->type) {
	case KUMASHOOT:
		draw_scene(&minigame->current.kumashoot.scene, minigame_window,
			   resources->shaders[SPRITE_SHADER]);
		break;
	case DRESSUP:
		draw_scene(&minigame->current.dressup.scene, minigame_window,
			   resources->shaders[SPRITE_SHADER]);
		break;
	case THEATER:
		draw_scene(&minigame->current.theater.scene, minigame_window,
			   resources->shaders[SPRITE_SHADER]);
		break;
	default:
		break;
	}
}

void start_queued_minigame(Resources *resources, GameState *game_state,
			   Menu *menu, GLFWwindow *main_window,
			   GLFWwindow **minigame_window, Minigame *minigame)
{
	// NOTE:
	// start functions here return 0 if they fail
	// i am not quite sure what to do in that case :D
	switch (minigame->queued_minigame) {
	case KUMASHOOT:
		start_kumashoot(menu, resources, game_state, minigame,
				minigame_window, main_window);
		break;
	case DRESSUP:
		start_dressup(menu, resources, game_state, minigame,
			      minigame_window, main_window);
		break;
	case THEATER:
		start_theater(menu, resources, game_state, minigame,
			      minigame_window, main_window);
		break;
	default:
		break;
	}
	minigame->queued_minigame = NO_MINIGAME;
}

_Bool can_refresh(double time, Minigame *minigame)
{
	return time - minigame->last_updated > 1.0 / 25.0;
}

void get_minigame_scene(Minigame *minigame, Scene *target)
{
	switch (minigame->type) {
	case KUMASHOOT:
		*target = minigame->current.kumashoot.scene;
		break;
	case DRESSUP:
		*target = minigame->current.dressup.scene;
		break;
	case THEATER:
		*target = minigame->current.theater.scene;
		break;
	default:
		break;
	}
}

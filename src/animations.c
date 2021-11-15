#include "animations.h"

#include <stdio.h>
#include <stdlib.h>

static AnimationFrame *add_frame_to_animation(Animation *animation,
					      AnimationFrame frame)
{
	AnimationFrame *new_frame =
	    (AnimationFrame *)malloc(sizeof(AnimationFrame));

	if (new_frame == NULL) {
		printf("Failed to allocate memory for animation frame.\n");
		return 0;
	}

	new_frame->frame_index = frame.frame_index;
	new_frame->timing = frame.timing;
	new_frame->next = NULL;

	if (animation->first == NULL) {
		animation->first = new_frame;
		animation->last = new_frame;
	} else {
		animation->last->next = new_frame;
		animation->last = new_frame;
	}

	return new_frame;
}

static void create_animation(Animation *animation, AnimationFrame *frames,
			     unsigned int frame_count, int loop_frame)
{
	animation->first = NULL;
	animation->last = NULL;
	animation->loop_frame = NULL;

	for (int i = 0; i < frame_count; i++) {
		AnimationFrame *frame =
		    add_frame_to_animation(animation, frames[i]);

		if (frame == NULL) {
			// TODO handle
		}

		if (i == loop_frame) {
			animation->loop_frame = frame;
		}
	}

	if (animation->loop_frame != NULL) {
		animation->last->next = animation->loop_frame;
	}
}

void init_animations(Animation *animations)
{
	AnimationFrame main_ui_expand[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 1, .timing = 2},
	    (AnimationFrame){.frame_index = 2, .timing = 2},
	    (AnimationFrame){.frame_index = 3, .timing = 10},
	    (AnimationFrame){.frame_index = 4, .timing = 3},
	    (AnimationFrame){.frame_index = 5, .timing = 1},
	    (AnimationFrame){.frame_index = 6, .timing = 1},
	};
	create_animation(&animations[MAIN_UI_EXPAND_ANIMATION], main_ui_expand,
			 7, -1);

	AnimationFrame main_ui_shrink[] = {
	    (AnimationFrame){.frame_index = 6, .timing = 1},
	    (AnimationFrame){.frame_index = 5, .timing = 2},
	    (AnimationFrame){.frame_index = 4, .timing = 2},
	    (AnimationFrame){.frame_index = 3, .timing = 10},
	    (AnimationFrame){.frame_index = 2, .timing = 3},
	    (AnimationFrame){.frame_index = 1, .timing = 1},
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	};
	create_animation(&animations[MAIN_UI_SHRINK_ANIMATION], main_ui_shrink,
			 7, -1);

	AnimationFrame ui_lain_blink[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 1, .timing = 2},
	    (AnimationFrame){.frame_index = 2, .timing = 2},
	    (AnimationFrame){.frame_index = 1, .timing = 5},
	    (AnimationFrame){.frame_index = 0, .timing = 5}};
	create_animation(&animations[UI_LAIN_BLINK_ANIMATION], ui_lain_blink, 5,
			 -1);

	AnimationFrame ui_lain_laugh_blink[] = {
	    (AnimationFrame){.frame_index = 4, .timing = 1},
	    (AnimationFrame){.frame_index = 5, .timing = 2},
	    (AnimationFrame){.frame_index = 6, .timing = 2},
	    (AnimationFrame){.frame_index = 5, .timing = 5},
	    (AnimationFrame){.frame_index = 4, .timing = 5}};
	create_animation(&animations[UI_LAIN_LAUGH_BLINK_ANIMATION],
			 ui_lain_laugh_blink, 5, -1);

	AnimationFrame ui_lain_laugh[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 3, .timing = 4},
	    (AnimationFrame){.frame_index = 4, .timing = 5}};
	create_animation(&animations[UI_LAIN_LAUGH_ANIMATION], ui_lain_laugh, 3,
			 -1);

	AnimationFrame ui_lain_stop_laugh[] = {
	    (AnimationFrame){.frame_index = 4, .timing = 1},
	    (AnimationFrame){.frame_index = 5, .timing = 2},
	    (AnimationFrame){.frame_index = 6, .timing = 2},
	    (AnimationFrame){.frame_index = 5, .timing = 5},
	    (AnimationFrame){.frame_index = 3, .timing = 4},
	    (AnimationFrame){.frame_index = 0, .timing = 3}};
	create_animation(&animations[UI_LAIN_STOP_LAUGH_ANIMATION],
			 ui_lain_stop_laugh, 6, -1);

	AnimationFrame bear_animation[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 1, .timing = 4},
	    (AnimationFrame){.frame_index = 2, .timing = 5},
	    (AnimationFrame){.frame_index = 3, .timing = 5},
	    (AnimationFrame){.frame_index = 4, .timing = 5},
	    (AnimationFrame){.frame_index = 5, .timing = 5},
	    (AnimationFrame){.frame_index = 6, .timing = 5},
	    (AnimationFrame){.frame_index = 7, .timing = 5}};
	create_animation(&animations[BEAR_ANIMATION], bear_animation, 8, 0);

	AnimationFrame kuma_shoot_smoke_animation[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 1, .timing = 9},
	    (AnimationFrame){.frame_index = 2, .timing = 10},
	    (AnimationFrame){.frame_index = 3, .timing = 10},
	    (AnimationFrame){.frame_index = 4, .timing = 10}};
	create_animation(&animations[KUMA_SHOOT_SMOKE_ANIMATION],
			 kuma_shoot_smoke_animation, 5, -1);

	AnimationFrame kuma_shoot_yasuo_animation[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 1, .timing = 4},
	    (AnimationFrame){.frame_index = 2, .timing = 5},
	};
	create_animation(&animations[KUMA_SHOOT_YASUO_ANIMATION],
			 kuma_shoot_yasuo_animation, 3, 0);

	AnimationFrame kuma_shoot_mika_animation[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 1, .timing = 9},
	    (AnimationFrame){.frame_index = 2, .timing = 20},
	};
	create_animation(&animations[KUMA_SHOOT_MIKA_ANIMATION],
			 kuma_shoot_mika_animation, 3, 0);

	AnimationFrame kuma_shoot_miho_animation[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 1, .timing = 4},
	    (AnimationFrame){.frame_index = 2, .timing = 5},
	    (AnimationFrame){.frame_index = 3, .timing = 5},
	    (AnimationFrame){.frame_index = 4, .timing = 5},
	    (AnimationFrame){.frame_index = 5, .timing = 10},
	    (AnimationFrame){.frame_index = 4, .timing = 10},
	    (AnimationFrame){.frame_index = 5, .timing = 10},
	    (AnimationFrame){.frame_index = 4, .timing = 10},
	    (AnimationFrame){.frame_index = 5, .timing = 10},
	    (AnimationFrame){.frame_index = 4, .timing = 10},
	    (AnimationFrame){.frame_index = 5, .timing = 10},
	    (AnimationFrame){.frame_index = 6, .timing = 10},
	};
	create_animation(&animations[KUMA_SHOOT_MIHO_ANIMATION],
			 kuma_shoot_miho_animation, 13, 0);

	// frames 3-4 are skipped completely in original
	AnimationFrame kuma_shoot_school_lain_animation[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 1, .timing = 4},
	    (AnimationFrame){.frame_index = 2, .timing = 5},
	    (AnimationFrame){.frame_index = 3, .timing = 5},
	    (AnimationFrame){.frame_index = 4, .timing = 5},
	    (AnimationFrame){.frame_index = 5, .timing = 1},
	    (AnimationFrame){.frame_index = 6, .timing = 4},
	    (AnimationFrame){.frame_index = 7, .timing = 5},
	    (AnimationFrame){.frame_index = 8, .timing = 5},
	    (AnimationFrame){.frame_index = 9, .timing = 5},
	    (AnimationFrame){.frame_index = 10, .timing = 5},
	    (AnimationFrame){.frame_index = 11, .timing = 5},
	    (AnimationFrame){.frame_index = 12, .timing = 5},
	};
	create_animation(&animations[KUMA_SHOOT_SCHOOL_LAIN_ANIMATION],
			 kuma_shoot_school_lain_animation, 13, 5);

	AnimationFrame kuma_shoot_default_lain_animation[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 1, .timing = 4},
	    (AnimationFrame){.frame_index = 2, .timing = 5},
	    (AnimationFrame){.frame_index = 3, .timing = 5},
	    (AnimationFrame){.frame_index = 4, .timing = 5},
	    (AnimationFrame){.frame_index = 5, .timing = 5},
	    (AnimationFrame){.frame_index = 6, .timing = 5},
	    (AnimationFrame){.frame_index = 7, .timing = 5},
	};
	create_animation(&animations[KUMA_SHOOT_DEFAULT_LAIN_ANIMATION],
			 kuma_shoot_default_lain_animation, 8, 0);

	// first frame is skipped completely in original
	AnimationFrame kuma_shoot_screwdriver_lain_animation[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 1, .timing = 4},
	    (AnimationFrame){.frame_index = 2, .timing = 4},
	    (AnimationFrame){.frame_index = 3, .timing = 15},
	    (AnimationFrame){.frame_index = 4, .timing = 5},
	    (AnimationFrame){.frame_index = 5, .timing = 5},
	    (AnimationFrame){.frame_index = 6, .timing = 5},
	    (AnimationFrame){.frame_index = 7, .timing = 5},
	    (AnimationFrame){.frame_index = 8, .timing = 5},
	};
	create_animation(&animations[KUMA_SHOOT_SCREWDRIVER_LAIN_ANIMATION],
			 kuma_shoot_screwdriver_lain_animation, 9, -1);

	AnimationFrame kuma_shoot_explosion_animation[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 1, .timing = 4},
	    (AnimationFrame){.frame_index = 2, .timing = 5},
	    (AnimationFrame){.frame_index = 3, .timing = 5},
	    (AnimationFrame){.frame_index = 4, .timing = 5},
	};
	create_animation(&animations[KUMA_SHOOT_EXPLOSION_ANIMATION],
			 kuma_shoot_explosion_animation, 5, -1);

	AnimationFrame lain_leave_animation[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 1, .timing = 4},
	    (AnimationFrame){.frame_index = 2, .timing = 5},
	    (AnimationFrame){.frame_index = 3, .timing = 5},
	    (AnimationFrame){.frame_index = 4, .timing = 5},
	    (AnimationFrame){.frame_index = 5, .timing = 5},
	    (AnimationFrame){.frame_index = 6, .timing = 5},
	    (AnimationFrame){.frame_index = 7, .timing = 5},
	    (AnimationFrame){.frame_index = 8, .timing = 5},
	    (AnimationFrame){.frame_index = 9, .timing = 5},
	    (AnimationFrame){.frame_index = 10, .timing = 5},
	    (AnimationFrame){.frame_index = 3, .timing = 5},
	    (AnimationFrame){.frame_index = 4, .timing = 5},
	    (AnimationFrame){.frame_index = 5, .timing = 5},
	};
	create_animation(&animations[LAIN_LEAVE_ANIMATION],
			 lain_leave_animation, 14, -1);

	AnimationFrame lain_move_left_animation[] = {
	    (AnimationFrame){.frame_index = 2, .timing = 1},
	    (AnimationFrame){.frame_index = 3, .timing = 4},
	    (AnimationFrame){.frame_index = 4, .timing = 5},
	    (AnimationFrame){.frame_index = 5, .timing = 5},
	    (AnimationFrame){.frame_index = 6, .timing = 5},
	    (AnimationFrame){.frame_index = 7, .timing = 5},
	    (AnimationFrame){.frame_index = 0, .timing = 5},
	    (AnimationFrame){.frame_index = 1, .timing = 5},
	    (AnimationFrame){.frame_index = 2, .timing = 5},
	    (AnimationFrame){.frame_index = 3, .timing = 5},
	    (AnimationFrame){.frame_index = 5, .timing = 5},
	    (AnimationFrame){.frame_index = 6, .timing = 5},
	    (AnimationFrame){.frame_index = 7, .timing = 5},
	    (AnimationFrame){.frame_index = 0, .timing = 5},
	    (AnimationFrame){.frame_index = 1, .timing = 5},
	    (AnimationFrame){.frame_index = 2, .timing = 5},
	};
	create_animation(&animations[LAIN_MOVE_LEFT_ANIMATION],
			 lain_move_left_animation, 16, -1);
}

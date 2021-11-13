#include "animations.h"

#include <stdio.h>
#include <stdlib.h>

static void add_frame_to_animation(Animation *animation, AnimationFrame frame)
{
	AnimationFrame *new_frame =
	    (AnimationFrame *)malloc(sizeof(AnimationFrame));

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
}

static void create_animation(Animation *animation, AnimationFrame *frames,
			     unsigned int frame_count)
{
	animation->first = NULL;
	animation->last = NULL;

	for (int i = 0; i < frame_count; i++) {
		add_frame_to_animation(animation, frames[i]);
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
			 7);

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
			 7);

	AnimationFrame ui_lain_blink[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 1, .timing = 2},
	    (AnimationFrame){.frame_index = 2, .timing = 2},
	    (AnimationFrame){.frame_index = 1, .timing = 5},
	    (AnimationFrame){.frame_index = 0, .timing = 5}};
	create_animation(&animations[UI_LAIN_BLINK_ANIMATION], ui_lain_blink,
			 5);

	AnimationFrame ui_lain_laugh_blink[] = {
	    (AnimationFrame){.frame_index = 4, .timing = 1},
	    (AnimationFrame){.frame_index = 5, .timing = 2},
	    (AnimationFrame){.frame_index = 6, .timing = 2},
	    (AnimationFrame){.frame_index = 5, .timing = 5},
	    (AnimationFrame){.frame_index = 4, .timing = 5}};
	create_animation(&animations[UI_LAIN_LAUGH_BLINK_ANIMATION],
			 ui_lain_laugh_blink, 5);

	AnimationFrame ui_lain_laugh[] = {
	    (AnimationFrame){.frame_index = 0, .timing = 1},
	    (AnimationFrame){.frame_index = 3, .timing = 4},
	    (AnimationFrame){.frame_index = 4, .timing = 5}};
	create_animation(&animations[UI_LAIN_LAUGH_ANIMATION], ui_lain_laugh,
			 3);

	AnimationFrame ui_lain_stop_laugh[] = {
	    (AnimationFrame){.frame_index = 4, .timing = 1},
	    (AnimationFrame){.frame_index = 5, .timing = 2},
	    (AnimationFrame){.frame_index = 6, .timing = 2},
	    (AnimationFrame){.frame_index = 5, .timing = 5},
	    (AnimationFrame){.frame_index = 3, .timing = 4},
	    (AnimationFrame){.frame_index = 0, .timing = 3}};
	create_animation(&animations[UI_LAIN_STOP_LAUGH_ANIMATION],
			 ui_lain_stop_laugh, 6);
}

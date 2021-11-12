#include "animations.h"

#include <stdio.h>
#include <stdlib.h>

static void add_frame_to_animation(Animation *animation, AnimationFrame frame)
{
	AnimationFrame *new_frame =
	    (AnimationFrame *)malloc(sizeof(AnimationFrame));

	new_frame->index = frame.index;
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
	    (AnimationFrame){.index = 0, .timing = 1},
	    (AnimationFrame){.index = 1, .timing = 2},
	    (AnimationFrame){.index = 2, .timing = 2},
	    (AnimationFrame){.index = 3, .timing = 10},
	    (AnimationFrame){.index = 4, .timing = 3},
	    (AnimationFrame){.index = 5, .timing = 1},
	    (AnimationFrame){.index = 6, .timing = 1},
	};
	create_animation(&animations[MAIN_UI_EXPAND_ANIMATION], main_ui_expand,
			 7);

	AnimationFrame main_ui_shrink[] = {
	    (AnimationFrame){.index = 6, .timing = 1},
	    (AnimationFrame){.index = 5, .timing = 2},
	    (AnimationFrame){.index = 4, .timing = 2},
	    (AnimationFrame){.index = 3, .timing = 10},
	    (AnimationFrame){.index = 2, .timing = 3},
	    (AnimationFrame){.index = 1, .timing = 1},
	    (AnimationFrame){.index = 0, .timing = 1},
	};
	create_animation(&animations[MAIN_UI_SHRINK_ANIMATION], main_ui_shrink,
			 7);

	AnimationFrame ui_lain_blink[] = {
	    (AnimationFrame){.index = 0, .timing = 1},
	    (AnimationFrame){.index = 1, .timing = 3},
	    (AnimationFrame){.index = 2, .timing = 5},
	    (AnimationFrame){.index = 1, .timing = 10}};
}

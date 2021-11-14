#pragma once

// the original game stores timings for each frame for each spritesheet
// in a separate binary file. these timings can differ widely, even in
// the same spritesheet.
// this is my way of porting that here, without that external file.

enum {
	NO_ANIMATION,

	MAIN_UI_EXPAND_ANIMATION,
	MAIN_UI_SHRINK_ANIMATION,
	UI_LAIN_BLINK_ANIMATION,
	UI_LAIN_LAUGH_BLINK_ANIMATION,
	UI_LAIN_LAUGH_ANIMATION,
	UI_LAIN_STOP_LAUGH_ANIMATION,

	BEAR_ANIMATION,
	KUMA_SHOOT_SMOKE_ANIMATION,
	KUMA_SHOOT_MIHO_ANIMATION,
	KUMA_SHOOT_MIKA_ANIMATION,
	KUMA_SHOOT_YASUO_ANIMATION,
	KUMA_SHOOT_SCHOOL_LAIN_ANIMATION,
	KUMA_SHOOT_DEFAULT_LAIN_ANIMATION,
	KUMA_SHOOT_SCREWDRIVER_LAIN_ANIMATION,
	KUMA_SHOOT_EXPLOSION_ANIMATION,
};

typedef struct AnimationFrame {
	unsigned int frame_index;
	double timing;
	struct AnimationFrame *next;
} AnimationFrame;

typedef struct {
	AnimationFrame *first;
	AnimationFrame *last;

	// specifies on which frame the animation should continue if it reaches
	// the end NULL if not looped.
	AnimationFrame *loop_frame;
} Animation;

void init_animations(Animation *animations);

#pragma once

// the original game stores timings for each frame for each spritesheet
// in a separate binary file. these timings can differ widely, even in
// the same spritesheet.
// this is my way of porting that here, without that external file.

typedef enum {
	NO_ANIMATION,
	MAIN_UI_EXPAND_ANIMATION,
	MAIN_UI_SHRINK_ANIMATION,
	UI_LAIN_BLINK_ANIMATION,
	UI_LAIN_LAUGH_BLINK_ANIMATION,
	UI_LAIN_LAUGH_ANIMATION,
	UI_LAIN_STOP_LAUGH_ANIMATION
} AnimationID;

typedef struct AnimationFrame {
	unsigned int frame_index;
	double timing;
	struct AnimationFrame *next;
} AnimationFrame;

typedef struct {
	AnimationFrame *first;
	AnimationFrame *last;
} Animation;

void init_animations(Animation *animations);

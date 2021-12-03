#pragma once

#include "texture.h"
#include "vector2d.h"

#include <stdint.h>

#define MAX_ANIMATION_COUNT 128
#define MAX_THEATER_ANIMATION_COUNT 16

#define FOREACH_SPRITE_ANIMATION(ANIMATION)                                    \
	ANIMATION(MAIN_UI_EXPAND_ANIMATION)                                    \
	ANIMATION(MAIN_UI_COLLAPSE_ANIMATION)                                  \
	ANIMATION(UI_DEFAULT_LAIN_BLINK_ANIMATION)                             \
	ANIMATION(UI_DEFAULT_LAIN_LAUGH_ANIMATION)                             \
	ANIMATION(UI_DEFAULT_LAIN_LAUGH_BLINK_ANIMATION)                       \
	ANIMATION(UI_BEAR_LAIN_BLINK_ANIMATION)                                \
	ANIMATION(UI_BEAR_LAIN_LAUGH_ANIMATION)                                \
	ANIMATION(UI_BEAR_LAIN_LAUGH_BLINK_ANIMATION)                          \
	ANIMATION(KUMA_SHOOT_BROWN_BEAR_WALK_ANIMATION)                        \
	ANIMATION(KUMA_SHOOT_WHITE_BEAR_WALK_ANIMATION)                        \
	ANIMATION(KUMA_SHOOT_SMOKE_ANIMATION)                                  \
	ANIMATION(KUMA_SHOOT_MIHO_ANIMATION)                                   \
	ANIMATION(KUMA_SHOOT_MIKA_ANIMATION)                                   \
	ANIMATION(KUMA_SHOOT_YASUO_ANIMATION)                                  \
	ANIMATION(KUMA_SHOOT_SCHOOL_LAIN_1_ANIMATION)                          \
	ANIMATION(KUMA_SHOOT_SCHOOL_LAIN_2_ANIMATION)                          \
	ANIMATION(KUMA_SHOOT_DEFAULT_LAIN_ANIMATION)                           \
	ANIMATION(KUMA_SHOOT_SCREWDRIVER_LAIN_ANIMATION)                       \
	ANIMATION(KUMA_SHOOT_SCREW_ANIMATION)                                  \
	ANIMATION(KUMA_SHOOT_EXPLOSION_ANIMATION)                              \
	ANIMATION(LAIN_DEFAULT_LEAVE_ANIMATION)                                \
	ANIMATION(LAIN_DEFAULT_WALK_LEFT_ANIMATION)                            \
	ANIMATION(LAIN_SCHOOL_LEAVE_ANIMATION)                                 \
	ANIMATION(LAIN_SCHOOL_WALK_LEFT_ANIMATION)                             \
	ANIMATION(LAIN_CYBERIA_LEAVE_ANIMATION)                                \
	ANIMATION(LAIN_CYBERIA_WALK_LEFT_ANIMATION)                            \
	ANIMATION(LAIN_BEAR_LEAVE_ANIMATION)                                   \
	ANIMATION(LAIN_BEAR_WALK_LEFT_ANIMATION)                               \
	ANIMATION(LAIN_SWEATER_LEAVE_ANIMATION)                                \
	ANIMATION(LAIN_SWEATER_WALK_LEFT_ANIMATION)                            \
	ANIMATION(LAIN_ALIEN_LEAVE_ANIMATION)                                  \
	ANIMATION(LAIN_ALIEN_WALK_LEFT_ANIMATION)                              \
	ANIMATION(SCHOOL_LAIN_THEATER_WALK_ANIMATION)                          \
	ANIMATION(BEAR_LAIN_THEATER_WALK_ANIMATION)                            \
	ANIMATION(CYBERIA_LAIN_THEATER_WALK_ANIMATION)                         \
	ANIMATION(ALIEN_LAIN_THEATER_WALK_ANIMATION)                           \
	ANIMATION(SWEATER_LAIN_THEATER_WALK_ANIMATION)                         \
	ANIMATION(DEFAULT_LAIN_THEATER_WALK_ANIMATION)

#define FOREACH_THEATER_ANIMATION(ANIMATION)                                   \
	ANIMATION(THEATER_CLASSROOM_ANIMATION)                                 \
	ANIMATION(THEATER_BRIDGE_ANIMATION)                                    \
	ANIMATION(THEATER_SCHOOL_ANIMATION)                                    \
	ANIMATION(THEATER_LAIN_ROOM_NIGHT_ANIMATION)                           \
	ANIMATION(THEATER_ARISU_ROOM_ANIMATION)                                \
	ANIMATION(THEATER_CYBERIA_ANIMATION)                                   \
	ANIMATION(THEATER_STREET_ANIMATION)

#define GENERATE_ANIMATION_ENUM(ENUM) ENUM,
#define GENERATE_ANIMATION_STRING(STRING) #STRING,

typedef enum { FOREACH_SPRITE_ANIMATION(GENERATE_ANIMATION_ENUM) } AnimationID;
typedef enum {
	FOREACH_THEATER_ANIMATION(GENERATE_ANIMATION_ENUM)
} TheaterAnimationID;

typedef struct AnimationFrame {
	uint8_t index;
	uint16_t timing;
	Texture *texture;
	Vector2D pos_offset;
	_Bool visible;
	struct AnimationFrame *next;
} AnimationFrame;

typedef struct {
	AnimationFrame *first;
	AnimationFrame *last;

	// specifies on which frame the animation should continue if it reaches
	// the end.
	// NULL if not looped.
	_Bool looped;
} Animation;

typedef struct {
	_Bool initialized;
	uint8_t layer_count;
	Animation layers[5];
} TheaterAnimation;

struct resources;

void animations_init(struct resources *resources);
Animation *animation_get(struct resources *resources, AnimationID animation_id);
TheaterAnimation *theater_animation_get(struct resources *resources,
					TheaterAnimationID animation_id);
void animation_free(Animation *animation);

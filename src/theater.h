#pragma once

#include "animations.h"
#include "menu.h"
#include "texture.h"
#include "vector2d.h"

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#define VIDEO_WIDTH 400
#define VIDEO_HEIGHT 300

typedef struct {
	_Bool played_first_frame;
	Texture texture_handle;
	AVRational time_base;
	double start_time;
	uint8_t frame_buffer[VIDEO_WIDTH * VIDEO_HEIGHT * 4];

	AVFormatContext *av_format_ctx;
	AVCodecContext *av_codec_ctx;
	AVFrame *av_frame;
	AVPacket *av_packet;
	int video_stream_idx;
} Video;

typedef enum {
	THEATER_CLASSROOM,
	THEATER_SCHOOL,
	THEATER_LAIN_ROOM_NIGHT,
	THEATER_ARISU_ROOM,
	THEATER_CYBERIA,
	THEATER_STREET,
	THEATER_BRIDGE
} TheaterType;

typedef struct {
	TheaterType type;

	uint8_t layer_count;
	Sprite layers[5];

	double last_updated;
	Scene scene;

	_Bool is_movie_playing;
	Video video;
} Theater;

struct minigame;

int start_theater(Menu *menu, Resources *resources, GameState *game_state,
		  struct minigame *minigame, GLFWwindow **minigame_window,
		  GLFWwindow *main_window);
void update_theater(Resources *resources, Menu *menu, GameState *game_state,
		    GLFWwindow *window, struct minigame *minigame);

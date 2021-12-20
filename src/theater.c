#include <libavutil/avutil.h>
#include <libavutil/error.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "animations.h"
#include "minigame.h"
#include "portaudio.h"
#include "resources.h"
#include "scene.h"
#include "sndfile.h"
#include "sound.h"
#include "sprite.h"
#include "state.h"
#include "texture.h"
#include "theater.h"
#include "vector2d.h"
#include "window.h"

static int movie_video_init(Video *v)
{
	v->played_first_frame = false;

	// allocate context
	v->av_format_ctx = avformat_alloc_context();
	if (v->av_format_ctx == NULL) {
		printf("Failed to allocate format context.\n");
		return 0;
	}

	// open file
	if (avformat_open_input(&v->av_format_ctx, "./lain_mov.dat", NULL,
				NULL) != 0) {
		printf("Couldnt open video file.\n");
		return 0;
	};

	// find stream index
	v->video_stream_idx = -1;
	AVCodecParameters *av_codec_params;
	AVCodec *av_codec;
	for (int i = 0; i < v->av_format_ctx->nb_streams; i++) {
		AVStream *stream = v->av_format_ctx->streams[i];
		av_codec_params = stream->codecpar;
		av_codec = avcodec_find_decoder(av_codec_params->codec_id);

		if (av_codec == NULL) {
			continue;
		}

		if (av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO) {
			v->video_stream_idx = i;
			v->time_base = stream->time_base;
			break;
		}
	}

	v->av_codec_ctx = avcodec_alloc_context3(av_codec);
	if (v->av_codec_ctx == NULL) {
		printf("Couldn't allocate codec context.\n");
		return 0;
	}

	if (avcodec_parameters_to_context(v->av_codec_ctx, av_codec_params) <
	    0) {
		printf("Couldn't initialize codec context.\n");
		return 0;
	}

	if (avcodec_open2(v->av_codec_ctx, av_codec, NULL) < 0) {
		printf("Failed to open codec.\n");
		return 0;
	}

	v->av_frame = av_frame_alloc();
	if (v->av_frame == NULL) {
		printf("Couldn't allocate frame memory.\n");
		return 0;
	}

	v->av_packet = av_packet_alloc();
	if (v->av_packet == NULL) {
		printf("Couldn't allocate packet memory.\n");
		return 0;
	}

	v->sws_ctx = sws_getContext(
	    VIDEO_WIDTH, VIDEO_HEIGHT, v->av_codec_ctx->pix_fmt, VIDEO_WIDTH,
	    VIDEO_HEIGHT, AV_PIX_FMT_RGB0, SWS_BILINEAR, NULL, NULL, NULL);

	if (v->sws_ctx == NULL) {
		printf("Failed to create sws context.\n");
		return 0;
	}

	return 1;
}

static int movie_video_read_frame(Video *v, int64_t *pts, uint8_t *frame_buffer)
{
	int response, read_frame_status;
	while ((read_frame_status =
		    av_read_frame(v->av_format_ctx, v->av_packet) >= 0)) {
		if (v->av_packet->stream_index != v->video_stream_idx) {
			av_packet_unref(v->av_packet);
			continue;
		}

		response = avcodec_send_packet(v->av_codec_ctx, v->av_packet);
		if (response < 0) {
			printf("Failed to decode packet: %s\n",
			       av_err2str(response));
			av_packet_unref(v->av_packet);
			return 0;
		}

		response = avcodec_receive_frame(v->av_codec_ctx, v->av_frame);
		if (response == AVERROR(EAGAIN)) {
			av_packet_unref(v->av_packet);
			continue;
		} else if (response < 0) {
			printf("Failed to decode packet: %s\n",
			       av_err2str(response));
			av_packet_unref(v->av_packet);
			return 0;
		}
		av_packet_unref(v->av_packet);
		break;
	}

	if (!read_frame_status) {
		av_packet_unref(v->av_packet);
		return 0;
	}

	uint8_t *dest[4] = {frame_buffer, NULL, NULL, NULL};
	int dest_linesize[4] = {v->av_frame->width * 4, 0, 0, 0};

	// flip the output
	v->av_frame->data[0] +=
	    v->av_frame->linesize[0] * (v->av_frame->height - 1);
	v->av_frame->linesize[0] = -v->av_frame->linesize[0];

	sws_scale(v->sws_ctx, v->av_frame->data, v->av_frame->linesize, 0,
		  v->av_frame->height, dest, dest_linesize);

	*pts = v->av_frame->pts;
	*frame_buffer = dest;

	return 1;
}

void movie_video_free(Video *v)
{
	sws_freeContext(v->sws_ctx);
	avformat_close_input(&v->av_format_ctx);
	avformat_free_context(v->av_format_ctx);
	av_frame_free(&v->av_frame);
	av_packet_free(&v->av_packet);
	avcodec_free_context(&v->av_codec_ctx);
}

static int init_movie(Theater *theater)
{
	Video *v = &theater->video;

	// init video
	if (!movie_video_init(v)) {
		printf("Failed to initialize movie state.\n");
		return 0;
	}

	glGenTextures(1, &v->texture_handle.gl_id);
	glBindTexture(GL_TEXTURE_2D, v->texture_handle.gl_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	v->texture_handle.size = (Vector2D){VIDEO_WIDTH, VIDEO_HEIGHT};

	// init scene
	// free old scene
	free_scene(&theater->scene);

	Sprite *sprites[5] = {&theater->layers[0]};

	make_sprite(&theater->layers[0],
		    (Sprite){.visible = true,
			     .pos = {300.0f, 200.0f},
			     .pivot_centered = true,
			     .texture = &v->texture_handle});

	init_scene(&theater->scene, sprites, 1, NULL, 0, NULL, 0, NULL, 0);

	// finalize
	theater->type = THEATER_MOVIE;

	return 1;
}

static inline int get_lain_walk_animation(LainOutfit outfit)
{
	switch (outfit) {
	case OUTFIT_SCHOOL:
		return SCHOOL_LAIN_THEATER_WALK_ANIMATION;
	case OUTFIT_DEFAULT:
		return DEFAULT_LAIN_THEATER_WALK_ANIMATION;
	case OUTFIT_CYBERIA:
		return CYBERIA_LAIN_THEATER_WALK_ANIMATION;
	case OUTFIT_ALIEN:
		return ALIEN_LAIN_THEATER_WALK_ANIMATION;
	case OUTFIT_SWEATER:
		return SWEATER_LAIN_THEATER_WALK_ANIMATION;
	case OUTFIT_BEAR:
		return BEAR_LAIN_THEATER_WALK_ANIMATION;
	}
}

static void load_theater_animation(GameState *game_state, Resources *resources,
				   Theater *theater,
				   TheaterAnimationID animation_id)
{
	TheaterAnimation *animation =
	    theater_animation_get(resources, animation_id);

	for (int i = 0; i < animation->layer_count; i++) {
		Animation *layer_animation = &animation->layers[i];
		Sprite *layer = &theater->layers[i];

		make_sprite(&theater->layers[i], (Sprite){
						     .z_index = i,
						 });

		sprite_set_animation_direct(resources, game_state->time, layer,
					    layer_animation);
	}

	theater->layer_count = animation->layer_count;
}

static void init_theater(Resources *resources, GameState *game_state,
			 Theater *theater, int preview)
{
	LainOutfit lain_outfit = game_state->lain.outfit;
	LainToolState lain_tool_state = game_state->lain.tool_state;

	Sprite *sprites[5] = {&theater->layers[0], &theater->layers[1],
			      &theater->layers[2], &theater->layers[3],
			      &theater->layers[4]};

	switch (preview) {
	case CLASSROOM_PREVIEW: {
		theater->type = THEATER_CLASSROOM;

		if (lain_outfit == OUTFIT_SCHOOL) {
			load_theater_animation(game_state, resources, theater,
					       THEATER_CLASSROOM_ANIMATION);
		} else {
			make_sprite(&theater->layers[0],
				    (Sprite){.visible = true,
					     .pos = {0.0f, 0.0f},
					     .z_index = 0,
					     .texture = texture_get(
						 resources, CLASSROOM_BG)});

			make_sprite(&theater->layers[1],
				    (Sprite){.visible = true,
					     .pos = {0.0f, 0.0f},
					     .z_index = 2,
					     .texture = texture_get(
						 resources, CLASSROOM_TABLES)});

			make_sprite(&theater->layers[2],
				    (Sprite){.visible = true, .z_index = 1});
			sprite_set_animation(
			    resources, game_state->time, &theater->layers[2],
			    get_lain_walk_animation(lain_outfit));

			theater->layer_count = 3;
		}
		break;
	}
	case SCHOOL_PREVIEW: {
		theater->type = THEATER_SCHOOL;

		if (lain_outfit == OUTFIT_SCHOOL &&
		    lain_tool_state == HOLDING_NAVI) {
			load_theater_animation(game_state, resources, theater,
					       THEATER_SCHOOL_ANIMATION);
		} else {
			make_sprite(&theater->layers[0],
				    (Sprite){.visible = true,
					     .pos = {0.0f, 0.0f},
					     .z_index = 0,
					     .texture = texture_get(
						 resources, SCHOOL_BG)});

			make_sprite(&theater->layers[1],
				    (Sprite){.visible = true, .z_index = 1});
			sprite_set_animation(
			    resources, game_state->time, &theater->layers[1],
			    get_lain_walk_animation(lain_outfit));

			theater->layer_count = 2;
		}
		break;
	}
	case LAIN_ROOM_NIGHT_PREVIEW: {
		theater->type = THEATER_LAIN_ROOM_NIGHT;

		if (lain_outfit == OUTFIT_BEAR) {
			load_theater_animation(
			    game_state, resources, theater,
			    THEATER_LAIN_ROOM_NIGHT_ANIMATION);
		} else {
			make_sprite(
			    &theater->layers[0],
			    (Sprite){.visible = true,
				     .pos = {0.0f, 0.0f},
				     .z_index = 0,
				     .texture = texture_get(
					 resources, LAIN_ROOM_NIGHT_BG)});

			make_sprite(&theater->layers[1],
				    (Sprite){.visible = true, .z_index = 1});
			sprite_set_animation(
			    resources, game_state->time, &theater->layers[1],
			    get_lain_walk_animation(lain_outfit));

			theater->layer_count = 2;
		}
		break;
	}
	case ARISU_ROOM_PREVIEW: {
		theater->type = THEATER_ARISU_ROOM;

		if (lain_outfit == OUTFIT_ALIEN) {
			load_theater_animation(game_state, resources, theater,
					       THEATER_ARISU_ROOM_ANIMATION);
		} else {
			make_sprite(&theater->layers[0],
				    (Sprite){.visible = true,
					     .pos = {0.0f, 0.0f},
					     .z_index = 0,
					     .texture = texture_get(
						 resources, ARISU_ROOM_BG)});

			make_sprite(&theater->layers[1],
				    (Sprite){.visible = true, .z_index = 1});
			sprite_set_animation(
			    resources, game_state->time, &theater->layers[1],
			    get_lain_walk_animation(lain_outfit));

			theater->layer_count = 2;
		}

		break;
	}
	case CYBERIA_PREVIEW: {
		theater->type = THEATER_CYBERIA;

		if (lain_outfit == OUTFIT_CYBERIA) {
			load_theater_animation(game_state, resources, theater,
					       THEATER_CYBERIA_ANIMATION);
		} else {
			make_sprite(&theater->layers[0],
				    (Sprite){.visible = true,
					     .pos = {0.0f, 0.0f},
					     .z_index = 0,
					     .texture = texture_get(
						 resources, CYBERIA_BG)});

			make_sprite(&theater->layers[1],
				    (Sprite){.visible = true, .z_index = 1});
			sprite_set_animation(
			    resources, game_state->time, &theater->layers[1],
			    get_lain_walk_animation(lain_outfit));

			theater->layer_count = 2;
		}
		break;
	}
	case STREET_PREVIEW: {
		theater->type = THEATER_STREET;

		if (lain_outfit == OUTFIT_SWEATER) {
			load_theater_animation(game_state, resources, theater,
					       THEATER_STREET_ANIMATION);
		} else {
			make_sprite(&theater->layers[0],
				    (Sprite){.visible = true,
					     .pos = {0.0f, 0.0f},
					     .z_index = 0,
					     .texture = texture_get(
						 resources, STREET_BG_1)});

			make_sprite(&theater->layers[1],
				    (Sprite){.visible = true, .z_index = 1});
			sprite_set_animation(
			    resources, game_state->time, &theater->layers[1],
			    get_lain_walk_animation(lain_outfit));

			theater->layer_count = 2;
		}
		break;
	}
	case BRIDGE_PREVIEW: {
		theater->type = THEATER_BRIDGE;

		load_theater_animation(game_state, resources, theater,
				       THEATER_BRIDGE_ANIMATION);

		break;
	}
	}

	init_scene(&theater->scene, sprites, theater->layer_count, NULL, 0,
		   NULL, 0, NULL, 0);
}

void update_theater(Resources *resources, Menu *menu, GameState *game_state,
		    GLFWwindow *window, Minigame *minigame)
{

	Theater *theater = &minigame->current.theater;

	_Bool was_animated = false;
	if (theater->type == THEATER_MOVIE) {
		Video *v = &theater->video;

		int64_t pts;

		if (movie_video_read_frame(v, &pts, v->frame_buffer)) {
			if (!v->played_first_frame) {
				v->played_first_frame = true;
				v->start_time = game_state->time;
			}

			double pt_in_seconds = pts * (double)v->time_base.num /
					       (double)v->time_base.den;

			while (pt_in_seconds > glfwGetTime() - v->start_time) {
				glfwWaitEventsTimeout(
				    pt_in_seconds -
				    (glfwGetTime() - v->start_time));
			}

			glBindTexture(GL_TEXTURE_2D, v->texture_handle.gl_id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, VIDEO_WIDTH,
				     VIDEO_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				     v->frame_buffer);

			was_animated = true;
		}
	} else {
		// hacky way of detecting whether or not the theater is over
		// if it wasnt animated, that means no animation in the scene
		// has a next frame, which means its over.
		for (int i = 0; i < cvector_size(theater->scene.sprites); ++i) {
			Sprite *curr = theater->scene.sprites[i];
			if (curr->animation != NULL) {
				sprite_try_next_frame(resources,
						      game_state->time, curr);
				was_animated = true;
			}
		}
	}

	if (!was_animated && theater->type == THEATER_BRIDGE) {
		init_movie(theater);
		was_animated = true;
	}

	if (!was_animated || glfwWindowShouldClose(window)) {
		play_sound(&resources->audio_engine, SND_111);
		destroy_minigame(resources->textures, menu, minigame, window);
		return;
	}

	update_scene(&theater->scene);
}

int start_theater(Menu *menu, Resources *resources, GameState *game_state,
		  Minigame *minigame, GLFWwindow **minigame_window,
		  GLFWwindow *main_window)
{

	if (!(make_window(minigame_window, MINIGAME_WIDTH, MINIGAME_HEIGHT,
			  "lain theatre", main_window, true))) {
		printf("Failed to create theater window.\n");
		return 0;
	}

	init_theater(resources, game_state, &minigame->current.theater,
		     menu->theater_preview.texture->id);

	minigame->type = THEATER;
	minigame->last_updated = game_state->time;

	return 1;
}

#include <stdio.h>

#include "embedded.h"
#include "sprite.h"
#include "stb_image.h"
#include "texture.h"

static void init_texture(Texture *texture, const unsigned char *bytes,
			 int length)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, nr_channels;
	unsigned char *data = stbi_load_from_memory(bytes, length, &width,
						    &height, &nr_channels, 0);
	if (data == NULL) {
		printf("Failed to load texture.\n");
		exit(1);
	}

	texture->size = (Vector2D){width, height};
	texture->nr_channels = nr_channels;

	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->size.x,
		     texture->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(data);
}

// clang-format off
void textures_init(Texture *textures)
{
	init_texture(&textures[BEAR_ICON_ACTIVE],
		     bear_icon_active_png,
		     bear_icon_active_png_size);

	init_texture(&textures[BEAR_ICON_INACTIVE],
		     bear_icon_inactive_png,
		     bear_icon_inactive_png_size);

	init_texture(&textures[UI_LAIN_BEAR],
		     ui_lain_bear_png,
		     ui_lain_bear_png_size);

	init_texture(&textures[UI_LAIN],
		     ui_lain_png,
		     ui_lain_png_size);

	init_texture(&textures[MAIN_UI],
		     main_ui_png,
		     main_ui_png_size);

	init_texture(&textures[MAIN_UI_BAR_ACTIVE],
		     main_ui_bar_active_png,
		     main_ui_bar_active_png_size);

	init_texture(&textures[MAIN_UI_BAR_INACTIVE],
		     main_ui_bar_inactive_png,
		     main_ui_bar_inactive_png_size);

	init_texture(&textures[DRESSUP_BUTTON_INACTIVE],
		     dressup_button_inactive_png,
		     dressup_button_inactive_png_size);

	init_texture(&textures[DRESSUP_BUTTON_ACTIVE],
		     dressup_button_active_png,
		     dressup_button_active_png_size);

	init_texture(&textures[THEATER_PREVIEWS],
		     theater_previews_png,
		     theater_previews_png_size);

	init_texture(&textures[THEATER_BUTTON_ACTIVE],
		     theater_button_active_png,
		     theater_button_active_png_size);

	init_texture(&textures[THEATER_BUTTON_INACTIVE],
		     theater_button_inactive_png,
		     theater_button_inactive_png_size);

	init_texture(&textures[SCREWDRIVER_ICON_INACTIVE],
		     screwdriver_icon_inactive_png,
		     screwdriver_icon_inactive_png_size);

	init_texture(&textures[PAW_ICON],
		     paw_icon_png,
		     paw_icon_png_size);

	init_texture(&textures[SCORE_PREVIEW],
		     score_preview_png,
		     score_preview_png_size);

	init_texture(&textures[RED_FONT_TEXTURE],
		     red_font_png,
		     red_font_png_size);

	init_texture(&textures[WHITE_FONT_TEXTURE],
		     white_font_png,
		     white_font_png_size);

	init_texture(&textures[KUMA_SHOOT_BG],
		     kuma_shoot_bg_png,
		     kuma_shoot_bg_png_size);

	init_texture(&textures[KUMA_SHOOT_BUSH_OVERLAY],
		     bush_overlay_png,
		     bush_overlay_png_size);

	init_texture(&textures[BROWN_BEAR_MOVE_RIGHT],
		     brown_bear_move_right_png,
		     brown_bear_move_right_png_size);

	init_texture(&textures[WHITE_BEAR_MOVE_RIGHT],
		     white_bear_move_right_png,
		     white_bear_move_right_png_size);

	init_texture(&textures[KUMA_SHOOT_SMOKE],
		     kuma_shoot_smoke_png,
		     kuma_shoot_smoke_png_size);

	init_texture(&textures[KUMA_SHOOT_MIHO],
		     kuma_shoot_miho_png,
		     kuma_shoot_miho_png_size);

	init_texture(&textures[KUMA_SHOOT_MIKA],
		     kuma_shoot_mika_png,
		     kuma_shoot_mika_png_size);

	init_texture(&textures[KUMA_SHOOT_YASUO],
		     kuma_shoot_yasuo_png,
		     kuma_shoot_yasuo_png_size);

	init_texture(&textures[KUMA_SHOOT_SCREWDRIVER_LAIN],
		     kuma_shoot_screwdriver_lain_png,
		     kuma_shoot_screwdriver_lain_png_size);

	init_texture(&textures[KUMA_SHOOT_SCHOOL_LAIN],
		     kuma_shoot_school_lain_png,
		     kuma_shoot_school_lain_png_size);

	init_texture(&textures[KUMA_SHOOT_DEFAULT_LAIN],
		     kuma_shoot_default_lain_png,
		     kuma_shoot_default_lain_png_size);

	init_texture(&textures[KUMA_SHOOT_EXPLOSION],
		     kuma_shoot_explosion_png,
		     kuma_shoot_explosion_png_size);

	init_texture(&textures[DRESSUP_BEAR_OUTFIT],
		     bear_outfit_png,
		     bear_outfit_png_size);

	init_texture(&textures[DRESSUP_SCHOOL_OUTFIT],
		     school_outfit_png,
		     school_outfit_png_size);

	init_texture(&textures[DRESSUP_PAJAMA_OUTFIT],
		     pajama_outfit_png,
		     pajama_outfit_png_size);

	init_texture(&textures[DRESSUP_CYBERIA_OUTFIT],
		     cyberia_outfit_png,
		     cyberia_outfit_png_size);

	init_texture(&textures[DRESSUP_UFO],
		     dressup_ufo_png,
		     dressup_ufo_png_size);

	init_texture(&textures[DRESSUP_NAVI],
		     dressup_navi_png,
		     dressup_navi_png_size);

	init_texture(&textures[DRESSUP_SCREWDRIVER],
		     dressup_screwdriver_png,
		     dressup_screwdriver_png_size);

	init_texture(&textures[LAIN_ROOM],
		     lain_room_png,
		     lain_room_png_size);

	init_texture(&textures[LAIN_CYBERIA_STANDING],
		     lain_cyberia_standing_png,
		     lain_cyberia_standing_png_size);

	init_texture(&textures[LAIN_CYBERIA_LEAVE],
		     lain_cyberia_leave_png,
		     lain_cyberia_leave_png_size);

	init_texture(&textures[LAIN_CYBERIA_MOVE_LEFT],
		     lain_cyberia_walk_left_png,
		     lain_cyberia_walk_left_png_size);

	init_texture(&textures[LAIN_ALIEN_STANDING],
		     lain_alien_standing_png,
		     lain_alien_standing_png_size);

	init_texture(&textures[LAIN_ALIEN_LEAVE],
		     lain_alien_leave_png,
		     lain_alien_leave_png_size);

	init_texture(&textures[LAIN_ALIEN_MOVE_LEFT],
		     lain_alien_walk_left_png,
		     lain_alien_walk_left_png_size);

	init_texture(&textures[LAIN_SCHOOL_STANDING],
		     lain_school_standing_png,
		     lain_school_standing_png_size);

	init_texture(&textures[LAIN_SCHOOL_LEAVE],
		     lain_school_leave_png,
		     lain_school_leave_png_size);

	init_texture(&textures[LAIN_SCHOOL_MOVE_LEFT],
		     lain_school_walk_left_png,
		     lain_school_walk_left_png_size);

	init_texture(&textures[LAIN_PAJAMA_STANDING],
		     lain_pajama_standing_png,
		     lain_pajama_standing_png_size);

	init_texture(&textures[LAIN_PAJAMA_LEAVE],
		     lain_pajama_leave_png,
		     lain_pajama_leave_png_size);

	init_texture(&textures[LAIN_PAJAMA_MOVE_LEFT],
		     lain_pajama_leave_png,
		     lain_pajama_leave_png_size);

	init_texture(&textures[LAIN_BEAR_STANDING],
		     lain_bear_standing_png,
		     lain_bear_standing_png_size);

	init_texture(&textures[LAIN_BEAR_LEAVE],
		     lain_bear_leave_png,
		     lain_bear_leave_png_size);

	init_texture(&textures[LAIN_BEAR_MOVE_LEFT],
		     lain_bear_walk_left_png,
		     lain_bear_walk_left_png_size);


}
// clang-format on

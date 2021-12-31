#!/usr/bin/env python3

import sys
import numpy as np
import cv2
from PIL import Image
import os
from os import path, mkdir, remove, listdir
from shutil import move, copyfile
from distutils.dir_util import copy_tree
import json
import collections
import pathlib
import extract_n_decompress

# theater animation ids
# inbetween these are "generic theaters", where lain simply walks past a scene
# ["202", "216", "209", "223", "237", "244", "230"]

# offsets so far:
# 155 - all 0  main_ui_expand
# 246 - all 0  main_ui_shrink

# 143 - all 40 ui_default_lain_blink
# 145 - all 40 ui_default_lain_laugh
# 146 - all 40 ui_default_lain_laugh_blink

# 149 - ui_bear_lain_blink
# 151 - ui_bear_lain_laugh
# 152 - ui_bear_laugh_blink

# 128 - brown bear walk animation
# 130 - white bear walk animation
# 132 - kumashoot smoke
# 134 - miho
# 133 - yasuo
# 135 - mika
# 136 - school lain scratch head kumashoot
# 137 - school lain kumashoot
# 139 - screwdriver lain kumashoot
# 140 - default lain kumashoot

# 189 - dressup default laeve
# 183 - dressup default walk left

# 190 - dressup schol leave
# 184 - dressup school walk left

# 191 - dressup cyberia leave
# 185 - dressup cyberia walk left

# 192 - dressup bear leave
# 186 - dressup bear walk left

# 193 - dressup sweater leave
# 187 - dressup sweater walk left

# 194 - dressup alien leave
# 188 - dressup alien walk left

animations = [
    {"id": "155", "name": "MAIN_UI_EXPAND_ANIMATION", "null_offsets": True},
    {"id": "246", "name": "MAIN_UI_COLLAPSE_ANIMATION", "null_offsets": True},
    {
        "id": "143",
        "name": "UI_DEFAULT_LAIN_BLINK_ANIMATION",
        "null_offsets": True,
        "additional": [
            {"timing": 12, "rsrc_id": 442, "x_offset": -1, "y_offset": -1, "index": 4}
        ],
    },
    {"id": "145", "name": "UI_DEFAULT_LAIN_LAUGH_ANIMATION", "null_offsets": True},
    {
        "id": "146",
        "name": "UI_DEFAULT_LAIN_LAUGH_BLINK_ANIMATION",
        "null_offsets": True,
        "additional": [
            {"timing": 12, "rsrc_id": 446, "x_offset": -1, "y_offset": -1, "index": 4}
        ],
    },
    {
        "id": "149",
        "name": "UI_BEAR_LAIN_BLINK_ANIMATION",
        "null_offsets": True,
        "additional": [
            {"timing": 12, "rsrc_id": 451, "x_offset": -1, "y_offset": -1, "index": 4}
        ],
    },
    {"id": "151", "name": "UI_BEAR_LAIN_LAUGH_ANIMATION", "null_offsets": True},
    {
        "id": "152",
        "name": "UI_BEAR_LAIN_LAUGH_BLINK_ANIMATION",
        "null_offsets": True,
        "additional": [
            {"timing": 12, "rsrc_id": 455, "x_offset": -1, "y_offset": -1, "index": 4}
        ],
    },
    {"id": "128", "name": "KUMA_SHOOT_BROWN_BEAR_WALK_ANIMATION", "null_offsets": True},
    {"id": "130", "name": "KUMA_SHOOT_WHITE_BEAR_WALK_ANIMATION", "null_offsets": True},
    {"id": "132", "name": "KUMA_SHOOT_SMOKE_ANIMATION", "null_offsets": True},
    {"id": "133", "name": "KUMA_SHOOT_YASUO_ANIMATION", "null_offsets": True},
    {"id": "134", "name": "KUMA_SHOOT_MIHO_ANIMATION", "null_offsets": True},
    {"id": "135", "name": "KUMA_SHOOT_MIKA_ANIMATION", "null_offsets": True},
    {"id": "136", "name": "KUMA_SHOOT_SCHOOL_LAIN_1_ANIMATION", "null_offsets": True},
    {"id": "137", "name": "KUMA_SHOOT_SCHOOL_LAIN_2_ANIMATION", "null_offsets": True},
    {
        "id": "139",
        "layer": "0",
        "name": "KUMA_SHOOT_SCREWDRIVER_LAIN_ANIMATION",
        "null_offsets": True,
    },
    {
        "id": "139",
        "layer": "1",
        "name": "KUMA_SHOOT_SCREW_ANIMATION",
        "null_offsets": True,
    },
    {"id": "140", "name": "KUMA_SHOOT_DEFAULT_LAIN_ANIMATION", "null_offsets": True},
    {"id": "189", "name": "LAIN_DEFAULT_LEAVE_ANIMATION", "null_offsets": False},
    {"id": "183", "name": "LAIN_DEFAULT_WALK_LEFT_ANIMATION", "null_offsets": False},
    {"id": "190", "name": "LAIN_SCHOOL_LEAVE_ANIMATION", "null_offsets": False},
    {"id": "184", "name": "LAIN_SCHOOL_WALK_LEFT_ANIMATION", "null_offsets": False},
    {"id": "191", "name": "LAIN_CYBERIA_LEAVE_ANIMATION", "null_offsets": False},
    {"id": "185", "name": "LAIN_CYBERIA_WALK_LEFT_ANIMATION", "null_offsets": False},
    {"id": "192", "name": "LAIN_BEAR_LEAVE_ANIMATION", "null_offsets": False},
    {"id": "186", "name": "LAIN_BEAR_WALK_LEFT_ANIMATION", "null_offsets": False},
    {"id": "193", "name": "LAIN_SWEATER_LEAVE_ANIMATION", "null_offsets": False},
    {"id": "187", "name": "LAIN_SWEATER_WALK_LEFT_ANIMATION", "null_offsets": False},
    {"id": "194", "name": "LAIN_ALIEN_LEAVE_ANIMATION", "null_offsets": False},
    {"id": "188", "name": "LAIN_ALIEN_WALK_LEFT_ANIMATION", "null_offsets": False},
    {
        "id": "203",
        "layer": 1,
        "name": "DEFAULT_LAIN_THEATER_WALK_ANIMATION",
        "null_offsets": False,
    },
    {
        "id": "204",
        "layer": 1,
        "name": "SCHOOL_LAIN_THEATER_WALK_ANIMATION",
        "null_offsets": False,
    },
    {
        "id": "205",
        "layer": 1,
        "name": "CYBERIA_LAIN_THEATER_WALK_ANIMATION",
        "null_offsets": False,
    },
    {
        "id": "206",
        "layer": 1,
        "name": "BEAR_LAIN_THEATER_WALK_ANIMATION",
        "null_offsets": False,
    },
    {
        "id": "207",
        "layer": 1,
        "name": "SWEATER_LAIN_THEATER_WALK_ANIMATION",
        "null_offsets": False,
    },
    {
        "id": "208",
        "layer": 1,
        "name": "ALIEN_LAIN_THEATER_WALK_ANIMATION",
        "null_offsets": False,
    },
]

theater_animations = [
    {"id": "202", "name": "THEATER_CLASSROOM_ANIMATION"},
    {"id": "216", "name": "THEATER_LAIN_ROOM_NIGHT_ANIMATION"},
    {"id": "209", "name": "THEATER_SCHOOL_ANIMATION"},
    {"id": "223", "name": "THEATER_ARISU_ROOM_ANIMATION"},
    {"id": "230", "name": "THEATER_CYBERIA_ANIMATION"},
    {"id": "237", "name": "THEATER_STREET_ANIMATION"},
    {"id": "244", "name": "THEATER_BRIDGE_ANIMATION"},
]


def format_frames(frames, additional_frame, null_offsets):
    formatted_frames = []
    padded_first_frame = False
    for i, frame in enumerate(frames):
        if i == 0 and frame["timing"] > 1:
            formatted_frames.append(
                {"timing": 0, "rsrc_id": -1, "x_offset": -1, "y_offset": -1, "index": 0}
            )
            padded_first_frame = True

        formatted_frame = {}
        formatted_frame["timing"] = (
            0 if frames[i]["timing"] == 1 else frames[i]["timing"]
        )
        if frames[i]["res_id"] == 0:
            formatted_frame["rsrc_id"] = -1
        else:
            formatted_frame["rsrc_id"] = frames[i]["res_id"] - 128
        if null_offsets:
            formatted_frame["x_offset"] = -1
            formatted_frame["y_offset"] = -1
        else:
            formatted_frame["x_offset"] = frames[i]["rel_x"]
            formatted_frame["y_offset"] = frames[i]["rel_y"]
        formatted_frame["index"] = i + 1 if padded_first_frame else i

        formatted_frames.append(formatted_frame)

    return formatted_frames


def extract_layers(frames, target_layer, additional_frame, null_offsets):
    layers = {}
    for frame in frames:
        curr_layer = frame["layer"] - 1 if frame["layer"] - 1 > 0 else 0

        if curr_layer not in layers:
            layers[curr_layer] = []
            layers[curr_layer].append(frame)
        else:
            layers[curr_layer].append(frame)

    for layer in layers:
        layers[layer] = format_frames(layers[layer], additional_frame, null_offsets)

    if target_layer != None:
        return layers[int(target_layer)]
    else:
        return layers


def get_max_width_height(images):
    width_max = 0
    height_max = 0
    for img in images:
        h, w = img.shape[:2]
        width_max = max(width_max, w)
        height_max = max(height_max, h)

    return (width_max, height_max)


def pad_images_to_same_size(images):
    width_max, height_max = get_max_width_height(images)

    images_padded = []
    for img in images:
        h, w = img.shape[:2]
        diff_vert = height_max - h
        pad_top = diff_vert // 2
        pad_bottom = diff_vert - pad_top
        diff_hori = width_max - w
        pad_left = diff_hori // 2
        pad_right = diff_hori - pad_left
        img_padded = cv2.copyMakeBorder(
            img, pad_top, pad_bottom, pad_left, pad_right, cv2.BORDER_CONSTANT, value=0
        )
        assert img_padded.shape[:2] == (height_max, width_max)
        images_padded.append(img_padded)

    return images_padded


def make_bushes(curr_dir):
    img_width, img_height = 600, 400
    n_channels = 4

    res = np.zeros((img_height, img_width, n_channels), dtype=np.uint8)

    bush_poses = [
        [55, 6],
        [17, 76],
        [216, 33],
        [50, 191],
        [24, 246],
        [455, 58],
        [419, 128],
        [563, 99],
        [400, 270],
    ]

    for i, pos in enumerate(bush_poses):
        sprite_id = "{}.png".format(i + 517)
        bush_img = cv2.imread(path.join(curr_dir, sprite_id), -1)

        x = pos[0]
        y = pos[1]

        x1, x2 = x, x + bush_img.shape[1]
        y1, y2 = y, y + bush_img.shape[0]

        res[y1:y2, x1:x2] = bush_img

        remove(path.join(curr_dir, sprite_id))

    move(path.join(curr_dir, "526.png"), path.join(curr_dir, "494.png"))
    cv2.imwrite(path.join(curr_dir, "495.png"), res)


def make_fonts(curr_dir):
    white_font_characters = []
    for i in range(492, 505):
        filename = f"{i}.png"
        white_font_characters.append(
            cv2.imread(path.join(curr_dir, filename), cv2.IMREAD_UNCHANGED)
        )
        remove(path.join(curr_dir, filename))

    white_font = cv2.hconcat(pad_images_to_same_size(white_font_characters))

    red_font_characters = []
    for i in range(505, 516):
        filename = f"{i}.png"
        red_font_characters.append(
            cv2.imread(path.join(curr_dir, filename), cv2.IMREAD_UNCHANGED)
        )
        remove(path.join(curr_dir, filename))

    red_font = cv2.hconcat(pad_images_to_same_size(red_font_characters))

    cv2.imwrite(path.join(curr_dir, "492.png"), white_font)
    cv2.imwrite(path.join(curr_dir, "493.png"), red_font)


def apply_mask(img_src, mask_src):
    with Image.open(img_src) as img, Image.open(mask_src) as mask:
        mask_pixels = mask.load()
        mask_pixels[0, mask.size[1] - 1] = mask_pixels[1, mask.size[1] - 1]
        img = img.convert("RGBA")
        mask = mask.convert("1")
        img.paste(0, mask=mask)
        return img


DECOMPRESSED_DIR = "./extracted/sprites/decompressed/"
SOUNDS_DIR = "./extracted/sounds/"
DAT_JSON = "./extracted/lain_win.json"
MOV_FILE = "./binaries/lain_mov.dat"
ICON_FILE = "./extracted/window_icon.png"

SPRITES_DEST_DIR = "./res/sprites"
SOUNDS_DEST_DIR = "./res/sounds"


def main():
    if not path.isdir(path.join(os.getcwd(), "extracted")):
        extract_n_decompress.main()

    pathlib.Path(SPRITES_DEST_DIR).mkdir(parents=True, exist_ok=True)
    pathlib.Path(SOUNDS_DEST_DIR).mkdir(parents=True, exist_ok=True)

    # apply masks
    for i in range(527):
        mask_src = os.path.join(os.getcwd(), DECOMPRESSED_DIR, str(i + 527))
        img_src = os.path.join(os.getcwd(), DECOMPRESSED_DIR, str(i))

        if os.path.exists(mask_src):
            print("Applying mask {}...".format(i))
            dest_path = os.path.join(os.getcwd(), SPRITES_DEST_DIR, "{}.png".format(i))

            if not os.path.exists(dest_path):
                newimg = apply_mask(img_src, mask_src)
                newimg.save(dest_path, "png")

    make_fonts(SPRITES_DEST_DIR)

    make_bushes(SPRITES_DEST_DIR)

    move(path.join(SPRITES_DEST_DIR, "516.png"), path.join(SPRITES_DEST_DIR, "496.png"))

    print("Copying sound files...")
    copy_tree(SOUNDS_DIR, SOUNDS_DEST_DIR)

    print("Copying window icon...")
    copyfile(ICON_FILE, os.path.join(os.getcwd(), "res", "window_icon.png"))

    print("Extracting animation data...")
    with open(DAT_JSON) as f:
        content = json.load(f)
        final = {}
        final["sprite_animations"] = {}
        final["theater_scenes"] = {}

        for anim in animations:
            frames = content["animations"][anim["id"]]["frames"]
            has_additional_frame = "additional" in anim
            if "layer" in anim:
                final["sprite_animations"][anim["name"]] = extract_layers(
                    frames, anim["layer"], has_additional_frame, anim["null_offsets"]
                )
            else:
                final["sprite_animations"][anim["name"]] = format_frames(
                    frames, has_additional_frame, anim["null_offsets"]
                )
            if has_additional_frame:
                for frame in anim["additional"]:
                    final["sprite_animations"][anim["name"]].append(frame)

        for anim in theater_animations:
            frames = content["animations"][anim["id"]]["frames"]
            layers = extract_layers(frames, None, False, False)
            ordered_layers = collections.OrderedDict(sorted(layers.items()))
            final["theater_scenes"][anim["name"]] = ordered_layers

        with open(os.path.join(os.getcwd(), "res", "animations.json"), "w") as f:
            f.write(json.dumps(final, indent=2))

    copyfile(MOV_FILE, os.path.join(os.getcwd(), "res", "lain_mov.dat"))

    move(os.path.join(os.getcwd(), "res"), os.path.join(os.getcwd(), "../res"))


if __name__ == "__main__":
    main()

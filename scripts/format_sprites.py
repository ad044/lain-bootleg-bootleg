import sys
import cv2
from os import path, mkdir, rename, remove, listdir
from shutil import copyfile, rmtree, move

# wip, will expand the marks as more shit is needed

FORMAT = ".png"


class Sprite:
    def __init__(self, name, category, index):
        self.category = category
        self.name = name + FORMAT
        self.index = index


class ItemSprite(Sprite):
    def __init__(self, name, index):
        super().__init__(name, "items", index)


class LainSprite(Sprite):
    def __init__(self, name, index):
        super().__init__(name, "lain", index)


class UISprite(Sprite):
    def __init__(self, name, index):
        super().__init__(name, "ui", index)


class BackgroundSprite(Sprite):
    def __init__(self, name, index):
        super().__init__(name, "backgrounds", index)


# END INDEX IS INCLUSIVE
class SpriteSheet:
    def __init__(self, name, category, start_index, end_index):
        self.name = name + FORMAT
        self.category = category
        self.start_index = start_index
        self.end_index = end_index


class LainSpriteSheet(SpriteSheet):
    def __init__(self, name, start_index, end_index):
        super().__init__(name, "lain", start_index, end_index)


class UISpriteSheet(SpriteSheet):
    def __init__(self, name, start_index, end_index):
        super().__init__(name, "ui", start_index, end_index)


marks = [
    # BACKGROUNDS
    BackgroundSprite("lain_room", 128),
    # DEFAULT OUTFIT
    LainSprite("lain_default_standing", 129),
    LainSpriteSheet("lain_default_walk_right", 130, 140),
    LainSpriteSheet("lain_default_walk_left", 141, 148),
    # SCHOOL OUTFIT
    ItemSprite("school_outfit", 247),
    LainSprite("lain_school_standing", 243),
    LainSpriteSheet("lain_school_walk_right", 149, 159),
    LainSpriteSheet("lain_school_walk_left", 160, 167),
    # HAT OUTFIT
    ItemSprite("hat_outfit", 248),
    LainSprite("lain_hat_standing", 244),
    LainSpriteSheet("lain_hat_walk_right", 168, 178),
    LainSpriteSheet("lain_hat_walk_left", 179, 186),
    # BEAR OUTFIT
    ItemSprite("bear_outfit", 249),
    LainSprite("lain_bear_standing", 245),
    LainSpriteSheet("lain_bear_walk_right", 187, 197),
    LainSpriteSheet("lain_bear_walk_left", 198, 205),
    # PAJAMA OUTFIT
    ItemSprite("pajama_outfit", 250),
    LainSprite("lain_pajama_standing", 246),
    LainSpriteSheet("lain_pajama_walk_right", 206, 216),
    LainSpriteSheet("lain_pajama_walk_left", 217, 224),
    # ALIEN OUTFIT
    LainSprite("lain_alien_standing", 225),
    LainSpriteSheet("lain_alien_walk_right", 226, 234),
    LainSpriteSheet("lain_alien_walk_left", 235, 242),
    # UI
    UISpriteSheet("main_ui_wrapper_expand", 588, 593),
    UISprite("main_ui_expanded", 613),
    UISprite("main_ui_blue_part", 594),
    UISprite("main_ui_cyan_part", 595),
    UISprite("main_ui_yellow_", 596),
    UISprite("main_ui_red_part", 597),
]


def pad_images_to_same_size(images):
    width_max = 0
    height_max = 0
    for img in images:
        h, w = img.shape[:2]
        width_max = max(width_max, w)
        height_max = max(height_max, h)

    images_padded = []
    for img in images:
        h, w = img.shape[:2]
        diff_vert = height_max - h
        pad_top = diff_vert//2
        pad_bottom = diff_vert - pad_top
        diff_hori = width_max - w
        pad_left = diff_hori//2
        pad_right = diff_hori - pad_left
        img_padded = cv2.copyMakeBorder(
            img, pad_top, pad_bottom, pad_left, pad_right, cv2.BORDER_CONSTANT, value=0)
        assert img_padded.shape[:2] == (height_max, width_max)
        images_padded.append(img_padded)

    return images_padded


if __name__ == "__main__":
    src_dir = sys.argv[1]
    dest_dir = sys.argv[2]

    if not path.isdir(dest_dir):
        mkdir(dest_dir)
    else:
        rmtree(dest_dir)
        mkdir(dest_dir)

    # copy to new directory with respective numbers as names
    for file_name in listdir(src_dir):
        new_name = file_name.split("_")[3]
        copyfile(path.join(src_dir, file_name), path.join(dest_dir, new_name))

    for mark in marks:
        if isinstance(mark, Sprite):
            category_dir = path.join(dest_dir, mark.category)
            if not path.isdir(category_dir):
                mkdir(category_dir)
            src = path.join(dest_dir, str(mark.index))
            dest = path.join(category_dir, mark.name)

            print("Renaming {} to {}...".format(src, dest))
            move(path.abspath(src), path.abspath(dest))
        if isinstance(mark, SpriteSheet):
            category_dir = path.join(dest_dir, mark.category)
            if not path.isdir(category_dir):
                mkdir(category_dir)

            spritesheet_length = mark.end_index - mark.start_index

            print("Making spritesheet {}...".format(mark.name))
            sprites = [cv2.imread(path.join(dest_dir, str(i)), cv2.IMREAD_UNCHANGED)
                       for i in range(mark.start_index, mark.end_index + 1)]

            padded_sprites = pad_images_to_same_size(sprites)

            im_tile = cv2.hconcat(padded_sprites)
            dest = path.join(category_dir, mark.name)
            cv2.imwrite(dest, im_tile)

            # remove frames since they're useless now
            for i in range(mark.start_index, mark.end_index + 1):
                remove(path.join(dest_dir, str(i)))

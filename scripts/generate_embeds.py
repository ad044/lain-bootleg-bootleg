import os
from os import path


def get_file_bytes(src):
    f = open(src, "rb")
    try:
        byte_array = bytearray(f.read())
    finally:
        f.close()

    return byte_array


def format_bytes_to_str(byte_array):
    byte_str = ""
    for ch in byte_array:
        byte_str += "0x{:02X}, ".format(ch)

    return byte_str


DEBUG = False


class Resource:
    def __init__(self, index, name, byte_str):
        self.index = index
        self.name = name
        self.byte_str = byte_str


# byte str is huge so, to inspect output in cases of errors,
# debug mode replaces it with 0 for a less-laggy browsing experience


def init_resource(resource):
    return """
    unsigned char {name}[] = {{{byte_str}}};
    size_t {name}_size = sizeof({name});
    """.format(
        name=resource.name, byte_str=0 if DEBUG else resource.byte_str
    )


def create_load_call(resource):
    return """
    buffer[{index}] = (EmbeddedResource){{.bytes = {name}, .size = {name}_size}};
    """.format(
        index=resource.index, name=resource.name
    )


def create_source(
    texture_inits,
    sound_inits,
    animations_json_bytes,
    window_icon_bytes,
    movie_video_bytes,
    texture_loads,
    sound_loads,
):
    return """
    #include "embedded.h"
    #include "texture.h"

    {}

    {}

    const unsigned char animations_json[] = {{{}}};
    const size_t animations_json_size = sizeof(animations_json);

    const unsigned char window_icon[] = {{{}}};
    const size_t window_icon_size = sizeof(window_icon);

    unsigned char movie_video[] = {{{}}};
    size_t movie_video_size = sizeof(movie_video);

    void load_textures(EmbeddedResource *buffer)
    {{{}}}

    void load_sounds(EmbeddedResource *buffer)
    {{{}}}
    """.format(
        texture_inits,
        sound_inits,
        animations_json_bytes,
        window_icon_bytes,
        movie_video_bytes,
        texture_loads,
        sound_loads,
    )


def create_header():
    return """
    #pragma once

    #include "texture.h"
    #include "sound.h"

    #include <stddef.h>

    typedef struct {
        unsigned char *bytes;
        size_t size;
    } EmbeddedResource;

    extern const unsigned char animations_json[];
    extern const size_t animations_json_size;

    extern const unsigned char window_icon[];
    extern const size_t window_icon_size;

    extern unsigned char movie_video[];
    extern size_t movie_video_size;

    void load_textures(EmbeddedResource *buffer);
    void load_sounds(EmbeddedResource *buffer);
    """


def compile_resources(src, dst):
    textures_dir = path.join(src, "sprites")
    sounds_dir = path.join(src, "sounds")
    if not path.isdir(dst):
        print("Destination must be a directory.")
        return

    texture_resources = []
    sound_resources = []

    for f in os.listdir(textures_dir):
        # X?D
        if f == "desktop.ini":
            continue

        print("Compiling {}...".format(f))

        filename, ext = path.splitext(f)[0], path.splitext(f)[1][1:]

        name = f"res_{filename.lower()}_{ext}"

        file_bytes = get_file_bytes(path.join(textures_dir, f))
        byte_str = format_bytes_to_str(file_bytes)

        rsrc = Resource(filename, name, byte_str)

        texture_resources.append(rsrc)

    for f in os.listdir(sounds_dir):
        # X?D
        if f == "desktop.ini":
            continue

        print("Compiling {}...".format(f))

        filename, ext = path.splitext(f)[0], path.splitext(f)[1][1:]

        index = f"SND_{filename}"
        name = f"res_snd_{filename.lower()}_{ext}"

        file_bytes = get_file_bytes(path.join(sounds_dir, f))
        byte_str = format_bytes_to_str(file_bytes)

        rsrc = Resource(index, name, byte_str)

        sound_resources.append(rsrc)

    print("Compiling movie audio...")
    movie_audio_bytes = get_file_bytes(path.join(src, "movie_audio.opus"))
    movie_audio_byte_str = format_bytes_to_str(movie_audio_bytes)
    movie_audio = Resource("SND_MOVIE_AUDIO", "res_snd_movie_audio", movie_audio_byte_str)
    sound_resources.append(movie_audio)

    texture_resource_inits = []
    texture_resource_loads = []

    sound_resource_inits = []
    sound_resource_loads = []

    for rsrc in texture_resources:
        texture_resource_inits.append(init_resource(rsrc))
        texture_resource_loads.append(create_load_call(rsrc))

    for rsrc in sound_resources:
        sound_resource_inits.append(init_resource(rsrc))
        sound_resource_loads.append(create_load_call(rsrc))

    texture_inits_str = "".join("{}".format(i) for i in texture_resource_inits)
    texture_loads_str = "".join("{}".format(i) for i in texture_resource_loads)

    sound_inits_str = "".join("{}".format(i) for i in sound_resource_inits)
    sound_loads_str = "".join("{}".format(i) for i in sound_resource_loads)

    with open(path.join(dst, "embedded.c"), "w") as target_c:
        print("Compiling animation data...")

        animations_json = get_file_bytes(path.join(src, "animations.json"))

        print("Compiling window icon...")
        window_icon = get_file_bytes(path.join(src, "window_icon.png"))

        print("Compiling movie video...")
        movie_video = get_file_bytes(path.join(src, "movie_video.mp4"))

        target_c.write(
            create_source(
                texture_inits_str,
                sound_inits_str,
                format_bytes_to_str(animations_json),
                format_bytes_to_str(window_icon),
                format_bytes_to_str(movie_video),
                texture_loads_str,
                sound_loads_str,
            )
        )

    with open(path.join(dst, "embedded.h"), "w") as target_h:
        target_h.write(create_header())


if __name__ == "__main__":
    compile_resources("../res/", "../src/")

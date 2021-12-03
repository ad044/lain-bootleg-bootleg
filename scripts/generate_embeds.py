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


class Resource():
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
    """.format(name=resource.name, byte_str=0 if DEBUG else resource.byte_str)


def create_load_call(resource):
    return """
    buffer[{index}] = (EmbeddedResource){{.bytes = {name}, .size = {name}_size}};
    """.format(index=resource.index, name=resource.name)


def create_source(inits, animations_json_bytes, loads):
    return """
    #include "embedded.h"
    #include "texture.h"

    {}

    const char animations_json[] = {{{}}};
    const size_t animations_json_size = sizeof(animations_json);

    void load_textures(EmbeddedResource *buffer)
    {{{}}}
    """.format(inits, animations_json_bytes, loads)


def create_header():
    return """
    #pragma once

    #include "texture.h"

    #include <stddef.h>

    typedef struct {
        unsigned char *bytes;
        size_t size;
    } EmbeddedResource;

    extern const char animations_json[];
    extern const size_t animations_json_size;

    void load_textures(EmbeddedResource *buffer);
    """


def compile_resources(src, dst):
    sprites_dir = path.join(src, "sprites")
    if not path.isdir(dst):
        print("Destination must be a directory.")
        return

    resources = []
    for f in os.listdir(sprites_dir):
        # X?D
        if f == "desktop.ini":
            continue

        print("Compiling {}...".format(f))

        filename, ext = path.splitext(f)[0], path.splitext(f)[1][1:]

        name = f"res_{filename.lower()}_{ext}"

        file_bytes = get_file_bytes(path.join(sprites_dir, f))
        byte_str = format_bytes_to_str(file_bytes)

        rsrc = Resource(filename, name, byte_str)

        resources.append(rsrc)

    resource_inits = []
    resource_loads = []

    for rsrc in resources:
        resource_inits.append(init_resource(rsrc))
        resource_loads.append(create_load_call(rsrc))

    inits_str = "".join("{}".format(i) for i in resource_inits)
    loads_str = "".join("{}".format(i) for i in resource_loads)

    with open(path.join(dst, "embedded.c"), "w") as target_c:
        animations_json_bytes = get_file_bytes(
            path.join(src, "animations.json"))

        target_c.write(create_source(
            inits_str, format_bytes_to_str(animations_json_bytes), loads_str))

    with open(path.join(dst, "embedded.h"), "w") as target_h:
        target_h.write(create_header())


if __name__ == "__main__":
    compile_resources("../res/", "../src/")

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

def create_resource(name, byte_str):
    return """
    const unsigned char {name}[] = {{{byte_str}}};
    const size_t {name}_size = sizeof({name});
    """.format(name = name, byte_str = byte_str)

def create_header(enums):
    return """
    #pragma once

    #include <stddef.h>

    #include "texture.h"


    enum {{{enums}}};

    void load_textures(void (*loader)(Texture *texture, const unsigned char *bytes,
                                      size_t length),
                       Texture *textures);
    """.format(enums = enums)

def create_texture_load_function(name, enum):
    return """
    loader(&textures[{enum}], {name}, {name}_size);
    """.format(name = name, enum = enum)

def create_source(resources, init_calls):
    return """
    #include "embedded.h"

    {resources}

    void load_textures(void (*loader)(Texture *texture, const unsigned char *bytes,
				  size_t length),
                       Texture *textures)
    {{{init_calls}}}
    """.format(resources = resources, init_calls = init_calls)

def compile_resources(src, dst):
    if not path.isdir(dst):
        print("Destination must be a directory.")
        return

    enums = []
    resources = []
    texture_inits = []

    for f in os.listdir(src):
        # X?D
        if f == "desktop.ini":
            continue
        if f[0].isdigit():
            print("Filename starts with a digit. Skipping.")
            continue

        print("Compiling {}...".format(f))

        filename, ext = path.splitext(f)[0], path.splitext(f)[1][1:]
        enum = "{}".format(filename).upper()
        symbol = "{}_{}".format(filename, ext)

        file_bytes = get_file_bytes(path.join(src, f))
        byte_str = format_bytes_to_str(file_bytes)

        enums.append(enum)
        resources.append(create_resource(symbol, byte_str))
        texture_inits.append(create_texture_load_function(symbol, enum))

    resources_str = "".join("{}".format(i) for i in resources)
    texture_inits_str = "".join("{}".format(i) for i in texture_inits)
    enums_str = "".join("{},".format(i) for i in enums)

    with open(path.join(dst, "embedded.c"), "w") as target_c:
        target_c.write(create_source(resources_str, texture_inits_str))

    with open(path.join(dst, "embedded.h"), "w") as target_h:
        target_h.write(create_header(enums_str))

if __name__ == "__main__":
    compile_resources("../assets/", "../src/")



    

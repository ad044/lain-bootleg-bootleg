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
    const unsigned char {name}[] = {{{bytes}}};
    const size_t {name}_size = sizeof({name});
    """.format(name = name, bytes = byte_str)

def create_extern(name):
    return """
    extern const unsigned char {name}[];
    extern const size_t {name}_size;
    """.format(name = name)

def create_header(externs):
    return """
    #include <stddef.h>

    {externs}
    """.format(externs = externs)

def create_source(resources):
    return """
    #include "embedded.h"

    {resources}
    """.format(resources = resources)

def compile_resources(src, dst):
    if not path.isdir(dst):
        print("Destination must be a directory.")
        return

    resources = []
    externs = []

    for f in os.listdir(src):
        # X?D
        if f == "desktop.ini":
            continue

        filename, ext = path.splitext(f)[0], path.splitext(f)[1][1:]
        symbol = "{}_{}".format(filename, ext)

        file_bytes = get_file_bytes(path.join(src, f))
        byte_str = format_bytes_to_str(file_bytes)

        resources.append(create_resource(symbol, byte_str))
        externs.append(create_extern(symbol))

    resources_str = "".join("{}".format(i) for i in resources)
    externs_str = "".join("{}".format(i) for i in externs)

    with open(path.join(dst, "embedded.c"), "w") as target_c:
        target_c.write(create_source(resources_str))

    with open(path.join(dst, "embedded.h"), "w") as target_h:
        target_h.write(create_header(externs_str))

if __name__ == "__main__":
    compile_resources("../assets/", "../src/")



    

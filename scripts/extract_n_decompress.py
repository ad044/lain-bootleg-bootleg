#!/usr/bin/env python3

import pefile
import os
import subprocess
from PIL import Image
import os, sys
import pathlib
import json
import io

EXE_FILE = "./binaries/lain_win.exe"
DAT_FILE = "./binaries/lain_win.dat"

SPRITES_TMP_DIR = "./extracted/sprites/tmp/"
SPRITES_DECOMPRESSED_DIR = "./extracted/sprites/decompressed/"
WAVES_EXTRACTED_DIR = "./extracted/sounds/"


def bytes_to_str(bytes_to_convert):
    return "".join(map(chr, bytes_to_convert))


def read_bytes(file, n, signed=False):
    return int.from_bytes(file.read(n), "little", signed=signed)


def read_lain_dat(file):
    res = {}
    res["lain_dress"] = read_bytes(file, 1)
    res["selected_theatre"] = read_bytes(file, 1)
    res["progress_flags"] = bin(read_bytes(file, 2))[2:]
    res["score"] = read_bytes(file, 4)
    res["total_bears_popped"] = read_bytes(file, 4)
    res["equipment_flags"] = bin(read_bytes(file, 4))[2:]

    collision_rect_count = read_bytes(file, 2)
    res["rectangles"] = {}
    for i in range(collision_rect_count):
        id = str(read_bytes(file, 4))
        rect = {}
        rect["left"] = read_bytes(file, 4)
        rect["top"] = read_bytes(file, 4)
        rect["right"] = read_bytes(file, 4)
        rect["bottom"] = read_bytes(file, 4)
        res["rectangles"][id] = rect

    animation_count = read_bytes(file, 2)
    frame_count = read_bytes(file, 4)
    res["animations"] = {}
    for i in range(animation_count):
        id = str(read_bytes(file, 2))
        anim = {}
        anim["frame_count"] = read_bytes(file, 2)
        anim["first_frame"] = read_bytes(file, 4)
        res["animations"][id] = anim

    frames = {}
    for i in range(frame_count):
        frames[i] = {}
        frames[i]["timing"] = read_bytes(file, 2)
        read_bytes(file, 1)
        frames[i]["layer"] = read_bytes(file, 1)
        frames[i]["res_id"] = read_bytes(file, 2)
        frames[i]["rel_x"] = read_bytes(file, 2, True)
        frames[i]["rel_y"] = read_bytes(file, 2, True)
        frames[i]["half_width"] = read_bytes(file, 1)
        frames[i]["half_height"] = read_bytes(file, 1)

    for anim in res["animations"].values():
        anim["frames"] = []
        for i in range(anim["first_frame"], anim["first_frame"] + anim["frame_count"]):
            anim["frames"].append(frames[i])
        del anim["first_frame"]
        del anim["frame_count"]

    return res


# taken from https://gist.github.com/jjo-sec/0ead6f9a9f91e420f7c8
def extract_icon(pe):
    rt_string_idx = [entry.id for entry in pe.DIRECTORY_ENTRY_RESOURCE.entries].index(
        pefile.RESOURCE_TYPE["RT_GROUP_ICON"]
    )
    rt_string_directory = pe.DIRECTORY_ENTRY_RESOURCE.entries[rt_string_idx]
    rt_string_directory = [
        e
        for e in pe.DIRECTORY_ENTRY_RESOURCE.entries
        if e.id == pefile.RESOURCE_TYPE["RT_GROUP_ICON"]
    ][0]
    entry = rt_string_directory.directory.entries[-1]  # gives the highest res icon
    offset = entry.directory.entries[0].data.struct.OffsetToData
    size = entry.directory.entries[0].data.struct.Size
    data = pe.get_memory_mapped_image()[offset : offset + size]
    icon = data[:18] + "\x16\x00\x00\x00".encode()

    rt_string_idx = [entry.id for entry in pe.DIRECTORY_ENTRY_RESOURCE.entries].index(
        pefile.RESOURCE_TYPE["RT_ICON"]
    )
    rt_string_directory = pe.DIRECTORY_ENTRY_RESOURCE.entries[rt_string_idx]
    rt_string_directory = [
        e
        for e in pe.DIRECTORY_ENTRY_RESOURCE.entries
        if e.id == pefile.RESOURCE_TYPE["RT_ICON"]
    ][0]
    entry = rt_string_directory.directory.entries[-1]  # gives the highest res icon
    offset = entry.directory.entries[0].data.struct.OffsetToData
    size = entry.directory.entries[0].data.struct.Size
    icon += pe.get_memory_mapped_image()[offset : offset + size]
    return icon


def main():
    pathlib.Path(SPRITES_TMP_DIR).mkdir(parents=True, exist_ok=True)
    pathlib.Path(SPRITES_DECOMPRESSED_DIR).mkdir(parents=True, exist_ok=True)
    pathlib.Path(WAVES_EXTRACTED_DIR).mkdir(parents=True, exist_ok=True)

    if not os.path.exists(os.path.join(os.getcwd(), "lzss")):
        subprocess.run([os.environ.get("CC", "cc"), "lzss.c", "-o", "lzss"])

    with open(EXE_FILE, "rb") as f:
        pe = pefile.PE(EXE_FILE)

        rcdata_entries = pe.DIRECTORY_ENTRY_RESOURCE.entries[3]
        wave_entries = pe.DIRECTORY_ENTRY_RESOURCE.entries[0]

        print("Extracting icon...")
        icon_image = Image.open(io.BytesIO(extract_icon(pe)))
        icon_image.save(os.path.join(os.getcwd(), "extracted", "window_icon.png"), "png")


        for i, entry in enumerate(rcdata_entries.directory.entries):
            rva = entry.directory.entries[0].data.struct.OffsetToData
            size = entry.directory.entries[0].data.struct.Size
            data = pe.get_memory_mapped_image()[rva : rva + size]

            tempname = "tmp-{}".format(i)
            tempfile_path = os.path.join(os.getcwd(), SPRITES_TMP_DIR, tempname)
            decompressed_path = os.path.join(
                os.getcwd(), SPRITES_DECOMPRESSED_DIR, str(i)
            )

            print("Extracting sprite {}...".format(i))

            if not os.path.exists(os.path.join(os.getcwd(), tempfile_path)):
                with open(tempfile_path, "wb") as target:
                    target.write(data)

            if not os.path.exists(os.path.join(os.getcwd(), decompressed_path)):
                print("Decompressing sprite {}...".format(i))
                subprocess.run(["./lzss", tempfile_path, decompressed_path])

        for i, entry in enumerate(wave_entries.directory.entries):
            rva = entry.directory.entries[0].data.struct.OffsetToData
            size = entry.directory.entries[0].data.struct.Size
            data = pe.get_memory_mapped_image()[rva : rva + size]

            print("Extracting {}.wav...".format(i))
            outfile = os.path.join(os.getcwd(), WAVES_EXTRACTED_DIR, "{}.wav".format(i))

            with open(outfile, "wb") as target:
                target.write(data)

    with open(DAT_FILE, "rb") as f:
        data = read_lain_dat(f)
        outfile = os.path.join(os.getcwd(), "extracted", "lain_win.json")

        with open(outfile, "w") as target:
            target.write(json.dumps(data, indent=2))


if __name__ == "__main__":
    main()

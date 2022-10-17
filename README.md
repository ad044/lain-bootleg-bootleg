# lain-bootleg-bootleg

This project aims to reverse-engineer and remake Lain Bootleg, a Serial Experiments Lain minigame.

# Extracting assets

To run the Python scripts you are going to need [`opencv-python`](https://pypi.org/project/opencv-python/) and [`pefile`](https://pypi.org/project/pefile/).

The repository doesn't contain the game's assets, the extraction is done by automation scripts
located under the `scripts/` directory.

The user will have to provide the game binaries themselves in the following format:

Create directory `scripts/binaries/`, under it, place these items from the original game:
`lain_win.dat`, `lain_mov.dat`, `lain_win.exe`.

Afterwards, run the script `make_game_assets.py`

Once you're done running the script, a directory called `res/` should appear in
the root of the repo you just cloned. If that's the case, you can proceed to platform-specific
instructions for building.

If your goal is not to build the assets for the game but for your own use, you can also
use `extract_n_decompress.py` directly, which will yield raw decompressed assets along
with their mask bitmaps inside a directory called `extracted` 
(located under the same location as the script).

# Building

Before you build, you must have extracted the assets in the proper location by following 
the guide above.

Make sure to run `git submodule update --init --recursive` before proceeding to
pull external dependencies if you didn't do it along with the cloning step.

## Dependencies

- **gcc**
- **make**
- **cmake**
- **mpv** along with it's development headers

GLFW and GLEW will be compiled along with the program (submodules) and statically linked. 
If you would prefer to use your system-installed versions of these, use the CMake options 
`SYSTEM_GLFW` and `SYSTEM_GLEW`. For example:

```sh
cmake -DSYSTEM_GLFW=ON -DSYSTEM_GLEW=ON ..
```

## Compiling on Linux

1. `cd` into the repo
2. `mkdir build && cd build`
3. `cmake ..`
4. `make`

This should produce a binary called `lain-bootleg-bootleg`.

## Compiling on Windows using MinGW and MSYS2

For Windows we will need to download `libmpv` DLLs manually. The usual place to grab them would be
[shinchiro's builds](https://sourceforge.net/projects/mpv-player-windows/files/libmpv/).
Download the archive for your target architecture and extract it under `external/mpv`.
Afterwards, create another directory named `mpv` inside the `include/` directory and move all the contents
to it so that the headers of `external/mpv/include` end up in `external/mpv/include/mpv`.

So something like:

1. `cd` into the repo
2. `cd external/mpv/include`
3. `mkdir mpv`
4. `mv *.h mpv`

As for building:

1. `cd` into the repo
2. `mkdir build && cd build`
3. `cmake -G "MSYS Makefiles" ..`
4. `make`

This should produce an executable called `lain-bootleg-bootleg.exe`.

# Editing the save file

Upon closing the game for the first time, you will notice a new file in the same directory
as the executable called `lain_save.json`, which will have a format similar to:

```
{
        "score":        0,
        "theater_preview":      478,
        "tool_state":   0,
        "outfit":       0,
        "school_outfit_unlocked":       0,
        "cyberia_outfit_unlocked":      0,
        "sweater_outfit_unlocked":      0,
        "bear_outfit_unlocked": 0,
        "alien_outfit_unlocked":        0,
        "screwdriver_unlocked": 0,
        "navi_unlocked":        0
}
```

Notes - all the ranges mentioned below are inclusive, and the values are layed out in the 
proper order.

`score` is self explanatory.

`theater_preview` can range from 478 to 484 - `classroom`, `school`, `lain's room`, `arisu's room`, 
`cyberia`, `street` and `bridge`.

`tool_state` ranges from 0 to 2 - `no tools`, `holding screwdriver`, `holding navi`.

`outfit` goes from 0 to 5 - `default`, `school`, `cyberia`, `bear`, `sweater`, `alien`.

# Reporting bugs/issues

If you found any bugs, have questions, etc. feel free to open an issue or join the 
[Discord server](https://discord.com/invite/JGnEyhD6ah).

# Libraries used

- [`GLFW`](https://github.com/glfw/glfw)
- [`glew`](https://github.com/nigels-com/glew)
- [`libmpv`](https://github.com/mpv-player/mpv)
- [`cJSON`](https://github.com/DaveGamble/cJSON)
- [`cvector`](https://github.com/eteran/c-vector)
- [`miniaudio`](https://github.com/mackron/miniaudio)
- [`stb_image`](https://github.com/nothings/stb)

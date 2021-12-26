filename = "./lain_win.exe"

def bytes_to_str(bytes_to_convert):
    return "".join(map(chr, bytes_to_convert))
    
with open(filename, "rb") as f:
    found_wav_count = 0
    while True:
        four_bytes = bytes_to_str(f.read(4))
        if four_bytes != "RIFF":
            continue

        size = int.from_bytes(f.read(4), "little")

        f.seek(-8, 1)

        with open("{}.wav".format(found_wav_count), "wb") as target:
            print("Extracting {}.wav".format(found_wav_count))
            target.write(f.read(size))
            found_wav_count += 1
            # round it off at the next byte thats multiple of four to resume reading
            f.read(4 - size % 4)

            if found_wav_count == 11:
                break



/* LZSS decompression code from lain bootleg, modified output from ghidra so
 * some variables don't have meaningful names, but thanks to zerno, functions and most variables
 * do have proper names. */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *LZSS_compressed_data;
unsigned char current_compressed_byte;
int LZSS_bit_counter;
char *LZSS_Output_Memory;
int DAT_00412fe0;
char BIT_MASKS[8] = {0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1};
unsigned int DAT_00412fc8;
unsigned int DAT_00412fcc;
unsigned int DAT_00412fd0;
unsigned int circular_buffer_current;
unsigned int DAT_00412fd4;
unsigned int DAT_00412fdc;
char *LZSS_circular_buffer;

void set_lzss_src(char *compressed_data)

{
  LZSS_compressed_data = compressed_data;
  current_compressed_byte = 0;
  LZSS_bit_counter = 0;
  return;
}

void set_lzss_dst(char *output_memory)

{
  LZSS_Output_Memory = output_memory;
  DAT_00412fe0 = 0;
  return;
}

bool get_next_bit(void)

{
  int uVar1;

  if (LZSS_bit_counter == 0) {
    current_compressed_byte = *LZSS_compressed_data;
    LZSS_compressed_data = LZSS_compressed_data + 1;
  }
  uVar1 = LZSS_bit_counter;
  LZSS_bit_counter += 1;
  if (LZSS_bit_counter == 8) {
    LZSS_bit_counter = 0;
  }
  return (BIT_MASKS[uVar1] & current_compressed_byte) != 0;
}

unsigned int read_bits(int num_bits)

{
  bool bVar1;
  unsigned int output_bits;

  output_bits = 0;
  if (num_bits != 0) {
    do {
      output_bits <<= 1;
      bVar1 = get_next_bit();
      if (bVar1 != 0) {
        output_bits |= 1;
      }
      num_bits += -1;
    } while (num_bits != 0);
  }
  return output_bits;
}

char *get_circular_buffer_element_pointer(unsigned int param_1)

{
  return (char *)((DAT_00412fd0 - 1 & param_1) + LZSS_circular_buffer);
}

void write_n_of_byte(unsigned int param_1, unsigned char param_2)

{
  /* printf("writing %u of %02x\n", param_1, param_2); */
  if (param_1 != 0) {
    do {
      *LZSS_Output_Memory = param_2;
      LZSS_Output_Memory = LZSS_Output_Memory + 1;
      param_1 += -1;
    } while (param_1 != 0);
  }
  return;
}

void write_output(unsigned int param_1)

{
    /* printf("write_output %u\n", param_1); */
  if (DAT_00412fe0 == 0) {
    DAT_00412fdc = param_1;
    DAT_00412fe0 = 1;
    return;
  }
  if (DAT_00412fe0 != 1) {
    if (DAT_00412fe0 == 2) {
      DAT_00412fe0 = param_1;
      if (param_1 == 0) {
        DAT_00412fe0 = 0x100;
      }
      param_1 = DAT_00412fdc;
      if (3 < DAT_00412fe0) {
        return;
      }
    }
    write_n_of_byte(DAT_00412fe0, (char)param_1);
    DAT_00412fe0 = 1;
    return;
  }
  if (param_1 != DAT_00412fdc) {
    write_n_of_byte(1, (char)param_1);
    return;
  }
  DAT_00412fe0 = 2;
  return;
}

int read_displacement(void)

{
  bool bVar1;
  unsigned int uVar2;
  int iVar3;

  /* printf("DAT_00412fc8 = %i", DAT_00412fc8); */
  uVar2 = read_bits(DAT_00412fc8 - 1);
  if (DAT_00412fd4 - 1 <= (int)uVar2) {
    iVar3 = uVar2 * 2;
    bVar1 = get_next_bit();
    if (bVar1 != 0) {
      iVar3 += 1;
    }
    if (iVar3 == DAT_00412fd0 - 1) {
      return -1;
    }
    uVar2 = iVar3 + (1 - DAT_00412fd4);
  }
  return uVar2;
}

int read_length(void)

{
  bool bVar1;
  unsigned int uVar2;
  int iVar3;

  uVar2 = read_bits(DAT_00412fcc - 1);
  if (uVar2 == 1) {
    return 2;
  }
  iVar3 = uVar2 * 2;
  bVar1 = get_next_bit();
  if (bVar1 != 0) {
    iVar3 += 1;
  }
  if (iVar3 == 1) {
    iVar3 = 3;
  }
  if (iVar3 == 0) {
    iVar3 = DAT_00412fd4;
  }
  return iVar3;
}

void copy_from_buffer(int length, int displacement)

{
  unsigned char bVar1;
  unsigned int uVar2;
  char *pbVar3;

  if (length != 0) {
    do {
      pbVar3 = get_circular_buffer_element_pointer(displacement + circular_buffer_current);
      uVar2 = circular_buffer_current;
      bVar1 = *pbVar3;
      circular_buffer_current += 1;
      pbVar3 = get_circular_buffer_element_pointer(uVar2);
      *pbVar3 = bVar1;
      /* printf("copy_from_buffer bVar1 = %02x\n", bVar1); */
      write_output(bVar1);
      length -= 1;
    } while (length != 0);
  }
  return;
}

void DecompressLZSS(char *compressed_data, char *output_memory,
                    void *circular_buffer)

{
  unsigned int uVar1;
  bool flag;
  unsigned int uVar3;
  char *puVar4;
  unsigned int disp;
  unsigned int displacement;
  unsigned int length;

  set_lzss_src(compressed_data + 8);
  set_lzss_dst(output_memory);
  // usually b01011 (11)
  DAT_00412fc8 = read_bits(5);
  printf("DAT_00412fc8 = %i\n", DAT_00412fc8);
  // usually b0100 (4)
  DAT_00412fcc = read_bits(4);
  printf("DAT_00412fcc = %i\n", DAT_00412fcc);
  // number of possible values for a 11 bit integer
  DAT_00412fd0 = 1 << (DAT_00412fc8 & 0x1f);
  printf("DAT_00412fd0 = %i\n", DAT_00412fd0);
  LZSS_circular_buffer = circular_buffer;
  circular_buffer_current = 0;
  // number of possible values for a 4 bit integer
  DAT_00412fd4 = 1 << (DAT_00412fcc & 0x1f);
  printf("DAT_00412fd4 = %i\n", DAT_00412fd4);
  do {
    flag = get_next_bit();
    if (flag == 0) {
      disp = read_displacement();
      /* printf("disp = %i\n", disp); */
      if (disp == -1) {
        return;
      }
      displacement = (DAT_00412fd0 - disp) + -1;
      /* printf("displacement = %i\n", displacement); */
      length = read_length();
      /* printf("length = %i\n", length); */
      copy_from_buffer(length, displacement);
    } else {
      uVar3 = read_bits(8);
      /* printf("main uVar3 = %i\n", uVar3); */
      uVar1 = circular_buffer_current;
      circular_buffer_current += 1;
      puVar4 = get_circular_buffer_element_pointer(uVar1);
      *puVar4 = (char)uVar3;
      write_output(uVar3);
    }
    circular_buffer_current &= DAT_00412fd0 - 1U;
  } while (true);
}

int load_file_to_memory(const char *filename, char **result) {
  int size = 0;
  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    *result = NULL;
    return -1; // -1 means file opening fail
  }
  fseek(f, 0, SEEK_END);
  size = ftell(f);
  fseek(f, 0, SEEK_SET);
  *result = (char *)malloc(size + 1);
  if (size != fread(*result, sizeof(char), size, f)) {
    free(*result);
    return -2; // -2 means file reading fail
  }
  fclose(f);
  (*result)[size] = 0;
  return size;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Too many or too few arguments\nUsage: lzss in_filename "
           "out_filename\n");
    return 1;
  }

  char *compressed_data;
  char *uncompressed_data;
  int file_size;
  uint32_t uncompressed_size;

  file_size = load_file_to_memory(argv[1], &compressed_data);
  if (file_size < 0) {
    printf("Error loading input file\n");
    return 1;
  }

  if (memcmp(compressed_data, "LZSS", 4) != 0) {
    printf("Not a valid LZSS file");
    return 1;
  }

  uncompressed_size = *(uint32_t *)&compressed_data[4];
  printf("uncompressed_size: %d\n", uncompressed_size);
  uncompressed_data = (char *)malloc(uncompressed_size);
  void *circular_buffer = malloc(65536);

  DecompressLZSS(compressed_data, uncompressed_data, circular_buffer);
  FILE *f = fopen(argv[2], "wb");
  if (f == NULL) {
    printf("failed to open output file\n");
    return 1;
  }
  fwrite(uncompressed_data, uncompressed_size, 1, f);
  fclose(f);
  return 0;
}

#include "tfslib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Reads the data on a block and returns it
static unsigned char *tfs_read_block(struct FileSystem *fs,
                                     uint16_t dir_table_index,
                                     uint16_t dir_table_head) {
  uint16_t data_index = dir_table_index * fs->superblock.block_size;
  uint8_t is_EOF = fs->FAT[dir_table_index] == DATA_EOF;
  unsigned char *data = NULL;
  size_t data_size;

  if (is_EOF)
    data_size = fs->dir_table[dir_table_head].size % fs->superblock.block_size;
  else
    data_size = fs->superblock.block_size;

  data = malloc(sizeof(*data) * data_size);
  if (!data)
    return NULL;

  memcpy(data, fs->data + data_index, data_size);
  return data;
}

// data_rel_index is index relative to the data stream
unsigned char *tfs_read_data(struct FileSystem *fs, uint16_t dir_table_index) {
  // The data memory is handled as a dynamic array in order to store any
  // arbitrary amount of data
  size_t capacity = 2;
  unsigned char *data =
      malloc(sizeof(*data) * capacity * fs->superblock.block_size);

  if (!data)
    return NULL;

  uint16_t curr_index = dir_table_index;
  size_t count = 0;

  do {
    if (capacity <= count) {
      capacity *= 2; // Doubles the data size each iteration
      unsigned char *tmp =
          realloc(data, sizeof(*data) * capacity * fs->superblock.block_size);

      if (!tmp) {
        free(data);
        return NULL;
      }

      data = tmp;
    }

    unsigned char *curr_data = tfs_read_block(fs, curr_index, dir_table_index);

    if (!curr_data) {
      free(data);
      return NULL;
    }

    size_t size;

    // If the current memory block is EOF then the size of the data is truncated
    // to the right size as to not overflow
    if (fs->FAT[curr_index] == DATA_EOF)
      size = fs->dir_table[dir_table_index].size % fs->superblock.block_size;
    else
      size = fs->superblock.block_size;

    memcpy(data + fs->superblock.block_size * count, curr_data, size);

    free(curr_data);
    ++count;
    if (fs->FAT[curr_index] == DATA_EOF)
      break;

    curr_index = fs->FAT[curr_index];
  } while (1);

  return data;
}

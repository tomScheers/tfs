#include "tfslib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Reads the data on a block and returns it
unsigned char *tfs_read_block(struct FileSystem *fs, uint16_t dir_table_index,
                              uint16_t dir_table_head) {
  printf("Index; %u\n", dir_table_index);
  printf("Data: %s\n", fs->data + fs->superblock.block_size * dir_table_index);

  uint16_t data_index = dir_table_index * fs->superblock.block_size;

  uint8_t is_EOF = fs->FAT[dir_table_index] == 0xFF;

  unsigned char *data = NULL;

  size_t data_size;
  if (is_EOF) {
    printf("IS EOF!!\n");
    data_size = fs->dir_table[dir_table_head].size % fs->superblock.block_size;
    printf("data size: %zu\n", data_size);
  } else {
    data_size = fs->superblock.block_size;
    printf("Data size: %zu\n", data_size);
  }

  data = malloc(sizeof(*data) * data_size);
  printf("Dataptr: %p\n", data);
  memcpy(data, fs->data + data_index, data_size);
  for (size_t i = 0; i < data_size; ++i)
    printf("%02X ", data[i]);
  putchar('\n');

  return data;
}

// data_rel_index is index relative to the data stream
unsigned char *tfs_read_data(struct FileSystem *fs, uint16_t dir_table_index) {
  size_t size = 0;
  size_t capacity = 2;
  printf("Allocating\n");
  unsigned char *data =
      malloc(sizeof(*data) * capacity * fs->superblock.block_size);
  printf("Allocated\n");

  uint16_t curr_index = dir_table_index;

  size_t count = 0;
  do {
    if (capacity <= size) {
      capacity *= 2;
      data =
          realloc(data, sizeof(*data) * capacity * fs->superblock.block_size);
    }
    unsigned char *curr_data = tfs_read_block(fs, curr_index, dir_table_index);
    printf("Curr index: %hu\n", curr_index);
    if (fs->FAT[curr_index] == 0xFF) {
      printf("IS ALSO EOF\n");
      memcpy(data + fs->superblock.block_size * count, curr_data,
             fs->dir_table[dir_table_index].size % fs->superblock.block_size);
    }
    else {
      memcpy(data + fs->superblock.block_size * count, curr_data,
             fs->superblock.block_size);
    }
    free(curr_data);

    ++count;
    ++size;

    if (fs->FAT[curr_index] == 0xFF)
      break;

    curr_index = fs->FAT[curr_index];
  } while (1);

  return data;
}

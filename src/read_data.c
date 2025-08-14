#include "tfslib.h"
#include <stdio.h>

// Reads the data on a block and returns it
unsigned char* tfs_read_data(struct FileSystem *fs, uint16_t index) {
  uint16_t data_index = fs->dir_table[index].starting_block;
  printf("Starting block: %hu\n", data_index);
  return fs->data[data_index];
}

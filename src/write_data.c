#include "tfslib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>

#define IS_FREE(fat_entry) (fat_entry) != 0x00

// Writes data to file block
int32_t tfs_write_data(struct FileSystem *fs, char file_path[],
                       unsigned char *bytes, size_t size) {

  if ((uint16_t)((double)size / (double)BLOCK_SIZE) >
      fs->superblock->free_blocks)
    return ERR_INSUFFICIENT_MEMORY;

  uint8_t is_EOF = 0;
  int32_t prev_block_index = -1;
  int32_t block_index = 0;
  size_t first_block_index = 0;
  size_t count = 0;

  // Loops over entire data array until it reaches the EOF
  while (!is_EOF) {
    block_index = 0;
    // Increment the block_index until it reaches a non-restricted free block
    while (IS_FREE(fs->FAT[block_index]) || block_index == prev_block_index) {
      ++block_index;
    }
    printf("Block Index; %d\n", block_index);

    // If count is 0 then it'll set first_block_index to, well, the first block
    // index
    if (count > 0)
      fs->FAT[prev_block_index] = block_index;
    else
      first_block_index = block_index;

    // Using this abomination of a formula to check whether or not the function
    // is EOF
    is_EOF = (signed)size - (signed)fs->superblock->block_size * (signed)count <
             (signed)fs->superblock->block_size;

    unsigned char *data_dst =
        fs->data +
        block_index *
            fs->superblock
                ->block_size; // Get the destination for the data, AKA the
                              // pointer to the right location on the data field
    unsigned char *data_src =
        bytes +
        count * fs->superblock
                    ->block_size; // Get the src for the data, AKA the poitner
                                  // to the right location in the bytes arg
    size_t data_size;

    if (is_EOF)
      data_size = size % fs->superblock->block_size;
    else
      data_size = fs->superblock->block_size;

    memcpy(data_dst, data_src, data_size);

    ++count;
    prev_block_index = block_index;
  }

  uint16_t dir_table_index =
      fs->superblock->file_max - fs->superblock->free_blocks;

  strncat(fs->dir_table[dir_table_index].name, file_path, strlen(file_path));
  fs->dir_table[dir_table_index].name[strlen(file_path)] = '\0';

  time_t current_time = time(NULL);

  // Might handle gracefully in the future
  if (current_time == (time_t)-1) {
    return ERR_TIME_RETRIEVAL;
  }

  fs->dir_table[dir_table_index].is_dir = 0;
  fs->dir_table[dir_table_index].created = current_time;
  fs->dir_table[dir_table_index].last_modified = current_time;
  fs->dir_table[dir_table_index].starting_block = first_block_index;
  fs->dir_table[dir_table_index].size = size;

  fs->FAT[block_index] = DATA_EOF; // Set last block to EOF

  --fs->superblock->free_blocks;
  return dir_table_index;
}

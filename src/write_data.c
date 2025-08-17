#include "tfslib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Writes data to file block
int32_t tfs_write_data(struct FileSystem *fs, char file_path[],
                       unsigned char *bytes, size_t size) {
  if ((uint16_t)((double)size / (double)BLOCK_SIZE) >
      fs->superblock.free_blocks)
    return -1;

  uint16_t dir_table_start = fs->superblock.dir_table_start;
  uint16_t dir_table_end =
      fs->superblock.dir_table_start + fs->superblock.dir_table_size;
  uint16_t FAT_start = fs->superblock.FAT_start;
  uint16_t FAT_end = fs->superblock.FAT_start + fs->superblock.FAT_size;

  uint8_t is_EOF = 0;
  size_t prev_block_index = 0;
  size_t block_index = 0;
  size_t first_block_index = 0;
  size_t count = 0;
  while (!is_EOF) {
    block_index = 1;
    while (fs->FAT[block_index] != 0x00 ||
           (block_index >= dir_table_start && block_index < dir_table_end) ||
           (block_index >= FAT_start && block_index < FAT_end) ||
           block_index == prev_block_index) {
      printf("FAT_LOOP: %d\n", fs->FAT[block_index]);
      ++block_index;
    }
    printf("Block Index; %zu\n", block_index);


    if (count > 0)
      fs->FAT[prev_block_index] = block_index;
    else
      first_block_index = block_index;

    is_EOF = (signed)size - (signed)fs->superblock.block_size * (signed)count <
             (signed)fs->superblock.block_size;

    if (is_EOF)
      memcpy(fs->data + block_index * fs->superblock.block_size, bytes + count * fs->superblock.block_size, size % fs->superblock.block_size);
    else
      memcpy(fs->data + block_index * fs->superblock.block_size, bytes + count * fs->superblock.block_size, fs->superblock.block_size);

    ++count;
    prev_block_index = block_index;
  }

  fs->FAT[block_index] = 0xFF; // Set last block to EOF

  uint16_t dir_table_index =
      fs->superblock.total_blocks - fs->superblock.free_blocks;

  fs->dir_table[dir_table_index].is_dir = 0;

  fs->dir_table[dir_table_index].starting_block = dir_table_start;

  strncat(fs->dir_table[dir_table_index].name, file_path, strlen(file_path));
  fs->dir_table[dir_table_index].name[strlen(file_path)] = '\0';

  time_t current_time = time(NULL);

  if (current_time == (time_t)0) {
    fprintf(stderr, "Failed to retrieve time\n");
    return -1;
  }

  fs->dir_table[dir_table_index].created = current_time;
  fs->dir_table[dir_table_index].last_modified = current_time;
  fs->dir_table[dir_table_index].starting_block = first_block_index;

  printf("Size to be written: %zu\n", size);
  fs->dir_table[dir_table_index].size = size;

  --fs->superblock.free_blocks;

  return dir_table_index;
}

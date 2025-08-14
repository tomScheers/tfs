#include "tfslib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Writes data to file block
int32_t tfs_write_data(struct FileSystem *fs, char file_path[],
                      unsigned char *bytes, size_t size) {
  if ((uint16_t)((double)size / (double)BLOCK_SIZE) > fs->superblock.free_blocks)
    return -1;

  size_t block_index = 1;

  uint16_t dir_table_start = fs->superblock.dir_table_start;
  uint16_t dir_table_end =
      fs->superblock.dir_table_start + fs->superblock.dir_table_size;
  uint16_t FAT_start = fs->superblock.FAT_start;
  uint16_t FAT_end = fs->superblock.FAT_start + fs->superblock.FAT_size;

  while (fs->FAT[block_index] != 0x00 || (block_index >= dir_table_start &&
         block_index <= dir_table_end) || (block_index >= FAT_start && 
         block_index <= FAT_end))
    ++block_index;
  printf("Block Index; %zu\n", block_index);

  fs->data[block_index] = bytes;

  fs->FAT[block_index] = 0xFF; // Set to EOF

  uint16_t dir_table_index =
      fs->superblock.total_blocks - fs->superblock.free_blocks;

  fs->dir_table[dir_table_index].is_dir = 0;

  fs->dir_table[dir_table_index].starting_block = dir_table_start;

  strncpy(fs->dir_table[dir_table_index].name, file_path, sizeof(fs->dir_table[dir_table_index].name) - 1);

  fs->dir_table[dir_table_index].name[sizeof(fs->dir_table[dir_table_index].name) - 1] = '\0';

  time_t current_time = time(NULL);

  if (current_time == (time_t)0) {
    fprintf(stderr, "Failed to retrieve time\n");
    return -1;
  }

  fs->dir_table[dir_table_index].created = current_time;
  fs->dir_table[dir_table_index].last_modified = current_time;
  fs->dir_table[dir_table_index].starting_block = block_index;
  
  --fs->superblock.free_blocks;

  return dir_table_index;
}

#include "tfslib.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

uint8_t tfs_mk_dir(struct FileSystem *fs, char dir_path[MAX_FILE_LEN]) {
  uint16_t curr_index = 0;
  for (; fs->FAT[curr_index] != 0x00; ++curr_index)
    ;

  time_t curr_time = time(NULL);

  if (curr_time == (time_t)-1)
    return ERR_TIME_RETRIEVAL;

  fs->dir_table[curr_index].last_modified = curr_time;
  fs->dir_table[curr_index].created = curr_time;
  strncpy(fs->dir_table[curr_index].name, dir_path, MAX_FILE_LEN - 1);
  fs->dir_table[curr_index].name[sizeof(fs->dir_table[curr_index].name) - 1] =
      '\0';
  fs->dir_table[curr_index].size = fs->superblock->block_size;
  fs->dir_table[curr_index].starting_block = curr_index;

  fs->dir_table[curr_index].FAT_index = curr_index;
  fs->FAT[curr_index] = DATA_EOF;

  ++fs->superblock->amount_of_files;
  --fs->superblock->free_blocks;
  return 0;
}

#include "tfslib.h"

#include <stdint.h>

uint8_t tfs_delete_file(struct FileSystem *fs, uint16_t index) {
  if (fs->FAT[index] == 0x00)
    return ERR_OFB_WRITE;

  uint16_t curr_data_block = fs->dir_table[index].FAT_index;
  uint16_t prev_data_block = fs->dir_table[index].FAT_index;

  do {
    prev_data_block = curr_data_block;

    if (fs->FAT[curr_data_block] == 0x00)
      return ERR_OFB_WRITE;

    if (fs->FAT[prev_data_block] == DATA_EOF) {
      fs->FAT[curr_data_block] = 0x00;
      break;
    }

    curr_data_block = fs->FAT[curr_data_block];
    fs->FAT[prev_data_block] = 0x00;
    ++fs->superblock->free_blocks;
  } while (1);

  --fs->superblock->amount_of_files;

  return 0;
}

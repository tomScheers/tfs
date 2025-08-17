#include <stdlib.h>
#include "tfslib.h"

// Initializes the file system
struct FileSystem* tfs_init() {
  struct FileSystem* fs = malloc(sizeof(*fs));
  if (!fs)
    return NULL;

  fs->FAT = calloc(FAT_SIZE, sizeof(*fs->FAT) * BLOCK_SIZE);
  if (!fs->FAT) {
    free(fs);
    return NULL;
  }

  fs->data = calloc(FREE_BLOCKS, sizeof(*fs->data) * BLOCK_SIZE);
  if (!fs->data) {
    free(fs->FAT);
    free(fs);
    return NULL;
  }

  fs->dir_table = calloc(DIR_TABLE_SIZE, sizeof(*fs->dir_table) * BLOCK_SIZE);
  if (!fs->dir_table) {
    free(fs->FAT);
    free(fs->data);
    free(fs);
    return NULL;
  }

  fs->superblock.magic = MAGIC_NUMBER;
  fs->superblock.block_size = BLOCK_SIZE;
  fs->superblock.dir_table_size = DIR_TABLE_SIZE;
  fs->superblock.dir_table_start = DIR_TABLE_START;
  fs->superblock.FAT_size = FAT_SIZE;
  fs->superblock.FAT_start = FAT_START;
  fs->superblock.free_blocks = FREE_BLOCKS;
  fs->superblock.total_blocks = TOTAL_BLOCKS;
  fs->superblock.version = VERSION;

  return fs;
}

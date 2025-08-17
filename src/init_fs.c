#include <stdio.h>
#include <stdlib.h>
#include "tfslib.h"

// Initializes the file system
struct FileSystem* tfs_init() {
  struct FileSystem* fs = malloc(sizeof(struct FileSystem));
  fs->FAT = calloc(FAT_SIZE, sizeof(unsigned char*) * BLOCK_SIZE);
  fs->data = calloc(FREE_BLOCKS, sizeof(unsigned char) * BLOCK_SIZE);
  fs->dir_table = calloc(DIR_TABLE_SIZE, sizeof(unsigned char*) * BLOCK_SIZE);

  fs->superblock.magic = MAGIC_NUMBER;
  fs->superblock.block_size = BLOCK_SIZE;
  fs->superblock.dir_table_size = DIR_TABLE_SIZE;
  fs->superblock.dir_table_start = DIR_TABLE_START;
  fs->superblock.FAT_size = FAT_SIZE;
  fs->superblock.FAT_start = FAT_START;
  printf("Free blocks: %d\n", FREE_BLOCKS);
  fs->superblock.free_blocks = FREE_BLOCKS;
  fs->superblock.total_blocks = TOTAL_BLOCKS;
  fs->superblock.version = VERSION;

  return fs;
}

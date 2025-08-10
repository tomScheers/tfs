#include <stdio.h>
#include <stdint.h>

#include "tfslib.h"

int8_t tfs_save_fs(struct FileSystem *fs, char *name) {
  FILE* fptr = fopen(name, "w");
  if (!fptr)
    return -1;
  fwrite(&fs->superblock, sizeof(fs->superblock), 1, fptr);
  fwrite(fs->dir_table, fs->superblock.dir_table_size * BLOCK_SIZE, 1, fptr);
  fwrite(fs->FAT, fs->superblock.FAT_size * BLOCK_SIZE, 1, fptr);
  fwrite(fs->data, (fs->superblock.total_blocks - fs->superblock.dir_table_size - fs->superblock.FAT_size - 1) * BLOCK_SIZE, 1, fptr);
  fclose(fptr);
  return 0;
}

#include <stdint.h>
#include <stdio.h>

#include "tfslib.h"

// Write fs to a file
int8_t tfs_save_fs(struct FileSystem *fs, char *file_name) {
  FILE *fptr = fopen(file_name, "wb");
  if (!fptr)
    return ERR_GENERAL;

  fwrite(fs->superblock, 1, SUPERBLOCK_BYTES, fptr);

  for (size_t i = 0; i < fs->superblock->file_max; ++i)
    fwrite((unsigned char *)fs->dir_table + i * sizeof(*fs->dir_table), 1,
           sizeof(*fs->dir_table), fptr);

  fwrite(fs->FAT, 1, fs->superblock->file_max, fptr);
  for (size_t i = 0; i < fs->superblock->file_max; ++i)
    fwrite(fs->data + i * fs->superblock->block_size, 1,
           fs->superblock->block_size, fptr);

  fclose(fptr);
  return 0;
}

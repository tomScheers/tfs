#include <stdio.h>
#include <stdlib.h>

#include "tfslib.h"

struct FileSystem *tfs_read_fs(char file_name[]) {
  FILE *fptr = fopen(file_name, "rb");
  if (!fptr)
    return NULL;

  struct FileSystem *fs = malloc(sizeof(*fs));

  if (!fs) {
    fclose(fptr);
    return NULL;
  }

  fs->superblock = calloc(1, SUPERBLOCK_BYTES);

  if (!fs->superblock) {
    free(fs);
    fclose(fptr);
    return NULL;
  }

  fread(fs->superblock, 1, SUPERBLOCK_BYTES, fptr);

  fs->FAT = calloc(1, fs->superblock->file_max);
  if (!fs->FAT) {
    free(fs->superblock);
    fclose(fptr);
    free(fs);
    return NULL;
  }

  fs->data = calloc(fs->superblock->file_max, fs->superblock->block_size);
  if (!fs->data) {
    free(fs->FAT);
    free(fs->superblock);
    free(fs);
    fclose(fptr);
    return NULL;
  }

  fs->dir_table = calloc(fs->superblock->file_max, sizeof(*fs->dir_table));
  if (!fs->dir_table) {
    free(fs->FAT);
    free(fs->superblock);
    free(fs->data);
    free(fs);
    fclose(fptr);
    return NULL;
  }

  size_t dir_table_entry_size = sizeof(struct DirTableEntry);

  for (size_t i = 0; i < fs->superblock->file_max; ++i) {
    size_t seek = SUPERBLOCK_BYTES + dir_table_entry_size * i;
    fseek(fptr, seek, SEEK_SET);
    fread(&fs->dir_table[i], 1, sizeof(struct DirTableEntry), fptr);
  }

  size_t FAT_start =
      SUPERBLOCK_BYTES + fs->superblock->file_max * sizeof(*fs->dir_table);

  fseek(fptr, FAT_start, SEEK_SET);
  fread(fs->FAT, 1, fs->superblock->file_max, fptr);

  for (size_t i = 0; i < fs->superblock->file_max; ++i) {
    size_t seek =
        FAT_start + fs->superblock->file_max + fs->superblock->block_size * i;

    fseek(fptr, seek, SEEK_SET);
    fread(fs->data + i * fs->superblock->block_size, 1,
          fs->superblock->block_size, fptr);
  }

  return fs;
}

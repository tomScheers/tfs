#include <stdio.h>
#include <stdlib.h>

#include "tfslib.h"

struct FileSystem *tfs_read_fs(char file_name[]) {
  FILE *fptr = fopen(file_name, "rb");
  if (!fptr)
    return NULL;
  struct FileSystem *fs = malloc(sizeof(*fs));
  struct SuperBlock *superblock = calloc(1, SUPERBLOCK_BYTES);
  if (!superblock) {
    fclose(fptr);
    return NULL;
  }
  fread(superblock, 1, SUPERBLOCK_BYTES, fptr);
  fs->superblock = superblock;
  print_superblock(fs);

  fs->FAT = calloc(1, superblock->file_max);
  fs->data = calloc(superblock->file_max, superblock->block_size);
  fs->dir_table = calloc(superblock->file_max, sizeof(*fs->dir_table));
  printf("Directory table size: %zu\n", sizeof(*fs->dir_table));

  size_t dir_table_entry_size = sizeof(struct DirTableEntry);
  for (size_t i = 0; i < superblock->file_max; ++i) {
    size_t seek = SUPERBLOCK_BYTES + dir_table_entry_size * i;
    printf("Seek index: %zu\n", seek);
    fseek(fptr, seek, SEEK_SET);
    fread(&fs->dir_table[i], 1,
          sizeof(struct DirTableEntry), fptr);
  }

  //printf("Dir table index: %lu\n",
  //       superblock->file_max * sizeof(*fs->dir_table));
  //fseek(fptr, SUPERBLOCK_BYTES, SEEK_SET);

  //fread(fs->dir_table, 1, superblock->file_max, fptr);
  //print_dir_table(fs, 3);

  size_t FAT_start =
      SUPERBLOCK_BYTES + superblock->file_max * sizeof(*fs->dir_table);
  printf("FAT SIZE: %zu\n", FAT_start);

  fseek(fptr, FAT_start, SEEK_SET);
  fread(fs->FAT, 1, superblock->file_max, fptr);
  print_FAT(fs);

  for (size_t i = 0; i < superblock->file_max; ++i) {
    size_t seek = FAT_start + superblock->file_max + superblock->block_size * i;
    printf("Seeking to: %zu\n", seek);
    fseek(fptr, seek, SEEK_SET);
    fread(fs->data + i * superblock->block_size, 1, superblock->block_size,
          fptr);
    if (fs->data + i * superblock->block_size)
      printf("Data: %s\n", (fs->data + i * superblock->block_size));
  }
  printf("GOT ALL THE WAY HERE!??\n");
  return fs;
}

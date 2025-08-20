#include <stdint.h>
#include <stdio.h>

#include "tfslib.h"

//// Writes the file system to a file
// int8_t tfs_save_fs(struct FileSystem *fs, char *name) {
//   FILE* fptr = fopen(name, "w");
//   if (!fptr)
//     return -1;
//   fwrite(&fs->superblock, sizeof(fs->superblock) * fs->superblock->block_size,
//   1, fptr); printf("Size: %lu\n", sizeof(*fs->FAT) * fs->superblock->FAT_size
//   * fs->superblock->block_size); fwrite(fs->dir_table, sizeof(*fs->dir_table)
//   * fs->superblock->dir_table_size * fs->superblock->block_size, 1, fptr);
//   print_superblock(fs);
//   printf("Size: %lu\n", sizeof(*fs->FAT) * fs->superblock->FAT_size *
//   fs->superblock->block_size); fwrite(fs->FAT, sizeof(*fs->FAT) *
//   fs->superblock->FAT_size * fs->superblock->block_size, 1, fptr);
//   fwrite(fs->data, (*fs->data) * (fs->superblock->total_blocks -
//   fs->superblock->dir_table_size - fs->superblock->FAT_size - 1) *
//   fs->superblock->block_size, 1, fptr); fclose(fptr); return 0;
// }
//

int8_t tfs_save_fs(struct FileSystem *fs, char *file_name) {
  FILE *fptr = fopen(file_name, "wb");
  if (!fptr)
    return ERR_GENERAL;

  fwrite(fs->superblock, 1, SUPERBLOCK_BYTES, fptr);

  for (size_t i = 0; i < fs->superblock->file_max; ++i)
    fwrite((unsigned char*)fs->dir_table + i * sizeof(*fs->dir_table), 1, sizeof(*fs->dir_table), fptr);

  fwrite(fs->FAT, 1, fs->superblock->file_max, fptr);
  for (size_t i = 0; i < fs->superblock->file_max; ++i)
      fwrite(fs->data + i * fs->superblock->block_size, 1, fs->superblock->block_size, fptr);

  fclose(fptr);
  return 0;
}

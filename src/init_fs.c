#include "tfslib.h"
#include <stdlib.h>

// Initializes the file system
struct FileSystem *tfs_init() {
  struct FileSystem *fs = malloc(sizeof(*fs));
  if (!fs)
    return NULL;

  fs->FAT = calloc(1, MAX_FILE);
  if (!fs->FAT) {
    free(fs);
    return NULL;
  }

  fs->data = calloc(MAX_FILE, BLOCK_SIZE);
  if (!fs->data) {
    free(fs->FAT);
    free(fs);
    return NULL;
  }

  fs->dir_table = calloc(MAX_FILE, sizeof(*fs->dir_table));
  if (!fs->dir_table) {
    free(fs->FAT);
    free(fs->data);
    free(fs);
    return NULL;
  }

  fs->superblock = calloc(1, SUPERBLOCK_BYTES);
  if (!fs->superblock) {
    free(fs->FAT);
    free(fs->data);
    free(fs->dir_table);
    free(fs);
    return NULL;
  }

  fs->superblock->magic = MAGIC_NUMBER;
  fs->superblock->block_size = BLOCK_SIZE;
  fs->superblock->free_blocks = MAX_FILE;
  fs->superblock->file_max = MAX_FILE;
  fs->superblock->version = VERSION;

  return fs;
}

#include <stdlib.h>

#include "tfslib.h"

// Frees all data on the file system
void tfs_free_fs(struct FileSystem *fs) {
  if (!fs)
    return;
  free(fs->data);
  free(fs->FAT);
  free(fs->dir_table);
  free(fs->superblock);
  free(fs);
}

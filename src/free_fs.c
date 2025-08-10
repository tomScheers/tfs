#include <stdio.h>
#include <stdlib.h>

#include "tfslib.h"

void tfs_free_fs(struct FileSystem *fs) {
  if (!fs)
    return;
  free(fs->data);
  printf("Freed fs->data\n");
  free(fs->FAT);
  printf("Freed FAT\n");
  free(fs->dir_table);
  free(fs);
  printf("Free fs\n");
}

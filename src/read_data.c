#include <stdio.h>
#include <stdlib.h>

#include "tfslib.h"

struct FileSystem *tfs_read_fs(char file_name[]) {
  FILE *fptr = fopen(file_name, "rb");
  if (!fptr)
    return NULL;
  struct FileSystem *fs = malloc(sizeof(struct FileSystem));
  if (fread(fs, sizeof(*fs), 1, fptr) != 1) {
    fclose(fptr);
    return NULL;
  }
  fclose(fptr);
  return fs;
}

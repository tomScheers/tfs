#include <stdio.h>
#include "tfslib.h"

int main() {
  struct FileSystem *fs = tfs_init();
  printf("%u\n", fs->superblock.magic);
  fs->superblock.version = 67;
  tfs_save_fs(fs, "save.tfs");

  printf("File saved successfully!\n");

  struct FileSystem *fs_cpy = tfs_read_fs("save.tfs");
  printf("magic = %u\nversion = %u\n", fs_cpy->superblock.magic, fs_cpy->superblock.version);
  tfs_free_fs(fs_cpy);
  printf("Freed fs_cpy\n");
  tfs_free_fs(fs);
  printf("Freed fs\n");
	return 0;
}

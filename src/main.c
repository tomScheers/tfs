#include <stdio.h>
#include "tfslib.h"

int main() {
  struct FileSystem *fs = tfs_init();

  unsigned char* data = (unsigned char*)"Hello, World!";

  int32_t index = tfs_write_data(fs, "test.txt", data, sizeof(data));

  printf("%s\n", read_data(fs, index));
  
  tfs_free_fs(fs);
	return 0;
}

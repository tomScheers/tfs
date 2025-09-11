#include "test.h"
#include "tfslib.h"
#include "unity.h"

#include <stdlib.h>

void tfs_test_dir() {
  struct FileSystem *fs = tfs_init();
  //  tfs_mk_dir(fs, "dir");
  //  tfs_mk_dir(fs, "my_dir");
  //  tfs_write_data(fs, "file", (unsigned char *)"data", sizeof("data"));
  //  tfs_mk_dir(fs, "your_dir");
  //  TEST_ASSERT_EQUAL(fs->dir_table[0].is_dir, true);
  //  tfs_free_fs(fs);
  tfs_free_fs(fs);
}

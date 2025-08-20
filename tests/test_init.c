#include "test.h"
#include "tfslib.h"
#include "unity.h"

void tfs_test_init() {
  struct FileSystem *fs = tfs_init();
  TEST_ASSERT_NOT_EQUAL_MESSAGE(fs, NULL, "Filesystem must not be NULL");
  TEST_ASSERT_EQUAL_UINT16(fs->superblock->block_size, BLOCK_SIZE);
  TEST_ASSERT_EQUAL_UINT16(fs->superblock->free_blocks, MAX_FILE);
  TEST_ASSERT_EQUAL_UINT16(fs->superblock->file_max, MAX_FILE);
  TEST_ASSERT_EQUAL_UINT32(fs->superblock->magic, MAGIC_NUMBER);
  TEST_ASSERT_EQUAL_UINT16(fs->superblock->version, VERSION);
  TEST_ASSERT_EMPTY(fs->data);
  TEST_ASSERT_EMPTY(fs->FAT);
  tfs_free_fs(fs);
}

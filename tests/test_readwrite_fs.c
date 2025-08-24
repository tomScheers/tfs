#include "test.h"
#include "tfslib.h"
#include "unity.h"

#include <stdint.h>
#include <stdlib.h>

void tfs_test_readwrite_fs() {
  struct FileSystem *fs = tfs_init();
  uint16_t new_version = 10;
  fs->superblock->version = new_version;
  unsigned char data[] = "Hello World!";
  size_t index = tfs_write_data(fs, "test.txt", data, sizeof(data));
  unsigned char *read_data = tfs_read_data(fs, index);
  TEST_ASSERT_EQUAL_STRING(data, read_data);
  free(read_data);
  tfs_save_fs(fs, "file.tfs");
  tfs_free_fs(fs);
  struct FileSystem *read_fs = tfs_read_fs("file.tfs");
  TEST_ASSERT_NOT_EQUAL_MESSAGE(read_fs, NULL, "File system must not be NULL");
  TEST_ASSERT_EQUAL_INT16(read_fs->superblock->block_size, BLOCK_SIZE);
  TEST_ASSERT_EQUAL_INT16(read_fs->superblock->free_blocks, MAX_FILE - 1);
  TEST_ASSERT_EQUAL_INT16(read_fs->superblock->amount_of_files, 1);
  TEST_ASSERT_EQUAL_INT16(read_fs->superblock->file_max, MAX_FILE);
  TEST_ASSERT_EQUAL_HEX32(read_fs->superblock->magic, MAGIC_NUMBER);
  TEST_ASSERT_EQUAL_INT16(read_fs->superblock->version, new_version);
  read_data = tfs_read_data(read_fs, index);
  TEST_ASSERT_EQUAL_STRING(read_data, data);
  free(read_data);
  tfs_free_fs(read_fs);
}

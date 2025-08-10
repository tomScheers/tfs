#include "tfslib.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void tfs_test_init();
void tfs_test_readwrite();

int main() {
  UNITY_BEGIN();
  RUN_TEST(tfs_test_init);
  RUN_TEST(tfs_test_readwrite);
  UNITY_END();
  return 0;
}

void tfs_test_init() {
  struct FileSystem *fs = tfs_init();
  TEST_ASSERT_NOT_EQUAL(fs, NULL);
  TEST_ASSERT_EQUAL(fs->superblock.block_size, BLOCK_SIZE);
  TEST_ASSERT_EQUAL(fs->superblock.dir_table_size, DIR_TABLE_SIZE);
  TEST_ASSERT_EQUAL(fs->superblock.dir_table_start, DIR_TABLE_START);
  TEST_ASSERT_EQUAL(fs->superblock.FAT_size, FAT_SIZE);
  TEST_ASSERT_EQUAL(fs->superblock.FAT_start, FAT_START);
  TEST_ASSERT_EQUAL(fs->superblock.free_blocks, FREE_BLOCKS);
  TEST_ASSERT_EQUAL(fs->superblock.total_blocks, TOTAL_BLOCKS);
  TEST_ASSERT_EQUAL(fs->superblock.magic, MAGIC_NUMBER);
  TEST_ASSERT_EQUAL(fs->superblock.version, VERSION);
  tfs_free_fs(fs);
}

void tfs_test_readwrite() {
  struct FileSystem *fs = tfs_init();
  uint16_t new_version = 10;
  uint16_t new_block_size = 8192;
  uint16_t new_FAT_size = 6;
  fs->superblock.version = new_version;
  fs->superblock.FAT_size = new_FAT_size;
  fs->superblock.block_size = new_block_size;
  tfs_save_fs(fs, "file.tfs");
  tfs_free_fs(fs);
  struct FileSystem *read_fs = tfs_read_fs("file.tfs");
  TEST_ASSERT_NOT_EQUAL(read_fs, NULL);
  TEST_ASSERT_EQUAL(read_fs->superblock.block_size, new_block_size);
  TEST_ASSERT_EQUAL(read_fs->superblock.dir_table_size, DIR_TABLE_SIZE);
  TEST_ASSERT_EQUAL(read_fs->superblock.dir_table_start, DIR_TABLE_START);
  TEST_ASSERT_EQUAL(read_fs->superblock.FAT_size, new_FAT_size);
  TEST_ASSERT_EQUAL(read_fs->superblock.FAT_start, FAT_START);
  TEST_ASSERT_EQUAL(read_fs->superblock.free_blocks, FREE_BLOCKS);
  TEST_ASSERT_EQUAL(read_fs->superblock.total_blocks, TOTAL_BLOCKS);
  TEST_ASSERT_EQUAL(read_fs->superblock.magic, MAGIC_NUMBER);
  TEST_ASSERT_EQUAL(read_fs->superblock.version, new_version);
  tfs_free_fs(read_fs);
}

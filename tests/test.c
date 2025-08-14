#include "tfslib.h"
#include "unity.h"
#include <time.h>

void setUp(void) {}
void tearDown(void) {}

void tfs_test_init();
void tfs_test_readwrite_fs();
void tfs_test_readwrite_data();

int main() {
  UNITY_BEGIN();
  RUN_TEST(tfs_test_init);
  RUN_TEST(tfs_test_readwrite_fs);
  RUN_TEST(tfs_test_readwrite_data);
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

void tfs_test_readwrite_fs() {
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

void tfs_test_readwrite_data() {
  struct FileSystem *fs = tfs_init();
  unsigned char* data = (unsigned char*)"This is my data!";
  uint16_t index = tfs_write_data(fs, "test.txt", data, sizeof(data));
  unsigned char* data_a = (unsigned char*)"Even more data!!";
  uint16_t index_a = tfs_write_data(fs, "test_a.txt", data_a, sizeof(data_a));
  printf("Index; %hu, Index_a: %hu\n", index, index_a);
  TEST_ASSERT_EQUAL_STRING(data, tfs_read_data(fs, index));
  TEST_ASSERT_EQUAL_STRING(data_a, tfs_read_data(fs, index_a));
  TEST_ASSERT_NOT_EQUAL(index, index_a);
  TEST_ASSERT_NOT_EQUAL(fs->dir_table[index].starting_block, fs->dir_table[index_a].starting_block);
  TEST_ASSERT_EQUAL(fs->dir_table[index].is_dir, 0);
  TEST_ASSERT_NOT_EQUAL(fs->dir_table[index].created, (time_t)0); 
  TEST_ASSERT_NOT_EQUAL(fs->dir_table[index].last_modified, (time_t)0);
  TEST_ASSERT_EQUAL(fs->dir_table[index].last_modified, fs->dir_table[index].created);
}

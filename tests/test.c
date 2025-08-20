#include "tfslib.h"
#include "unity.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

void setUp(void) {}
void tearDown(void) {}

void tfs_test_init();
void tfs_test_readwrite_fs();
void tfs_test_readwrite_data();

int main() {
  printf("Superblock size: %zu\n", sizeof(struct SuperBlock));
  UNITY_BEGIN();
  RUN_TEST(tfs_test_init);
  RUN_TEST(tfs_test_readwrite_data);
  RUN_TEST(tfs_test_readwrite_fs);
  UNITY_END();
  return 0;
}

void tfs_test_init() {
  struct FileSystem *fs = tfs_init();
  TEST_ASSERT_NOT_EQUAL_MESSAGE(fs, NULL, "Filesystem must not be NULL");
  TEST_ASSERT_EQUAL_UINT16(fs->superblock->block_size, BLOCK_SIZE);
  TEST_ASSERT_EQUAL_UINT16(fs->superblock->free_blocks, MAX_FILE);
  TEST_ASSERT_EQUAL_UINT16(fs->superblock->file_max, MAX_FILE);
  TEST_ASSERT_EQUAL_UINT32(fs->superblock->magic, MAGIC_NUMBER);
  TEST_ASSERT_EQUAL_UINT16(fs->superblock->version, VERSION);
  tfs_free_fs(fs);
}

void tfs_test_readwrite_fs() {
  struct FileSystem *fs = tfs_init();
  uint16_t new_version = 10;
  fs->superblock->version = new_version;
  unsigned char data[] = "Hello World!";
  size_t index = tfs_write_data(fs, "test.txt", data, sizeof(data));
  TEST_ASSERT_EQUAL_STRING(data, tfs_read_data(fs, index));
  tfs_save_fs(fs, "file.tfs");
  tfs_free_fs(fs);
  struct FileSystem *read_fs = tfs_read_fs("file.tfs");
  printf("Even here!?\n");
  TEST_ASSERT_NOT_EQUAL_MESSAGE(read_fs, NULL, "File system must not be NULL");
  TEST_ASSERT_EQUAL_INT16(read_fs->superblock->block_size, BLOCK_SIZE);
  TEST_ASSERT_EQUAL_INT16(read_fs->superblock->free_blocks, MAX_FILE - 1);
  TEST_ASSERT_EQUAL_INT16(read_fs->superblock->file_max, MAX_FILE);
  TEST_ASSERT_EQUAL_HEX32(read_fs->superblock->magic, MAGIC_NUMBER);
  TEST_ASSERT_EQUAL_INT16(read_fs->superblock->version, new_version);
  // printf("Data read: %s\n", tfs_read_data(read_fs, index));
  printf("Data index to read: %zu\n", index);
  printf("Data on the data field: %s\n", (char*)(read_fs->data));
  TEST_ASSERT_EQUAL_STRING(tfs_read_data(read_fs, index), data);
  tfs_free_fs(read_fs);
}

void tfs_test_readwrite_data() {
  struct FileSystem *fs = tfs_init();
  unsigned char *data = (unsigned char *)"This is my data!";
  uint16_t index =
      tfs_write_data(fs, "test.txt", data, strlen((char *)data) + 1);
  unsigned char *data_a = (unsigned char *)"Even more data!!";
  uint16_t index_a =
      tfs_write_data(fs, "test_a.txt", data_a, strlen((char *)data_a) + 1);

  unsigned char *read_data = tfs_read_data(fs, index);

  TEST_ASSERT_EQUAL_STRING(data, read_data);

  free(read_data);
  read_data = tfs_read_data(fs, index_a);
  TEST_ASSERT_EQUAL_STRING(data_a, read_data);
  free(read_data);
  TEST_ASSERT_NOT_EQUAL_INT16(index, index_a);
  TEST_ASSERT_NOT_EQUAL_INT16(fs->dir_table[index].starting_block,
                              fs->dir_table[index_a].starting_block);
  TEST_ASSERT_FALSE(fs->dir_table[index].is_dir);
  TEST_ASSERT_FALSE(fs->dir_table[index_a].is_dir);
  TEST_ASSERT_NOT_EQUAL(fs->dir_table[index].created, (time_t)0);
  TEST_ASSERT_NOT_EQUAL(fs->dir_table[index].last_modified, (time_t)0);
  TEST_ASSERT_EQUAL(fs->dir_table[index].last_modified,
                    fs->dir_table[index].created);

  // Tests for large data sizes
  unsigned char *data_l = malloc(fs->superblock->block_size * 3 + 1);
  for (int i = 0; i < fs->superblock->block_size * 3; ++i)
    data_l[i] = 'a';

  data_l[fs->superblock->block_size * 3] = '\0';

  uint16_t index_l = tfs_write_data(fs, "test_l.txt", data_l,
                                    fs->superblock->block_size * 3 + 1);

  read_data = tfs_read_data(fs, index_l);
  TEST_ASSERT_EQUAL_STRING(data_l, read_data);
  free(data_l);
  free(read_data);
  tfs_free_fs(fs);

  fs = tfs_init();
  uint8_t index_superblock =
      tfs_write_data(fs, "superblock->data", (unsigned char *)fs->superblock,
                     sizeof(*fs->superblock));

  struct SuperBlock *read_superblock =
      (struct SuperBlock *)tfs_read_data(fs, index_superblock);
  TEST_ASSERT_EQUAL_UINT16(read_superblock->block_size, BLOCK_SIZE);
  TEST_ASSERT_EQUAL_UINT16(read_superblock->free_blocks, MAX_FILE);
  TEST_ASSERT_EQUAL_UINT16(read_superblock->file_max, MAX_FILE);
  TEST_ASSERT_EQUAL_UINT32(read_superblock->magic, MAGIC_NUMBER);
  TEST_ASSERT_EQUAL_UINT16(read_superblock->version, VERSION);

  free(read_superblock);
  tfs_free_fs(fs);

}

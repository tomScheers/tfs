#include "test.h"
#include "tfslib.h"
#include "unity.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void tfs_test_readwrite_data() {
  struct FileSystem *fs = tfs_init();
  unsigned char *data = (unsigned char *)"This is my data!";
  uint16_t index =
      tfs_write_data(fs, "test.txt", data, strlen((char *)data) + 1);

  unsigned char *data_text = (unsigned char *)"Even more data!!";
  uint16_t index_text = tfs_write_data(fs, "test_a.txt", data_text,
                                       strlen((char *)data_text) + 1);

  unsigned char *read_data = tfs_read_data(fs, index);

  TEST_ASSERT_EQUAL_STRING(data, read_data);

  free(read_data);
  read_data = tfs_read_data(fs, index_text);
  TEST_ASSERT_EQUAL_STRING(data_text, read_data);
  free(read_data);
  TEST_ASSERT_NOT_EQUAL_INT16(index, index_text);
  TEST_ASSERT_NOT_EQUAL_INT16(fs->dir_table[index].starting_block,
                              fs->dir_table[index_text].starting_block);
  TEST_ASSERT_FALSE(fs->dir_table[index].is_dir);
  TEST_ASSERT_FALSE(fs->dir_table[index_text].is_dir);
  TEST_ASSERT_NOT_EQUAL(fs->dir_table[index].created, (time_t)0);
  TEST_ASSERT_NOT_EQUAL(fs->dir_table[index].last_modified, (time_t)0);
  TEST_ASSERT_EQUAL(fs->dir_table[index].last_modified,
                    fs->dir_table[index].created);

  // Tests for large data sizes
  unsigned char *data_long = malloc(fs->superblock->block_size * 3 + 1);
  for (int i = 0; i < fs->superblock->block_size * 3; ++i)
    data_long[i] = 'a';

  data_long[fs->superblock->block_size * 3] = '\0';

  uint16_t index_l = tfs_write_data(fs, "test_l.txt", data_long,
                                    fs->superblock->block_size * 3 + 1);

  read_data = tfs_read_data(fs, index_l);
  TEST_ASSERT_EQUAL_STRING(data_long, read_data);
  free(data_long);
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

  struct DirTableEntry dir_table = fs->dir_table[index_superblock];
  TEST_ASSERT_EQUAL_UINT16(dir_table.size, sizeof(*fs->superblock));
  TEST_ASSERT_EQUAL_UINT16(dir_table.FAT_index, 0); // First empty FAT
  TEST_ASSERT_EQUAL_UINT16(dir_table.created, dir_table.last_modified);
  TEST_ASSERT_FALSE(dir_table.is_dir);
  TEST_ASSERT_EQUAL_UINT16(dir_table.starting_block, 0);

  free(read_superblock);
  tfs_free_fs(fs);
}

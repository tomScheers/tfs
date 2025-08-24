#include "test.h"
#include "tfslib.h"
#include "unity.h"
#include <stdlib.h>

void tfs_test_delete_file() {
  struct FileSystem *fs = tfs_init();
  char data_1[] = "This is my data!";
  char data_2[] = "Ipsum lorem blablabla";
  char data_3[] = "Hello again";
  char data_4[BLOCK_SIZE * 3 + 1];
  for (size_t i = 0; i < BLOCK_SIZE * 3; ++i)
    data_4[i] = 'A';
  data_4[BLOCK_SIZE * 3] = '\0';
  tfs_write_data(fs, "first.c", (unsigned char *)data_1, sizeof(data_1));
  tfs_write_data(fs, "second.c", (unsigned char *)data_2, sizeof(data_2));
  tfs_write_data(fs, "third.c", (unsigned char *)data_3, sizeof(data_3));
  tfs_write_data(fs, "fourth.c", (unsigned char *)data_4, sizeof(data_4));
  for (size_t i = 0; i < 3; ++i) {
    TEST_ASSERT_EQUAL_UINT16(DATA_EOF, fs->FAT[i]);
    TEST_ASSERT_EQUAL_UINT16(i + 4, fs->FAT[i + 3]);
  }
  TEST_ASSERT_EQUAL_UINT16(DATA_EOF, fs->FAT[6]);

  tfs_delete_file(fs, 0);
  TEST_ASSERT_EQUAL_UINT16(0x00, fs->FAT[0]);
  tfs_write_data(fs, "alotofdata.txt", (unsigned char *)data_4, sizeof(data_4));
  TEST_ASSERT_EQUAL_UINT16(7, fs->FAT[0]);
  TEST_ASSERT_EQUAL_UINT16(8, fs->FAT[7]);
  TEST_ASSERT_EQUAL_UINT16(9, fs->FAT[8]);
  TEST_ASSERT_EQUAL_UINT16(DATA_EOF, fs->FAT[9]);

  unsigned char *read_data = tfs_read_data(fs, 0);
  TEST_ASSERT_EQUAL_STRING(data_4, read_data);

  tfs_delete_file(fs, 0);
  free(read_data);
  tfs_free_fs(fs);
}

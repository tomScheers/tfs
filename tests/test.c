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

// These tests don't have propper error handling yet
int main() {
  UNITY_BEGIN();
  RUN_TEST(tfs_test_init);
  RUN_TEST(tfs_test_readwrite_data);
  RUN_TEST(tfs_test_readwrite_fs);
  UNITY_END();
  return 0;
}

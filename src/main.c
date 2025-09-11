#include "render.h"
#include "tfslib.h"

#include <stdio.h>
#include <string.h>

int main() {
  struct FileSystem *fs = tfs_init();

  unsigned char *data_a =
      (unsigned char
           *)"Hello, "
             "World!\na\nb\nc\nd\ne\ne\nf\nf\n\n\n\n\n\n\n\n\n\n\nHello, "
             "World!\na\nb\nc\nd\ne\ne\nf\nf\n\n\n\n\n\n\n\n\n\n\n";

  unsigned char *data_b = (unsigned char *)"This is my data for B\n";
  unsigned char *data_c =
      (unsigned char
           *)"Oh right...\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nC's data...\n\n\n\nbruv";
  unsigned char *data_d = (unsigned char *)NULL;

  tfs_write_data(fs, "test.txt", data_a, strlen((char *)data_a));
  tfs_write_data(fs, "hello.c", data_b, strlen((char *)data_b));
  tfs_write_data(fs, "wolrd.c", data_c, strlen((char *)data_c));
  tfs_write_data(fs, "i.use.vim.btw", data_d, 0);

  render_fs(fs);

  tfs_free_fs(fs);
  return 0;
}

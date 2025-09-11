#include "render.h"
#include "tfslib.h"

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

void render_fs(struct FileSystem *fs) {
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  curs_set(0);

  size_t selected_entry = 0;
  ssize_t opened_entry_index = -1;
  int c = -1;

  do {
    wclear(stdscr);
    switch (c) {
    case 'j':
      if (selected_entry > fs->superblock->amount_of_files)
        break;
      ++selected_entry;
      break;
    case 'k':
      if (selected_entry <= 0)
        break;
      --selected_entry;
      break;
    case 10: // Enter
      opened_entry_index = selected_entry;
      break;
    case 'r':
      opened_entry_index = -1;
      break;
    }

    if (opened_entry_index == -1) {
      for (size_t entry_index = 0;
           entry_index < fs->superblock->amount_of_files; ++entry_index) {
        if (entry_index == selected_entry) {
          printw("* %s\n", fs->dir_table[entry_index].name);
        } else {
          printw("  %s\n", fs->dir_table[entry_index].name);
        }
      }
    } else {
      unsigned char *data = tfs_read_data(fs, opened_entry_index);
      printw("%s\n", data);
    }
    refresh();
  } while ((c = getch()) != 'q');

  endwin();
}

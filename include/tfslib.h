#ifndef TFS_H
#define TFS_H

#include <stdint.h>
#include <time.h>

#define BLOCK_SIZE 4096
#define MAGIC_NUMBER 0x74667300 // 't' 'f' 's' '\0', used for magic number
#define VERSION 0
#define TOTAL_BLOCKS 32
#define DIR_TABLE_START 1
#define DIR_TABLE_SIZE 1
#define FAT_START 2
#define FAT_SIZE 1
#define FREE_BLOCKS TOTAL_BLOCKS - DIR_TABLE_SIZE - FAT_SIZE - 1

struct SuperBlock {
  uint32_t magic;
  uint16_t version;
  uint16_t block_size;
  uint16_t total_blocks;
  uint16_t free_blocks;
  uint16_t dir_table_start;
  uint16_t dir_table_size;
  uint16_t FAT_start;
  uint16_t FAT_size;
};

// This will go into an array-of-structs
struct DirTableEntry {
  time_t created;
  time_t last_modified;
  uint16_t starting_block;
  uint8_t is_dir; // Directories are handled differently from files, see doc
  char name[64];
};

struct FileSystem {
  struct SuperBlock superblock;
  struct DirTableEntry
      *dir_table; // Array-of-structs, each struct represents one file/directory
  unsigned char *FAT;      // Must be allocated on the heap
  unsigned char **data;    // Must be allocated on the heap
};

struct FileSystem *tfs_init();
int8_t tfs_save_fs(struct FileSystem *fs, char name[]);
struct FileSystem *tfs_read_fs(char file_name[]);
void tfs_free_fs(struct FileSystem *fs);
int32_t tfs_write_data(struct FileSystem *fs, char file_path[],
                       unsigned char *bytes, size_t size);
unsigned char* tfs_read_data(struct FileSystem *fs, uint16_t index);

#endif

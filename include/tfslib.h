#ifndef TFS_H
#define TFS_H

#include <stdint.h>
#include <time.h>

#define BLOCK_SIZE 64
#define MAGIC_NUMBER 0x74667300 // 't' 'f' 's' '\0', used for magic number
#define VERSION 0
// #define TOTAL_BLOCKS 12
#define MAX_FILE 12
//#define DIR_TABLE_START 1
//#define DIR_TABLE_SIZE 1
//#define FAT_START 2
//#define FAT_SIZE 1
// #define FREE_BLOCKS TOTAL_BLOCKS - DIR_TABLE_SIZE - FAT_SIZE - 1

#define DATA_EOF 0xFF
#define SUPERBLOCK_BYTES 64

#define DATA_BLOCKS(fs)                                                        \
  (fs->superblock->total_blocks - fs->superblock->dir_table_size -             \
   fs->superblock->FAT_size - 1)

// These errors are only returned through non-pointer return types
// Errors are always negative
enum Errors {
  ERR_GENERAL = -1, // General errors are useful if it's a system error thus it
                    // can be handled using errno
  ERR_INSUFFICIENT_MEMORY, // Insufficient memory i.e. writing more memory
                           // blocks then that there are blocks free
  ERR_TIME_RETRIEVAL,      // If time(NULL) returns (time_t)-1
};

struct SuperBlock {
  uint32_t magic; // Magic number to verify the file
  uint16_t version;
  uint16_t block_size;
  // uint16_t total_blocks;
  uint16_t free_blocks;
  // uint16_t dir_table_start;
  // uint16_t dir_table_size;
  // uint16_t FAT_start;
  // uint16_t FAT_size;
  /*
   * Since each FAT item represents one data element and is exactly 1 byte
   * large, therefore we can use file_max to represent the size of our FAT.
   * The same goes for dir_table_size, since each file and directory must have a
   * corresponding dir_table for metadata, the sizeof *dir_table can be
   * calculated using file_max * sizeof(struct DirTableEntry).
   */
  /*
   * One thing which I must note here is that all of the restricted memory
   * doesn't follow the same data format as the data field does.
   * */
  uint16_t file_max; // New method of calculating directory size
};

// This will go into an array-of-structs
struct DirTableEntry {
  time_t created;
  time_t last_modified;
  uint16_t starting_block;
  uint16_t size;      // Size of the data
  uint16_t FAT_index; // Index of the FAT entry
  uint8_t is_dir;     // Directories are handled differently from files, see doc
  char name[64];
};

struct FileSystem {
  struct SuperBlock *superblock;
  struct DirTableEntry
      *dir_table; // Array-of-structs, each struct represents one file/directory
  unsigned char *FAT; // Must be allocated on the heap
  unsigned char
      *data; // Must be allocated on the heap, 1-dimensional flat array
};

struct FileSystem *tfs_init();
int8_t tfs_save_fs(struct FileSystem *fs, char name[]);
struct FileSystem *tfs_read_fs(char file_name[]);
void tfs_free_fs(struct FileSystem *fs);
int32_t tfs_write_data(struct FileSystem *fs, char file_path[],
                       unsigned char *bytes, size_t size);
unsigned char *tfs_read_data(struct FileSystem *fs, uint16_t index);

// Utility functions
void print_FAT(struct FileSystem *fs);
void print_dir_table(struct FileSystem *fs, size_t index);
void print_superblock(struct FileSystem *fs);

#endif

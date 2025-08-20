#include "tfslib.h"
#include <stdio.h>

void print_FAT(struct FileSystem* fs) {
  puts("--- FAT ---");
  for (int i = 0; i < fs->superblock->total_blocks; ++i)
    printf("%02X ", fs->FAT[i]);
  puts("\n"); // Actually puts two new line because puts auto-adds them
}

void print_dir_table(struct FileSystem* fs, size_t index) {
  puts("--- Dir Table ---");
  printf("Name: %s\n", fs->dir_table[index].name);
  printf("Size: %hu\n", fs->dir_table[index].size);
  printf("FAT index: %hu\n", fs->dir_table[index].FAT_index);
  printf("Is directory: %u\n", fs->dir_table[index].is_dir);
  printf("Starting data block: %u\n", fs->dir_table[index].starting_block);
  printf("Created: %ld\n", (long)fs->dir_table[index].created);
  printf("Modified: %ld\n\n", (long)fs->dir_table[index].last_modified);
}

void print_superblock(struct FileSystem* fs) {
  puts("--- Superblock ---");
  printf("Magic: %X\n", fs->superblock->magic);
  printf("Version: %hu\n", fs->superblock->version);
  printf("Block Size: %hu\n", fs->superblock->block_size);
  printf("Max files: %hu\n", fs->superblock->file_max);
  printf("Free blocks: %hu\n", fs->superblock->free_blocks);
  printf("Total blocks: %hu\n", fs->superblock->total_blocks);
}

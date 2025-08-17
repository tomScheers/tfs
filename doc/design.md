# Design notes
For this file system project I'm gonna use a FAT system with a directory table and a superblock for metadata.

## File architecture
Our file system will be stored in a file with a .tfs format.
Our data is organized in blocks of 4kB, where the first 3 blocks are reserved for metadata.
The file is organized in the following sequence:
| Block | Type            |
|-------|-----------------|
| #0    | Superblock      |
| #1    | Directory Table |
| #2    | FAT             |
| #3+   | Data            |

### Superblock
The superblock is used to store data about the file system itself. The data is stored in the following sequence:
| Field                 | Size         |
|-----------------------|--------------|
| Magic Number          | 4 bytes      |
| Version               | 2 bytes      |
| Block Size            | 2 bytes      |
| Total blocks          | 2 bytes      |
| Free blocks           | 2 bytes      |
| Directory Table Start | 2 bytes      |
| Directory Block Count | 2 bytes      |
| FAT Start Block       | 2 bytes      |
| FAT Block Count       | 2 bytes      |

When starting the program this is the first thing which gets read. This is used to firstly confirm the file is compatible through the magic number, the other is metadata which is usually the same but it's still handy to have it in the superblock if specifications change.

### Directory Table
The directory table holds all of the metadata about the file alongside pointing to its starting block.
| Field          | Size     |
|----------------|----------|
| Name           | 64 bytes |
| Created        | 8 bytes  |
| Last Modified  | 8 bytes  |
| Starting block | 2 bytes  |
| Data Size      | 2 bytes  |

> Keep in mind that both the superblock and the directory table are stored in an entire block, meaning that it's possible to store more metadata in the future

### FAT
The FAT is basically just a list of addresses with a size of 2 bytes each. Each address is the next file it points to, with 0xFF representing the EOF, and 0x00 meaning the data is free or reserved:
```0x00 0x00 0x00 0x06 0x00 0x09 0x00 0x00 0xFF```
Here the first three blocks are 0x00, meaning they're reserved. After that we get 0x06, which means that block #4 points to block #6 (0-indexed).
block #6 points to 0x09 (block #8), which in turn is FF, meaning it's EOF.

### Blocks
Each data block is the of the size given in the superblock. It just stores the data in a binary format and any metadata associated with it is stored in the directory table or the FAT.
The EOF in a block 

## Dir entry types
A dir_entry can either be a file or a directory.
If it is a file it just stores data in different blocks which, you can find the blocks you need to use and the order in the FAT or the directory entry.
If it is a directory, it acts like a FAT in the sense that it stores all the blocks in the directory in order. This also means that it has a reserved block of its own.
So say you wanted to open a nested file, say for example src/utils/foo.c, the program would first search for the src directory on the FAT, when it finds the location of the directory it does the exact same thing but then for the utils directory, and it does that again to find all the blocks which store data about foo.c.


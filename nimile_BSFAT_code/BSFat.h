#ifndef _BS_FAT_H_
#define _BS_FAT_H_

#include <stdlib.h>
#include <stdio.h>

#include "definitions.h"
#include "BSSort.h"
#include "BSFile.h"
#include "BSBlock.h"
#include "BSCluster.h"

/*
 * This struct contains information about a FAT filesystem
 */
struct BSFat_t {
	/// This attribute contains the files
	BSFile** files;

	/// This attribute contains the double linked list cluster of blocks
	BSCluster* cluster;

	/// This attribute describes the size of the disk
	int disk_size;

	/// This attribute describes the amount of blocks at the disk
	int block_count;
};

/**
 * This method create a FAT Filesystem with given size and given block size.
 * @param disk_size_t int Size of the disk
 * @param block_size_t int Size of each block
 * @return BSFat* Pointer to the file system
 */
BSFat* BSFAT_createBSFat(int disk_size_t, int block_size_t);

/**
 * This method defrags a file system.
 * @param file_system_t BSFat* This param is the file system which should be defragged
 */
void BSFAT_defrag(BSFat* file_system_t);

/**
 * TODO
 * USAGE?
 */
int BSFAT_insert_file(BSFat* file_system_t, BSFile* file_t);

/**
 * This method retrieves the next free file index inside a file system.
 * @param file_system_t BSFat* File system where the next file should be located.
 * @return int Index of the next possible file place
 */
int BSFAT_get_free_file_index(BSFat* file_system_t);

/**
 * This method gets the disk size of a file system.
 * @param file_system_t BSFat* File system which should be used for the operation
 * @return int Total size of the disk
 */
int BSFAT_get_disk_size(BSFat* file_system_t);

/**
 * This method gets the amount of blocks from a file system.
 * @param file_system_t BSFat* File system which should be used for the operation
 * @return int Amount of blocks at a file system
 */
int BSFAT_get_block_count(BSFat* file_system_t);

/**
 * This method gets the amount of free space of a file system.
 * @param file_system_t BSFat* File system which should be used for the operation
 * @return int Total free space diskspace
 */
int BSFAT_get_free_disk_space(BSFat* file_system_t);

/**
 * This method gets the amount of free space of a file system in percentage form 0 to 1.
 * @param file_system_t BSFat* File system which should be used for the operation
 * @return float Total free space diskspace in percentage from 0 to 1
 */
float BSFAT_get_free_disk_space_percentage(BSFat* file_system_t);

/**
 * This method gets the amount of used space of a file system.
 * @param file_system_t BSFat* File system which should be used for the operation
 * @return int Total used space diskspace
 */
int BSFAT_get_used_disk_space(BSFat* file_system_t);

/**
 * This method gets the amount of used space of a file system in percentage form 0 to 1.
 * @param file_system_t BSFat* File system which should be used for the operation
 * @return float Total used space diskspace in percentage from 0 to 1
 */
float BSFAT_get_used_disk_space_percentage(BSFat* file_system_t);

/**
 * This method shows the state of each block of a file system.
 * @param file_system_t BSFat* File system which should be used for the operation
 */
void BSFAT_show_fat(BSFat* file_system_t);

/**
 * This method prints details about a file system.
 * @param file_system_t BSFat* File system which should be used for the operation
 */
void BSFAT_print(BSFat* file_system_t);

/**
 * This method gets average fragmentation of a file system.
 * @param file_system_t BSFat* File system which should be used for the operation
 * @return float Average fragmentation from a file system
 */
double BSFAT_fragmentation_percentage(BSFat* file_system_t);

#endif //_BS_FAT_H_

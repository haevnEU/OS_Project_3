#ifndef _BS_BLOCK_H_
#define _BS_BLOCK_H_

#include<stdlib.h>
#include<stdio.h>

#include"definitions.h"

/**
 * This struct contains information about a filesystem block
 */
struct BSBlock_t {
	/// This attribute is set to 1 iff the block is occupied by a file otherwise 0
	uint free_state : 1;
	/// This attribute is set to 1 iff the block is reserved by the filesystem otherwise it is 0
	uint reserved : 1;
	/// This attribute is set to 1 iff the block corruption is detected by the filesystem otherwise 0
	uint corrupted : 1;
	/// This attribute contains the size of the block
	uint size;
	/// This attribute is the index inside a cluster
	int index;
	/// This attribute contains the file id which occupies the block
	int file_index;
};

/**
 * This method creates a new block
 * @param size uint size of the block
 * @param index int index of the block inside the block cluster
 * @return BSBlock BSBlock* Pointer to the allocated memory
 * @exception
 */
BSBlock* BSBLOCK_createBlock(uint size, int index);

/**
 * This method sets the attribute free_state to 1
 * @param block_t BSBlock* Block which should be set.
 */
void BSBLOCK_set_data(BSBlock* block_t);

/**
 * This method sets the attribute free_state to 0
 * @param block_t BSBlock* Block which should be cleared.
 */
void BSBLOCK_clear_data(BSBlock* block_t);

/*
 * This method returns the block size
 * @param block_t BSBlock* Block from where the size should be retrieved.
 * @return int Blocksize of given parameter
 */
int BSBLOCK_blockSize(BSBlock* block_t);

/**
 * This method prints information about the block.
 * @param block_t BSBlock* Block which should be printed
 */
void BSBLOCK_printBlock(BSBlock* block_t);

#endif //_BS_BLOCK_H_

#include "BSBlock.h"

BSBlock* BSBLOCK_createBlock(uint size_t, int index_t) {
	BSBlock* block = (BSBlock*)malloc(sizeof(BSBlock));
	block->size = size_t;
	block->index = index_t;
	BSBLOCK_clear_data(block);
	block->corrupted = 0;
	return block;
}

void BSBLOCK_set_data(BSBlock* block_t) {
	block_t->free_state = 1;
}

void BSBLOCK_clear_data(BSBlock* block_t) {
	block_t->free_state = 0;
}

int BSBLOCK_blockSize(BSBlock* block_t) {
	return  block_t->size;
}

void BSBLOCK_printBlock(BSBlock* block_t) {
	char c;
	if (NULL == block_t) {
		printf("Error: Block not initialized");
		return;
	}
	char free = block_t->free_state == 1 ? 'O' : 'F';
	char reserved = block_t->reserved == 1 ? 'R' : ' ';
	char corrupted = block_t->corrupted == 1 ? 'C' : ' ';
	printf("Blocksize: %i"
		"\nBlockstate: %c | %c | %c | ",
		block_t->size, corrupted, free, reserved);
	printf("\n");
}

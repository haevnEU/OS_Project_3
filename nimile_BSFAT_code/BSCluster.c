#include "BSCluster.h"


BSCluster* BSCLUSTER_create_cluster(int size) {
	BSCluster* list = (BSCluster*)malloc(sizeof(BSCluster) * size);
	for (int i = 0; i < size; i++) {
		list[i].index = i;
		list[i].block = BSBLOCK_createBlock(size, i);
		list[i].block->index = i;
		list[i].next = NULL;
		if (i > 0) {
			list[i].prev = &list[i - 1];
		}
		else {
			list[i].prev = NULL;
		}
		if (i < size) {
			list[i - 1].next = &list[i];
		}
	}
	return list;
}

BSCluster* BSCLUSTER_get_next_cluster(BSFat* file_system_t) {
	if (NULL == file_system_t->cluster) {
		return EXCEPTION_BS_CLUSTER_NOT_INITIALIZED;
	}

	BSCluster* prev = NULL;
	BSCluster* node = file_system_t->cluster;
	while (NULL != node) {
		if ((node->block->corrupted == 0 && node->block->reserved == 0)) {
			if (node->block->free_state == BS_BLOCK_FREE) {
				return node;
			}
		}
		prev = node;
		node = node->next;
	}
	return NULL;
}

BSBlock* BSCLUSTER_get_block(BSFat* file_system_t, int index_t) {
	if (file_system_t->block_count < index_t) {
		return EXCEPTION_BS_CLUSTER_INDEX_OUT_OF_BOUNDS;
	}
	return file_system_t->cluster[index_t].block;
}

int BSCLUSTER_count_free_blocks(BSFat* file_system_t) { 
	
	if(NULL == file_system_t->cluster){
		return EXCEPTION_BS_CLUSTER_NOT_INITIALIZED;
	}

	int sum = 0;
	BSCluster* prev = NULL;
	BSCluster* node = file_system_t->cluster;
	while (NULL != node) {
		if (node->block->free_state == BS_BLOCK_FREE) {
			sum++;
		}
		prev = node;
		node = node->next;
	}
	return sum;
}

int BSCLUSTER_count_occupied_blocks(BSFat* file_system_t) { 

	if (NULL == file_system_t->cluster) {
		return EXCEPTION_BS_CLUSTER_NOT_INITIALIZED;
	}

	int sum = 0;
	BSCluster* prev = NULL;
	BSCluster* node = file_system_t->cluster;
	while (NULL != node) {
		if (node->block->free_state == BS_BLOCK_OCCUPIED) {
			sum++;
		}
		prev = node;
		node = node->next;
	}
	return sum;
}

int BSCLUSTER_count_reserved_blocks(BSFat* file_system_t) {

	if (NULL == file_system_t->cluster) {
		return EXCEPTION_BS_CLUSTER_NOT_INITIALIZED;
	}

	int sum = 0;
	BSCluster* prev = NULL;
	BSCluster* node = file_system_t->cluster;
	while (NULL != node) {
		if (node->block->reserved == BS_BLOCK_RESERVED) {
			sum++;
		}
		prev = node;
		node = node->next;
	}
	return sum;
}

int BSCLUSTER_count_corrupted_blocks(BSFat* file_system_t) { 

	if (NULL == file_system_t->cluster) {
		return EXCEPTION_BS_CLUSTER_NOT_INITIALIZED;
	}

	int sum = 0;
	BSCluster* prev = NULL;
	BSCluster* node = file_system_t->cluster;
	while (NULL != node) {
		if (node->block->corrupted == BS_BLOCK_CORRUPTED) {
			sum++;
		}
		prev = node;
		node = node->next;
	}
	return sum;
}

int BSCLUSTER_print(BSCluster* start_t) {
	if (NULL == start_t) {
		return EXCEPTION_BS_CLUSTER_INDEX_OUT_OF_BOUNDS;
	}
	
	BSCluster* prev = NULL;
	BSCluster* node = start_t;
	while ( NULL != node->next) {
		printf("Index: %i"
				"\nNext: %p"
				"\nPrevious: %p",
				node->index, node->next, node->prev);
		printf("\nBlocks: ");
		BSBLOCK_printBlock(node->block);
		printf("\n");
		prev = node;
		node = node->next;
	}
}

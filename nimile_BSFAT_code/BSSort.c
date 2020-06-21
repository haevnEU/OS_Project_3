#include "BSSort.h"


void swap(BSCluster* cluster1, BSCluster* cluster2) {
	BSCluster tmp;
	tmp.block = cluster1->block;
	cluster1->block = cluster2->block;
	cluster2->block = tmp.block;
}


void BSSORT_heap(BSCluster* cluster_t) {

}

void BSSORT_merge(BSCluster* cluster_t) {

}

void BSSORT_quick(BSCluster* cluster_t) {

}

void BSSORT_insertion(BSCluster* cluster_t) {

}

void BSSORT_bubble(BSCluster* cluster_t, int size_t) {
	for (int i = 0; i < size_t - 1; i++) {
		for (int j = 0; j < size_t - i - 1; j++) {
			if (cluster_t[j].block->corrupted == 1) {
				continue;
			}
			if (cluster_t[j].block->file_index < 0) {
				cluster_t[j].block->file_index = INVALID_FILE_INDEX;
				cluster_t[j].block->free_state = 0;
			}
			if (cluster_t[j + 1].block->file_index < 0) {
				cluster_t[j + 1].block->file_index = INVALID_FILE_INDEX;
				cluster_t[j + 1].block->free_state = 0;
			}
			if (cluster_t[j].block->file_index > cluster_t[j + 1].block->file_index) {
				swap(&cluster_t[j], &cluster_t[j + 1]);
			}
		}
	}
}

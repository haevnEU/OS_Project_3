#include "BSFat.h"

BSFat* BSFAT_createBSFat(int disk_size_t, int block_size_t) {
	if (disk_size_t < block_size_t) {
		return EXCEPTION_BS_FAT_SIZE_MISMATCH;
	}

	// Allocate memory
	BSFat* bs_fat = (BSFat*)malloc(sizeof(BSFat));
	if (NULL == bs_fat) {
		return EXCEPTION_BS_FAT_COULD_NOT_INITIALIZE;
	}
	// Set metadata
	bs_fat->disk_size = disk_size_t;
	bs_fat->block_count = disk_size_t / block_size_t;

	// Initialize a cluster
	bs_fat->cluster = BSCLUSTER_create_cluster(bs_fat->block_count);
	bs_fat->files = (BSFile*)malloc(sizeof(BSFile));
	for (int i = 0; i < MAX_FILES; i++) {
		bs_fat->files[i] = NULL;
	}
	return bs_fat;
}

void BSFAT_defrag(BSFat* file_system_t) {
	BSSORT_bubble(file_system_t->cluster, file_system_t->block_count);
	for (int i = 0; i < MAX_FILES; i++) {
		if (file_system_t->files[i] != NULL) {
			int indize = 0;
			for (int j = 0; j < file_system_t->block_count; j++) {
				if (file_system_t->cluster[j].block->file_index == file_system_t->files[i]->file_id) {
					file_system_t->files[i]->cluster_indize[indize] = j;
					indize++;
				}
			}
		}
	}
}

int BSFAT_insert_file(BSFat* file_system_t, BSFile* file_t) {
	for (int i = 0; i < MAX_FILES; i++) {
		if (file_system_t->files[i] == NULL) {
			file_system_t->files[i] = file_t;
			return EXCEPTION_NO;
		}
	}
	
	return EXCEPTION_BS_FAT_NO_FREE_FILES;
}

int BSFAT_get_free_file_index(BSFat* file_system_t) {
	for (int i = 0; i < MAX_FILES; i++) {
		if (file_system_t->files[i] == NULL) {
			return i;
		}
	}
	return -1;
}

int BSFAT_get_disk_size(BSFat* file_system_t) {
	return file_system_t->disk_size;
}

int BSFAT_get_block_count(BSFat * file_system_t) {
	return file_system_t->block_count;
}
	
int BSFAT_get_free_disk_space(BSFat * file_system_t) {
	int disk_size = file_system_t->disk_size;

	BSCluster* prev = NULL;
	BSCluster* node= file_system_t->cluster;
	while (NULL != node) {
		if (node->block->free_state == 1 || node->block->corrupted == 1 || node->block->reserved == 1) {
			disk_size -= BLOCK_SIZE;
		}
		node = node->next;
	}

	return disk_size;
}

float BSFAT_get_free_disk_space_percentage(BSFat * file_system_t) {
	int occupied = BSFAT_get_free_disk_space(file_system_t);
	return ((float)occupied / (float)file_system_t->disk_size) * 100;
}

int BSFAT_get_used_disk_space(BSFat * file_system_t) {
	int disk_size = 0;

	BSCluster* prev = NULL;
	BSCluster* node = file_system_t->cluster;
	while (NULL != node) {
		if (node->block->free_state == 1 || node->block->corrupted == 1 || node->block->reserved == 1) {
			disk_size += BLOCK_SIZE;
		}
		node = node->next;
	}

	return disk_size;
}

float BSFAT_get_used_disk_space_percentage(BSFat* file_system_t) {
	int occupied = BSFAT_get_used_disk_space(file_system_t);
	return ((float)occupied / (float)file_system_t->disk_size) * 100;
}

void BSFAT_show_fat(BSFat* file_system_t) {

	
	printf("\nAvailable/size: %i/%i(%.0lf%%)\nFragmentation: %.0lf%%\n",
		BSFAT_get_free_disk_space(file_system_t),
		file_system_t->disk_size, 
		BSFAT_get_free_disk_space_percentage(file_system_t), 
		BSFAT_fragmentation_percentage(file_system_t));

	BSCluster* node = file_system_t->cluster;
	while (NULL != node) {
		if (node->block->free_state == 1) {
			printf("| %i ", node->block->file_index);
		}
		else if (node->block->reserved == 1) {
			printf("| R ");
		}
		else if (node->block->corrupted == 1) {
			printf("| C ");
		}
		else {
			//printf("| % i", node->block->file_index);
			printf("| F ");
		}

		node = node->next;
	}

	printf("|\n");
}

void BSFAT_print(BSFat* file_system_t) {
	
	printf("START FILESYSTEM(DBG)"
		"\nDisksize: %i"
		"\nAvailable space: %i"
		"\nBlocksize: %i",
		file_system_t->disk_size, (file_system_t->disk_size - BSFAT_get_free_disk_space(file_system_t)), BLOCK_SIZE);
	
	BSFAT_show_fat(file_system_t);
		
	printf("\nSTART CLUSTER(dbg)\n");
	BSCLUSTER_print(file_system_t->cluster);
	printf("END CLUSTER(dbg)\n");
	printf("\nSTART FILES(dbg)\n");
	for (int i = 0; i < MAX_FILES; i++) {
		if (NULL != file_system_t->files[i]) {
			if (BSFILE_print(file_system_t->files[i]) == EXCEPTION_NO) {
				printf("\n\n");
			}
		}
	}
	printf("END FILES(dbg)\n");
	
	printf("\nEND OF FILESYSTEM(DBG)");
}

double BSFAT_fragmentation_percentage(BSFat* file_system_t) {
	double defrag = 0;
	int file_counter = 0;
	for (int i = 0; i < MAX_FILES; i++) {
		if (NULL != file_system_t->files[i]) {
			int blocks = file_system_t->files[i]->used_blocks;
			int sum = 0;
			for (int j = 0; j < blocks - 1; j++) {
				if (file_system_t->files[i]->cluster_indize[j + 1] - file_system_t->files[i]->cluster_indize[j] > 1) {
					sum++;
				}
				//sum += file_system_t->files[i]->cluster_indize[j];
			}
			defrag += (double)sum / (double)blocks;
		}
		file_counter++;
	}
	if (file_counter <= 0) {
		return 1;
	}
	return ((double)defrag / (double)file_counter) * 100;
}


//
//void wrapper(void (*sort)(BSCluster*), BSCluster* head) {
//	sort(head);
//}

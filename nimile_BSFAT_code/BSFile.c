#include "BSFile.h"



BSFile* BSFILE_create_file(BSFat* file_system_t, int file_size_t, const char name_t[], const char ext_t[], uint attributes_t, uint userID_t,int time_stamp_t, int date_stamp_t) {
	
	int filename_size = strlen(name_t);
	int extension_size = strlen(ext_t);
	int required_blocks = 0;
	int free_blocks = 0;

	// Determine if the filename and extension fullfill the requirements
	if (filename_size > 8 || extension_size > 3) {
		return EXCEPTION_BS_FILE_TO_LARGE_STRING;
	}


	BSFile* file = (BSFile*)malloc(sizeof(BSFile));

	// if malloc couldnt allocate memory stop create prozess
	if (NULL == file) {
		return EXCEPTION_BS_FILE_FILE_NOT_CREATED;
	}
	
	// Set metadata
	file->userID = userID_t;
	file->size = file_size_t;
	file->create_modify_time = time_stamp_t;
	file->create_modify_date =date_stamp_t;
	file->attributes &= BS_FILE_ALL;
	file->attributes |= (BS_FILE_FREE | attributes_t);
	file->file_id = BSFAT_get_free_file_index(file_system_t);
	for (int i = 0; i < 8; i++) {
		file->name[i] = 0;
		if (i < filename_size) {
			file->name[i] = name_t[i];
		}
	}
	
	for (int i = 0; i < 3; i++) {
		file->extension[i] = 0;
		if (i < extension_size) {
			file->extension[i] = ext_t[i];
		}
	}

	// Calculate how many blocks the file requires
	required_blocks = (1 + file_size_t) / BLOCK_SIZE;
	// Set thre required blocks to 1 if the file theoretical requires only 0 blocks
	required_blocks = required_blocks == 0 ? 1 : required_blocks;

	// Get the total available ammount of free blocks
	free_blocks = BSCLUSTER_count_free_blocks(file_system_t);

	// If the disk has not enough blocks return a error
	if (required_blocks > free_blocks) {
		return EXCEPTION_BS_FILE_NOT_ENOUGH_SPACE;
	}

	// Create the cluster_indize array
	file->cluster_indize = (int*)malloc(required_blocks * sizeof(int));
	
	// Assign cluster_inidze his values
	for (int i = 0; i < required_blocks; i++) {
		// Get the next available block from disk
		BSCluster* cluster = BSCLUSTER_get_next_cluster(file_system_t);
		// Set the state to occupied
		if (NULL == cluster) {
			return NULL;
		}
		cluster->block->free_state = 1;

		// Now create a link between file and the block cluster
		// Set cluster index to file ID
		cluster->block->file_index = file->file_id;
		// Set cluster_inize[i] to the cluster index
		file->cluster_indize[i] = cluster->index; 

	}
	file->used_blocks = required_blocks;
	BSFAT_insert_file(file_system_t, file);

	return file;
}

void BSFILE_delete_file(BSFat* file_system_t, BSFile* file_t) {

	if (NULL == file_system_t || NULL == file_t) {
		return;
	}

	// Calculate how many blocks are reserved for this file
	int blocks = sizeof(file_t->cluster_indize) / sizeof(file_t->cluster_indize[0]) + 1;
	
	// Iterate over the cluster
	BSCluster* node = file_system_t->cluster;
	int idx = 0;
	while (NULL != node) {
		// Determine if the file occupies the block
		if (node->index == file_t->cluster_indize[idx]) {
			// Set the state from the block to free
			node->block->free_state = 0;
			// Seth the file index from the block to something not possible
			node->block->file_index = INVALID_FILE_INDEX;
			idx++;
		}
		node = node->next;
	}

	// Delete the file
	file_system_t->files[file_t->file_id] = NULL;
	free(file_t);
	file_t = NULL;
}

int BSFILE_print(BSFile* file_t) {
	if (NULL == file_t) {
		return EXCEPTION_NOT_EXIST;
	}
	if (BSFILE_attributes(file_t, BS_FILE_FREE)) {
		return EXCEPTION_BS_FILE;
	}
	
	cstring name = BSFILE_name(file_t);
	uint file_size = BSFILE_file_size(file_t);
	uint occupied_blocks = BSFILE_occupied_blocks(file_t);
	uint attributes = BSFILE_attributes(file_t, BS_FILE_ALL);
	uint uid = BSFILE_user_id(file_t);

	printf("Filename: %s"
		   "\nSize: %i" 
		   "\nOccupied blocks: %i"
		   "\nAttributes: %i"
		   "\nUser ID: %i",
		name, file_size, occupied_blocks, attributes, uid);
	return EXCEPTION_NO;
}

int BSFILE_get_cluster_index(BSFile* file_t, int index_t){
	return file_t->cluster_indize[index_t];
}

void BSFILE_set_attribute(BSFile* file_t, uint mask_t) {
	file_t->attributes |= 1 << mask_t;
}

void BSFILE_set_attributes(BSFile* file_t, uint mask_t) {
	file_t->attributes |= mask_t;
}

void BSFILE_clear_attribute(BSFile* file_t, uint mask_t) {
	file_t->attributes &= ~(1 << mask_t);
}

void BSFILE_clear_attributes(BSFile* file_t, uint mask_t) {
	file_t->attributes &= mask_t;
}

cstring BSFILE_name(const BSFile* file_t) {
	if (NULL == file_t) {
		return EXCEPTION_NOT_EXIST;
	}
	cstring file_name = malloc(14 * sizeof(char));
	int idx = 0;
	for (int i = 0; i < 8; i++, idx++) {
		if (file_t->name[i] == 0) {
			break;
		}
		file_name[idx] = file_t->name[i];
	}
	file_name[idx] = '.';
	idx++;
	for (int i = 0; i < 3; i++, idx++) {
		if (file_t->extension[i] == 0) {
			break;
		}
		file_name[idx] = file_t->extension[i];
	}
	file_name[idx] = 0;
	return file_name;
}

uint BSFILE_file_size(const BSFile* file_t) {
	return  (NULL != file_t) ? file_t->size : EXCEPTION_NOT_EXIST;
}

uint BSFILE_occupied_blocks(const BSFile* file_t) {
	return (NULL != file_t) ? (file_t->size < BLOCK_SIZE) ? 1 : file_t->size / BLOCK_SIZE : EXCEPTION_NOT_EXIST;
}

uint BSFILE_file_id(const BSFile* file_t) {
	return (NULL != file_t) ? file_t->file_id : EXCEPTION_NOT_EXIST;
}

uint BSFILE_user_id(const BSFile* file_t) {
	return (NULL != file_t) ? file_t->userID : EXCEPTION_NOT_EXIST;
}

int BSFILE_attributes(const BSFile* file_t, int attribute_mask_t) {
	return (NULL != file_t) ? file_t->attributes & attribute_mask_t : EXCEPTION_NOT_EXIST;
}

uint BSFILE_last_modify_date(const BSFile* file_t) {
	return (NULL != file_t) ? file_t->create_modify_date : EXCEPTION_NOT_EXIST;
}

uint BSFILE_last_modify_time(const BSFile* file_t) {
	return (NULL != file_t) ? file_t->create_modify_time : EXCEPTION_NOT_EXIST;
}


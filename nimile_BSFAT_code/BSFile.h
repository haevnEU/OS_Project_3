#ifndef _BS_FILE_H_
#define _BS_FILE_H_

#include <stdlib.h>
#include <stdio.h>

#include "definitions.h"
#include "BSFat.h"
#include "BSCluster.h"

/*
 * This struct contains information about a file
 */
struct BSFile_t {
	/// 8 bit attribute
	/// +-------+-------+-------+-------+-------+-------+-------+-------+
	/// |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
	/// +-------+-------+-------+-------+-------+-------+-------+-------+
	/// | FREE  |  DEL  | MODI  |  DIR  |  SYS  |  HID  |  EXE  |   RO  |
	/// +-------+-------+-------+-------+-------+-------+-------+-------+
	uint attributes : 8;
	/// This attribute is the create oder modiy time
	/// Bit  0 -  5: Seconds
	/// Bit  6 - 10: Minutes
	/// Bit 11 - 15: Hours
	uint create_modify_time : 16;

	/// This attribute is the create oder modiy date
	/// Bit  0 -  5: Days
	/// Bit  6 - 10: Months
	/// Bit 11 - 15: Years(Starting from 1970)
	uint create_modify_date : 16;

	/// This attribute describes the owner of the file
	uint userID : 3;

	/// This attributes stores in which blocks the data is stored on the disk
	int* cluster_indize;

	/// This attribute is the file extension, Importan it is not NULL terminated
	char extension[3];

	/// This attribute is the file name, Importan it is not NULL terminated
	char name[8];

	/// This attribute is the file size
	uint size;

	/// This attribute is a file id
	uint file_id;

	/// This attributes describes how many blocks are occupied by this file
	uint used_blocks;
};

/*
 * This method creates a new file and assign it to a block cluster from the filesystem.
 * @param file_system_t BSFat* Filesystem where file should be stored
 * @param file_size_t int Size of the file
 * @param name_t const char[] Name of the file, the name must be less than 8 chars (excluding null termination)
 * @param ext_t const char[] Extension of the file the extension  must be less than 3 chars (excluding null termination)
 * @params attributes uint Attributes of the file
 * @param userID_t uint ID of the calling user
 * @param time_stamp_t int Timestamp of the file
 * @param data_stamp_t int Datestamp of the file, starting from 1970
 * @return New BSFile or NULL
 */
BSFile* BSFILE_create_file(BSFat* file_system_t, int file_size_t, const char name_t[], const char ext_t[], uint attributes_t, uint userID_t,int time_stamp_t, int date_stamp_t);

/**
 * This method deletes a file from the filesystem. 
 * This method frees the file corresponding cluster and assign NULL to the file.
 * @param file_system_t BSFat* FAT filesystem where the file is located
 * @param file_t BSFile* File which should be deleted
 */
void BSFILE_delete_file(BSFat* file_system, BSFile* file_t);

/**
 * This method prints information about the file.
 * @param file_t BSFile* File which should be printed
 * @return 0 iff no exception ocurred, otherwise corespoding errorcode 
 */
int BSFILE_print(BSFile* file_t);

/**
 * This mehtod get the index from the cluster where the file is stored
 * @param file_t BSFile* File which should be used
 * @param index_t int ...
 */
int BSFILE_get_cluster_index(BSFile* file_t, int index_t);

/**
 * This method sets an attribute of a file.
 * @param file_t BSFile* File where the attributes should be set
 * @param mask_t uint Position where the attribute is located
 */
void BSFILE_set_attribute(BSFile* file_t, uint mask_t);

/**
 * This method sets a couple of attributes from a file.
 * @param file_t BSFile* File where the attributes should be set
 * @param mask_t uint Position where the attributes are located
 */
void BSFILE_set_attributes(BSFile* file_t, uint mask_t);

/**
 * This method clears an attribute of a file.
 * @param file_t BSFile* File where the attributes should be set
 * @param mask_t uint Position where the attribute is located
 */
void BSFILE_clear_attribute(BSFile* file_t, uint mask_t);

/**
 * This method clears a couple of attributes from a file.
 * @param file_t BSFile* File where the attributes should be set
 * @param mask_t uint Position where the attributes are located
 */
void BSFILE_clear_attributes(BSFile* file_t, uint mask_t);

/**
 * This method return the filename.
 * @param file_t BSFile* file where the name should be retrieved
 * @return cstring filename
 */
cstring BSFILE_name(const BSFile* file_t);

/**
 * This method return the filesize.
 * @param file_t const BSFile* file where the name should be retrieved
 * @return uint Size of the file
 */
uint BSFILE_file_size(const BSFile* file_t);

/**
 * This method returns how many blocks are occupied.
 * @param file_t const BSFile* file where the name should be retrieve
 * @return uint Blocks which the file occupies
 */
uint BSFILE_occupied_blocks(const BSFile* file_t);

/**
 * This method returns the id of a file
 * @param file_t const BSFile* File which the id should be retrieved
 * @return uint ID of the file
 */
uint BSFILE_file_id(const BSFile* file_t);

/**
 * This method returns the userID of a file
 * @param file_t const BSFile* File which the id should be retrieved
 * @return uint ID of the file
 */
uint BSFILE_user_id(const BSFile* file_t);

/**
 * This method returns the attributes of a file
 * @param file_t const BSFile* File which the id should be retrieved
 * @return uint Attributes of the file
 */
uint BSFILE_attributes(const BSFile* file_t, int attribute_mask_t);

/**
 * This method returns the the date where the file was last modified. The Year must be added to 1970
 * @param file_t const BSFile* File which the id should be retrieved
 * @return uint Date where the file was last modified
 */
uint BSFILE_last_modify_date(const BSFile* file_t);

/**
 * This method returns the the time where the file was last modified.
 * @param file_t const BSFile* File which the id should be retrieved
 * @return uint Time where the file was last modified
 */
uint BSFILE_last_modify_time(const BSFile* file_t);

#endif // _BS_FILE_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h> 

#include "definitions.h"
#include "BSFat.h"
#include "BSFile.h"
#include "BSBlock.h"
#include "BSCluster.h"

int main(int argc, const char* argv[]) {
	BSFat* file_system = BSFAT_createBSFat(8192, 512);

	srand(time(0));
	int rnd;

	rnd = (rand() % 15);
	file_system->cluster[rnd].block->corrupted = 1;
	rnd = (rand() % 15);
	file_system->cluster[rnd].block->corrupted = 1;

	rnd = (rand() % 15);
	file_system->cluster[rnd].block->reserved = 1;
	printf("\nInital cluster");
	BSFAT_show_fat(file_system);

	BSFile* file1 = BSFILE_create_file(file_system, 1024, "tmp", "dbg", BS_FILE_HIDDEN, 0, 0, 0);
	BSFile* file2 = BSFILE_create_file(file_system, 512, "tmp", "dbg", BS_FILE_HIDDEN, 0, 0, 0);
	BSFile* file3 = BSFILE_create_file(file_system, 512, "tmp", "dbg", BS_FILE_HIDDEN, 0, 0, 0);
	BSFile* file4 = BSFILE_create_file(file_system, 512, "tmp", "dbg", BS_FILE_HIDDEN, 0, 0, 0);
	BSFile* file5 = BSFILE_create_file(file_system, 512, "tmp", "dbg", BS_FILE_HIDDEN, 0, 0, 0);
	BSFile* file6 = BSFILE_create_file(file_system, 512, "tmp", "dbg", BS_FILE_HIDDEN, 0, 0, 0);
	BSFile* file7 = BSFILE_create_file(file_system, 512, "tmp", "dbg", BS_FILE_HIDDEN, 0, 0, 0);
	BSFile* file8 = BSFILE_create_file(file_system, 512, "tmp", "dbg", BS_FILE_HIDDEN, 0, 0, 0);
	BSFile* file9 = BSFILE_create_file(file_system, 1024, "tmp", "dbg", BS_FILE_HIDDEN, 0, 0, 0);
	BSFile* file10 = BSFILE_create_file(file_system, 1024, "tmp", "dbg", BS_FILE_HIDDEN, 0, 0, 0);
	printf("\nAfter file creation");
	BSFAT_show_fat(file_system);

	BSFILE_delete_file(file_system, file2);

	BSFILE_delete_file(file_system, file6);
	BSFILE_delete_file(file_system, file9);
	printf("\nAfter file deletion");
	BSFAT_show_fat(file_system);

	BSFile* fileHUGE = BSFILE_create_file(file_system, 1025, "HUGEFILE", "big", BS_FILE_HIDDEN | BS_FILE_EXECUTABLE, 1, 0, 0);
	printf("\nAfter HUGE file creation");
	BSFAT_show_fat(file_system);

	BSFILE_delete_file(file_system, file1);
	BSFILE_delete_file(file_system, file3);
	BSFILE_delete_file(file_system, file4);
	BSFILE_delete_file(file_system, file5);
	BSFILE_delete_file(file_system, file7);
	BSFILE_delete_file(file_system, file8);
	printf("\nBefore defrag");
	BSFAT_show_fat(file_system);
	BSFAT_defrag(file_system);
	printf("\nAfter defrag");
	BSFAT_show_fat(file_system);
	BSFAT_show_fat(file_system);
	BSFAT_defrag(file_system);
	return 0;

}


#ifndef DISKUTIL_H
#define DISKUTIL_H

#include <string>
#include <fstream>
#include <iostream>

extern "C"{
    #include <unistd.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
}

#include "utils.h"
#include "disk.h"

/**
 * @brief The diskUtil class
 * @details This class allows basic interaction for a virtual disk file
 */
class diskUtil{

private attributes:

    /// This is the current disk
    Disk* disk;

    bool active = false;


public methods:

    /**
     * @brief diskUtil
     * @details Creates a new DiskUtils object
     */
    diskUtil();

    /**
     * @brief show
     * @details "Mainloop" of the DiskUtils module
     * @param disk This his the disk which the module operates on
     */
    void show(Disk* disk);


    /**
     * @brief createMBR
     * @details Invokes the submodule which handles the creation of a MBR
     */
    void createMBR();

    /**
     * @brief listMetaInformation
     * @details Invokes the submodule which handles the listing of meta information
     */
    void listMetaInformation();


    /**
     * @brief createPartition
     * @details Invokes the submodule which invokes the create partition submodule
     */
    void createPartition();
    /**
     * @brief removePartition
     * @details This method deletes a entry from the master boot record.
     *          During the process the user can also select a full partition wipe
     */
    void removePartition();

    /**
     * @brief openPartitionTool
     * @details Invokes the submodule which handles the ...
     */
    void openPartitionTool();

};

#endif // DISKUTIL_H

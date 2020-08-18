#ifndef PARTITION_H
#define PARTITION_H

#include "FileSystemType.h"
#include "utils/utils.h"


/**
 * @brief The Partition class
 * @details This class is a wrapper for a partition
 */
class Partition{

private attributes:
    /// Primary partition, should always true
    bool bootable;

    /// Type of the filesystem
    FileSystemType fileSystemType_m;

    /// Start address of the partition
    unsigned int startAddress_m;

    /// Size of the partition
    unsigned int size_m;

    uint32_t endAddress_m;

public getter:

    bool isPrimary() const;
    FileSystemType fileSystemType() const;
    uint32_t startAddress() const;
    uint32_t endAddress() const;
    uint32_t size() const;


public methods:

    /**
     * @brief Partition
     * @details Creates a new Partition with given arguments
     * @param startAddress Starting address of the partition
     * @param size Size of the partition
     * @param fileSystemType Filesystemtype
     * @param isBootAble Primary partition, should always be true
     */
    Partition(unsigned int startAddress, uint32_t endAddress, FileSystemType fileSystemType, bool isBootAble = true);


};

#endif // PARTITION_H

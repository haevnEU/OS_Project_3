#ifndef MASTERBOOTRECORD_H
#define MASTERBOOTRECORD_H

#include <iostream>

#include "partition.h"
#include "utils/utils.h"

#define MBR_FULL 205

/**
 * @brief The MasterBootRecord class
 * @details Wrapper class for the MBR of a virtual disk file
 */
class MasterBootRecord{

private attributes:

    /// Partition table of the MBR, only 4 entries are possilbe
    Partition* partitionTable[4];

public getter:

    /**
     * @brief partition
     * @details Returns the partition which correpsonds to the index
     * @param index Index where the partition is localted (index < 4)
     * @return Partition corresponding to the index
     */
    Partition* partition(int index);


public methods:

    /**
     * @brief MasterBootRecord
     * @details Creates a new Master Boot Record Wrapper
     */
    MasterBootRecord();

    /**
     * @brief addPartition
     * @details Adds a new partition to the Master Boot Record
     * @param partition Partition which should be added
     * @param index Index where the partition is localted (index < 4)
     */
    void addPartition(Partition* partition, int index);

    /**
     * @brief removePartition
     * @details Removes a partition from the MBR partition list
     * @param index Index where the partition is localted (index < 4)
     */
    void removePartition(int index);


    /**
     * @brief findFreePartition
     * @details Finds the next free spot inside the MBR partition table
     * @return next free spot or MBR_FULL
     */
    int findFreePartition();
};

#endif // MASTERBOOTRECORD_H

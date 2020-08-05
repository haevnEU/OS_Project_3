#ifndef DISK_H
#define DISK_H

#include <iostream>

extern "C"{
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
}

#include "utils/utils.h"
#include "progressbar.h"

#include "partition.h"
#include "FileSystemType.h"
#include "masterbootrecord.h"

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024  MB)

/**
 * @brief The Disk class
 * @details This class wraps a virtual disk file (vdf) as a virtual disk. Basic
 *          disk operation are provided by this class.
 */
class Disk{

private static_attributes:

    /// Constant, the offset where partition will be created
    const static uint32_t MBR_SIZE = 512;

private attributes:

    /// PAth to the virtual disk file
    char* path_m;

    /// Mapped data
    unsigned char* data_m;

    /// Size of the disk
    uint32_t size_m;

    /// If true operation can be done on this disk
    bool mounted = false;

    ///Mapps the Master Boot Record from @see data into an object
    MasterBootRecord* MBR_m;

    /// Information about the Virtual Disk File
    struct stat* fileStats_m;


public getter:

    const char* path();
    unsigned const char* data();
    MasterBootRecord* MBR();
    uint32_t size() const;
    bool isMounted() const;


public methods:

    /**
     * @brief Disk
     * @details Creates a new Disk object. The parameter @see{#path} determines the
     *          location of the virtual disk file.
     * @param path Path where the VDF is located.
     */
    explicit Disk(const char* path_m);

    ~Disk();

    /**
     * @brief createDisk
     * @details This methods creates a new empty virtual disk file. The size of the
     *          virtual disk is determined by @see{#size}
     * @param size Size of the disk in bytes
     */
    void createDisk(uint32_t size_m);

    void deleteDisk();


    /**
     * @brief mount This method mounts the virtual disk into memory.
     *          After mounting the disk is operable
     * @details The mounting process required a non mounted state, if the disk
     *          is allready mounted the operation fails.
     *          First of all a file descriptor to the virtual disk file (vdf) is
     *          opened. Next step is retreiving information about the virtual disk file,
     *          This is step is required to ensure that the whole disk is mapped. Finally
     *          the virtual disk file is mapped into the memory using mmap.
     *          If a MBR exists it will be loaded into the wrapper class.
     */
    void mount();

    /**
     * @brief unmount This method unmounts a virtuak disk from memory.
     *          After unmounint the disk can no longer be accessed.
     * @details Unmounting only works on mounted disks, if the disk is not mounted
     *          the operation fails. The c function mmap is called to unmap the virtual disk
     *          from memory, if the operation fails the application is usntable therefore it
     *          should never occurr
     */
    void unmount();


    /**
     * @brief loadMasterBootRecord This method loads a MBR from a virtua disk.
     * @details This operation required a mounted virtual disk and an existing MBR.
     *          This operation reads the MBR from a virtual disk and mapps it values into
     *          a wrapper class. If a partition is found it will also create the partition.
     */
    void loadMasterBootRecord();

    /**
     * @brief createMasterBootRecord This method writes the master boot record into the virtual disk.
     * @details A Master Boot Record (MBR) contains of different areas, all of the are written
     *          during the execution of the method. A Bootloader is simulated with the hex code
     *          0xB0 0x01 0xC0 0xDE, the bootlader is 496 bytes big. Next 4 partitionentries will
     *          be created, each entry is 16 byte big. At the end the magic number 0x55 0xAA will
     *          complete the MBR. This operation required that the disk is mounted. Executing
     *          this operation on an existing MBR will override everything as a result the
     *          data will no longer be accessable under normal circumstances.
     */
    void createMasterBootRecord();


    /**
     * @brief createPartition This method creates a new partition
     * @details This method requires a mounted disk, a MBR on the disk and a free partition spot.
     *          As first step the operation will accuired the index of a free spot. Next a the
     *          MBR wrapper will create a new Partition wrapper and assigns it. THe last step is
     *          updating the virtual disk file.
     * @param size Size of the partition in bytes
     * @param fileSystemType Type of the filesystem @see FSTypes
     * @param isPrimary Indicates if the partition is a primary one, should alway be true
     */
    void createPartition(uint32_t size_m, FileSystemType fileSystemType, bool isPrimary);

    /**
     * @brief removePartition
     * @details Removes the partition from the MBR.
     *          No data will be wiped, but the data cannot be accessed anymore on an easy way
     * @param index Partition which should be removed
     */
    void removePartition(uint8_t index);


private methods:

    /**
     * @brief calculateStartAddress
     * @details This method calculates the start address of the new partition.
     *          The new partition is layed direct behind the last one.
     *          If this is the first partition the size of the MBR plus 1 will be returned
     *          otherwise the size of the MBR plus end address of previous partition plus 1
     *          will be returned.
     * @param partitionIndex Index of the new partition, range 0 to 3
     * @return Startaddress of the partition.
     */
    uint32_t calculateStartAddress(uint8_t partitionIndex);

};

#endif // DISK_H

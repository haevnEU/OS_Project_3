#pragma once

// c++ includes
#include <vector>

// c++ equivalent includes from c 
#include <cstdint>

// c include
extern "C"{
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
}

// libraries

// project includes
#include "../utils/utils.h"
#include "partition.hpp"

namespace core::utilities{
    
    /**
     * @brief This is is utility class.
     * @details This class handles the operation with the pure virtual disk file. It will also contain all loaded partitions from all virtual disk files.
     * @version 1.0.0.0
     */
    class PartitionUtils{
    public:
        
        static PartitionUtils& getInstance(){
            static PartitionUtils instance;
            return instance;
        }
        
    private:
      

    private: 
        /**
         * @brief Construct a new Partition Utils object
         */
        PartitionUtils();

        /**
         * @brief This method invoke a wizard
         * @details This private method invokes the wizard to create a new partition on a virtual disk file.
         */
        void createNewPartition();
        
        /**
         * @brief This method invoke a wizard
         * @details This private method invokes the wizard to wipe a partition from a virtual disk file.
         */
        void wipeExistingPartition();
        
        /**
         * @brief This method invoke a wizard
         * @details This private method invokes the wizard to erase a partition from a virtual disk file.
         */
        void eraseExistingPartition();

    public:
        
        /**
         * @brief Destroy the Partition Utils object
         */
        ~PartitionUtils();

        /**
         * @brief Enters the tui
         * @details This method invokes the TUI of this class, it will work via the terminal interface.
         *          The private wrapper methods are called for the api usage.
         */
        void enter();

        /**
         * @brief Create a new Partition object
         * @details This methods maps the master boot from a virtual disk file and updates the partition table.
         * @param path Path to the virtual disk file
         * @param start Start address of the partition
         * @param end End address of the partition
         * @param file_system File system type of the partition
         */
        void createPartition(const char* path, uint64_t start, uint64_t end, int file_system);

        /**
         * @brief Wipes a partition
         * @details Wiping is insecure, it only deletes the partition entry from the master boot record. 
         * @param path Path to virtual disk file
         * @param definitition Definition of the partition
         */
        void wipePartition(const char* path, partition_definition* definitition);

        /**
         * @brief Erases a partition
         * @details Erasing is secure, it deletes the partition entry from the master boot record and override the content with zero. 
         * @param path Path to virtual disk file
         * @param definitition Definition of the partition
         */
        void erasePartition(const char* path, partition_definition* definitition);

    };
}
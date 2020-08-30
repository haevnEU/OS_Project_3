#pragma once

#include <vector>

#include "../utils/utils.h"

#include "virtualDiskFile.hpp"
#include "partition.hpp"

namespace core::utilities{
    
    /**
     * @brief This is is utility class.
     * @details This class handles the operation with the pure virtual disk file. It will also contain all loaded partitions from all virtual disk files.
     * @version 1.0.0.0
     */
    class DiskUtils{
    public:
        
        static DiskUtils& getInstance(){
            static DiskUtils instance;
            return instance;
        }
        
    private:
        /**
         * @brief This attribute contains all laoded partitions from any virtual disk
         */
        std::vector<core::Partition*>* partition_definitions;

        std::string last_result_message;
    private: 
        /**
         * @brief Construct a new Disk Utils object
         */
        DiskUtils();

        /**
         * @brief This method invoke a wizard
         * @details This private method invokes the wizard to create a new virtual disk file.
         */
        void createNewVirtualDiskFile();
        
        /**
         * @brief This method invoke a wizard
         * @details This private method invokes the wizard to load an existing virtual disk file.
         */
        void loadExistingVirtualDiskFile();
        
        /**
         * @brief This method invoke a wizard
         * @details This private method invokes the wizard to verify an existing virtual disk file.
         */
        void verifyVirtualDiskFile();

    public:
        
        /**
         * @brief Destroy the Disk Utils object
         */
        ~DiskUtils();

        /**
         * @brief Enters the tui
         * @details This method invokes the TUI of this class, it will work via the terminal interface.
         *          The private wrapper methods are called for the api usage.
         */
        void enter();

        /**
         * @brief This method creates a new virtual disk file
         * @param path Full path, including file name, to the virtual disk file
         * @param size Size of the virtual disk file
         */
        void createDisk(const char* path, uint64_t size);

        /**
         * @brief Verifies a virtual disk file.
         * @details This method does a couple steps. First step is to ensure the path is valid. Next step is to verify the extension
         *          The last step is to ensure that the virtual disk file is compatible with this code, doing so the file will
         *          be mapped into memory and searched for the version check at the end, if its found the version is checked against the
         *          project ID and code version.
         * @param path Path to the virtual disk file
         * @return True Iff the virtual disk file is a valid one.
         */
        bool verifyDisk(const char* path);

        /**
         * @brief This methods loads the content of the master boot record
         * @details Important the disk is not fully loaded into memory. This method checks the master boot record
         *          and creates a new virtual_disk_file definition where the partitions from will be added
         *          to the partition_definitions object.
         * @param path Path to the virtual disk file
         */
        void loadDisk(const char* path);

        /**
         * @brief This methods returns all partition which are loaded via the loadDisk command.
         * @return std::vector<core::partition_definition*>* 
         */
        std::vector<core::Partition*>* loadedPartitions(){
            return partition_definitions;
        }

    };
}
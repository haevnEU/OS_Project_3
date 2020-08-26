#pragma once

#include "../utils/utils.h"

#include "partition.hpp"
#include "DiskUtils.hpp"

namespace core::utilities{
    
    /**
     * @brief This is is utility class.
     * @details This class handles the operation with a master boot record of a virtual disk file.
     * @version 1.0.0.0
     */
    class MasterBootRecordUtils{
    public:
        static MasterBootRecordUtils& getInstance(){
            static MasterBootRecordUtils instance;
            return instance;
        }    

    private:
        /**
         * @brief Construct a new Master Boot Record Utils object
         * 
         */
        MasterBootRecordUtils();
        
        /**
         * @brief This method is a wrapper for another methods, this is a wizard
         * @details This private method invokes the wizard to create a master boot record on a virtual disk file.
         */
        void createMBR();

        /**
         * @brief This method is a wrapper for another methods, this is a wizard
         * @details This private method invokes the wizard to wipe or erase a master boot record from a virtual disk file.
         */
        void formatMBR();


        /**
         * @brief This method is a wrapper for another methods, this is a wizard
         * @details This private method invokes the wizard to inspect a master boot record from a virtual disk file.
         */
        void inspectMBR();

    public:
        /**
         * @brief Destroy the Master Boot Record Utils object
         */
        ~MasterBootRecordUtils();

        /**
         * @brief Enters the tui
         * @details This method invokes the TUI of this class, it will work via the terminal interface.
         *          The private wrapper methods are called for the api usage.
         */
        void enter();

        /**
         * @brief Creates a new master boot record on a virtual disk file
         * @param path path to virtual disk file
         */
        void createMasterBootRecord(const char* path);

        /**
         * @brief Wipes a master boot record from a virtual disk file
         * @details Note wiping is insecure, it will only override the master boot record.
         * @param path path to virtual disk file
         */
        void wipeMasterBootRecord(const char* path);

        /**
         * @brief Wipes a master boot record from a virtual disk file
         * @details Note erasing is secure, it will override the master boot record and zeros the partition.
         * @param path path to virtual disk file
         */
        void eraseMasterBootRecord(const char* path);
    };
}
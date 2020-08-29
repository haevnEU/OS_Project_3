#pragma once

#include "../utils/utils.h"
#include "partition.hpp"

/*!
 *  \addtogroup core
 *  @{
 */

//! Virtual Disk File implementations
namespace core::disk{

    /**
     * @brief Contains the definition of a virtual disk file.
     */
    struct virtual_disk_file{
        /**
         * @brief Path of the virtual disk file
         */
        const char* path;

        /**
         * @brief First partition of the virtual disk file
         */
        core::partition_definition* partition_1;

        /**
         * @brief Second partition of the virtual disk file
         */
        core::partition_definition* partition_2;

        /**
         * @brief Third partition of the virtual disk file
         */
        core::partition_definition* partition_3;

        /**
         * @brief Fourth partition of the virtual disk file
         */
        core::partition_definition* partition_4;
    };

    /**
     * @brief This is a deprecated typedef
     * @deprecated 
     */
    typedef virtual_disk_file master_boot_record;

    /**
     * @brief This static method loads a master boot record from a virtual disk file.
     * @details This method opens the virtual disk file and maps the content into memory. After the mapping
     *          the virtual disk file is closed again and the mapped data is inspected. Finally the inspected
     *          data is unmapped from the memory.
     * @param disk_path Path to the virtual disk file
     * @return virtual_disk_file* Definition of the virtual disk file
     */
    static virtual_disk_file* load_master_boot_record(const char* disk_path){
        virtual_disk_file* mbr = new virtual_disk_file();
        
    
        auto fd = open(disk_path, O_RDWR, S_IRUSR | S_IWUSR);
        if(-1 == fd){
            return nullptr;
        }
        uint8_t* data = static_cast<uint8_t*>(mmap(nullptr,  0x1FF, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
        if(MAP_FAILED == data){
            close(fd);
            return nullptr;
        }
        close(fd);
        
        for(int index = 0; index < 4; index++){
            core::partition_definition* definition = new core::partition_definition();
            

            if(data[0x1BE + (index * 16)] > 0x00){
                // If the bootable flag is 0x80 this is a primary partition
                definition->primary = data[0x1BE + (index * 16)];
                definition->file_system_type = data[0x1BF + (index * 16)];
                // Calculate the start and end address
                uint32_t start = 0;
                start |= (uint32_t)data[0x1C0 + (index * 16)];
                start |= (uint32_t)data[0x1C1 + (index * 16)] << 8;
                start |= (uint32_t)data[0x1C2 + (index * 16)] << 16;
                start |= (uint32_t)data[0x1C3 + (index * 16)] << 24;

                uint32_t end = 0;
                end |= (uint32_t)data[0x1C4 + (index * 16)];
                end |= (uint32_t)data[0x1C5 + (index * 16)] << 8;
                end |= (uint32_t)data[0x1C6 + (index * 16)] << 16;
                end |= (uint32_t)data[0x1C7 + (index * 16)] << 24;
                
                definition->start_address = start;
                definition->end_address = end;


                uint16_t block_size = 0;
                uint64_t cluster_size = 0;

                
                cluster_size |= (uint32_t)data[0x1C8 + (index * 16)];
                cluster_size |= (uint32_t)data[0x1C9 + (index * 16)] << 8;
                cluster_size |= (uint32_t)data[0x1CA + (index * 16)] << 16;
                cluster_size |= (uint32_t)data[0x1CB + (index * 16)] << 24;

                block_size |= (uint8_t)data[0x1CC + (index * 16)];
                block_size |= (uint8_t)data[0x1CD + (index * 16)] << 8;

                definition->block_size = block_size;
                definition->cluster_size = cluster_size;

                definition->path = new char[strlen(disk_path) + 1];
                memcpy(definition->path, disk_path, strlen(disk_path) + 1);

            }else{
                definition = nullptr;
            }
            if(0 == index){
                mbr->partition_1 = definition;
            }else if(1 == index){
                mbr->partition_2 = definition;
            }else if(2 == index){
                mbr->partition_3 = definition;
            }else if(3 == index){
                mbr->partition_4 = definition;
            }
        }

        munmap(data, 0x1FF);
        return mbr;
    }
} // End namespace disk

/*! @} End of Doxygen Groups*/

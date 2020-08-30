#pragma once

#include "../utils/utils.h"
#include "partition.hpp"

namespace core::file_allocation_table{
    
    /**
     * @brief This struct represents a bloc of a cluster      
     */
    struct block{
        /**
         * @brief The actual data from the block
         */
        uint8_t* data;
        /**
         * @brief The next block address
         */
        uint32_t next_address;
    };
    
    /**
     * @brief This is a cluster
     */
    struct content{
        /**
         * @brief Starting block
         */
        block* start;
        /**
         * @brief Size of the file
         */
        uint64_t size;
    };

    /**
     * @brief This is the FAT table entry in memory 
     */
    struct root_table{
        /**
         * @brief Name of the file
         */
        char name[8];

        /**
         * @brief Extension of the file
         */
        char extension[3];

        bool read_only;
        bool hidden;
        bool system_file;
        bool special_entry;
        bool sub_directory;
        bool archive;
        
        uint16_t last_access_time;
        uint16_t last_access_date;
        uint32_t file_size;

        /**
         * @brief Start address inside the virtual disk file
         */
        uint32_t start_address;

    };

    /**
     * @brief This class impelements a partition as GENERIC FAT format 
     */
    class FATFileSystem : public Partition{

    private:
        /**
         * @brief Internal instance counter 
         */
        static int counter;
        /**
         * @brief Size of the used FAT table, this limits the total amount of files
         */
        const static uint64_t TABLE_SIZE = 100;

        /**
         * @brief FAT table
         */
        root_table* fat_table[TABLE_SIZE];
        

        /**
         * @brief This methods updates the fat table in memory
         */
        void update_fat_table();

        /**
         * @brief This methods checks if a file can be created
         * @return True iff the FAT table has a free spot
         */
        bool inline has_free_spot(){
            return get_free_table_entry() != -1;
        }

        /**
         * @brief Gets a free address from the fat table
         * @return uint32_t Starting address at the virtual disk file
         */
        uint32_t inline get_free_table_entry(){
            for(uint32_t i = 0; i < TABLE_SIZE; i++){
                if(fat_table[i]->name[0] == 0){
                    return i;
                }        
            }
            return -1;
        }
    public:

        /**
         * @brief Toggles the mounting state of the disk
         */
        void inline toggle_mount(){
            if(isMounted()){
                unmount();
            }else{
                mount();
            }
        }

        /**
         * @brief Construct a new FATFileSystem object
         * @param definition Definition of the partition
         */
        FATFileSystem(partition_definition* definition);

        ~FATFileSystem();

        /**
         * @brief Unmounts the partition
         */
        void mount();

        /**
         * @brief Get the fragmentation as percentage in range of 0 to 100
         * @return uint8_t Fragmentation 
         */
        uint8_t get_fragmentation();

        /**
         * @brief Defrags the partition
         */
        void fragment();

        /**
         * @brief Get the FAT table entry for a give file
         * @param name Name of the file
         * @param extension Extension of the file
         * @return uint64_t Entry inside the FAT tabler
         */
        uint64_t get_table_entry(const char name[8], const char extension[3]);
        
        /**
         * @brief Get the start address of a file inside the virtual disk file
         * @param name Name of the file
         * @param extension Extension of the file
         * @return uint32_t Start address
         */
        uint32_t get_start_address(const char name[8], const char extension[3]);

        /**
         * @brief Get the total free space
         * @return uint64_t Free space
         */
        uint64_t get_available_space();

        /**
         * @brief Get the total space
         * @return uint64_t total space
         */
        uint64_t get_total_space();

        /**
         * @brief Get the total used space
         * @return uint64_t Used space
         */
        uint64_t get_used_space();

        /**
         * @brief Checks if a file exists on the partition
         * @param name Filename
         * @param extension File extension
         * @return True iff the FAT table founds an entry
         */
        bool file_exists(const char name[8], const char extension[3]);

        /**
         * @brief Reads a file from disk
         * @param name Name of the file
         * @param extension Extension of the file
         * @return content* File content
         */
        content* read_file(const char name[8], const char extension[3]);

        /**
         * @brief Writes data to the virtual disk file
         * @param name Name of the file
         * @param extension Extension of the file 
         * @param file_data Data which should be written
         * @param file_size Size of the file
         */
        void write_file(const char name[8], const char extension[3], uint8_t* file_data, uint64_t file_size);

        /**
         * @brief Deletes a file from disk
         * @param name Name of the file
         * @param extension Extension of the file
         */
        void delete_file(const char name[8], const char extension[3]);

        /**
         * @brief Get the meta information about a file
         * @param name Name of the file
         * @param extension Extension of the file
         * @return root_table Meta information about the file
         */
        root_table* get_meta_data(const char name[8], const char extension[3]);

        /**
         * @brief Shows the file allocation table as a raw format
         */
        void show_file_allocation_table();
    };

}
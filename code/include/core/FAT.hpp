#pragma once

#include "../utils/utils.h"
#include "partition.hpp"

namespace core::file_allocation_table{
    
    struct block{
        uint8_t* data;
        uint32_t next_address;
    };
    
    struct content{
        uint8_t* data;
        uint64_t size;
    };

    struct root_table{
        char name[8];
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

        uint32_t start_address;

    };

    class FATFileSystem : public Partition{

    private:
        static int counter;
        const static uint64_t TABLE_SIZE = 100;

        root_table* fat_table[TABLE_SIZE];
        
        
        void update_fat_table();

        bool inline has_free_spot(){
            return get_free_table_entry() != -1;
        }

        uint32_t inline get_free_table_entry(){
            for(uint32_t i = 0; i < TABLE_SIZE; i++){
                if(fat_table[i]->name[0] == 0){
                    return i;
                }        
            }
            return -1;
        }
    public:

        void inline toggle_mount(){
            if(isMounted()){
                unmount();
            }else{
                mount();
            }
        }

        FATFileSystem(partition_definition* definition);

        ~FATFileSystem();

        void mount();

        uint8_t get_fragmentation();

        void fragment();

        uint64_t get_table_entry(const char name[8], const char extension[3]);
        uint32_t get_start_address(const char name[8], const char extension[3]);

        uint64_t get_available_space();

        uint64_t get_total_space();

        uint64_t get_used_space();

        bool file_exists(const char name[8], const char extension[3]);

        content* read_file(const char name[8], const char extension[3]);

        void write_file(const char name[8], const char extension[3], uint8_t* file_data, uint64_t file_size);

        void delete_file(const char name[8], const char extension[3]);

        root_table* get_meta_data(const char name[8], const char extension[3]);

        void show_file_allocation_table();
    };

}
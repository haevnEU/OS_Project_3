#include "../../include/core/FAT.hpp"

using namespace core::file_allocation_table;
    int FATFileSystem::counter = 0;
FATFileSystem::FATFileSystem(partition_definition* definition) : Partition(definition){
    counter++;
}

FATFileSystem::~FATFileSystem(){
    for(uint64_t i= 0; i < TABLE_SIZE; i++){
        delete fat_table[i];
    }
    counter--;
}

void FATFileSystem::mount(){
    // 1. mount the partition
    Partition::mount();

    // 2. When the partition is mounted create the FAT Entries
    //      The partition table lays on position 0. 
    //      One entry is 32 bytes big
    for(uint64_t i = 0; i < TABLE_SIZE; i++){
        fat_table[i] = new root_table();
        // 1. read the name
        //      Read byte 0 to 7 from the entry
        for(int name_index = 0; name_index < 8; name_index++){
            
            fat_table[i]->name[name_index] = getByte(name_index + (i * 32));
        }
        // 2. read the extension
        //      Read byte 8 to 11 from entry
        for(int extension_index = 0; extension_index < 3; extension_index++){
            fat_table[i]->extension[extension_index] = getByte(8 + extension_index + (i * 32));
        }        
        // 3. read the attributes
        //      Read byte 12
        // = data_m[12];
        // 4. read the creation/modify time
        //      Read bytes 13 and 14
        // 5. read the creation/modify date
        uint32_t date = 0;
        date |= (uint32_t)getByte(13 + (i * 32));
        date |= (uint32_t)getByte(14 + (i * 32)) << 8;
        fat_table[i]->last_access_date = date;
        //      Read bytes 15 and 16
        uint32_t time = 0;
        time |= (uint32_t)getByte(15 + (i * 32));
        time |= (uint32_t)getByte(16 + (i * 32)) << 8;
        fat_table[i]->last_access_time = time;
        // 6. read the filesize
        //      Read bytes 17, 18, 19 and 20
        uint32_t size = 0;
        size |= (uint32_t)getByte(17 + (i * 32));
        size |= (uint32_t)getByte(18 + (i * 32)) << 8;
        size |= (uint32_t)getByte(19 + (i * 32)) << 16;
        size |= (uint32_t)getByte(20 + (i * 23)) << 24;
        fat_table[i]->file_size = size;
        // 7. read the start address
        //      Read bytes 21, 22, 23, 24
        uint32_t start = 0;
        start |= (uint32_t)getByte(21 + (i * 32));
        start |= (uint32_t)getByte(22 + (i * 32)) << 8;
        start |= (uint32_t)getByte(23 + (i * 32)) << 16;
        start |= (uint32_t)getByte(24 + (i * 23)) << 24;
        fat_table[i]->start_address = start;       
    }
}


uint8_t FATFileSystem::get_fragmentation(){
    return 0;
}

void FATFileSystem::fragment(){

}

uint64_t FATFileSystem::get_available_space(){
    return 0;
}

uint64_t FATFileSystem::get_total_space(){
    return 0;
}

uint64_t FATFileSystem::get_used_space(){
    return 0;
}



uint64_t FATFileSystem::get_table_entry(const char name[8], const char extension[3]){
    uint32_t address = 0;
    for(int i = 0; i < TABLE_SIZE; i++){
        std::cout << "Checking: " << std::hex << i << std::dec << std::endl;
        if(extension[0] == fat_table[i]->extension[0] && 
            extension[1] == fat_table[i]->extension[1] && 
            extension[2] == fat_table[i]->extension[2] &&
            name[0] == fat_table[i]->name[0] &&
            name[1] == fat_table[i]->name[1] &&
            name[2] == fat_table[i]->name[2] &&
            name[3] == fat_table[i]->name[3] &&
            name[4] == fat_table[i]->name[4] &&
            name[5] == fat_table[i]->name[5] &&
            name[6] == fat_table[i]->name[6] &&
            name[7] == fat_table[i]->name[7]){ 
                return i;
        }
    }
    return -1;
}

uint32_t FATFileSystem::get_start_address(const char name[8], const char extension[3]){
    uint32_t address = 0;
    for(int i = 0; i < TABLE_SIZE; i++){
        if(extension[0] == fat_table[i]->extension[0] && 
            extension[1] == fat_table[i]->extension[1] && 
            extension[2] == fat_table[i]->extension[2] &&
            name[0] == fat_table[i]->name[0] &&
            name[1] == fat_table[i]->name[1] &&
            name[2] == fat_table[i]->name[2] &&
            name[3] == fat_table[i]->name[3] &&
            name[4] == fat_table[i]->name[4] &&
            name[5] == fat_table[i]->name[5] &&
            name[6] == fat_table[i]->name[6] &&
            name[7] == fat_table[i]->name[7]){
                return fat_table[i]->start_address;
        }
    }
    return address;
}

bool FATFileSystem::file_exists(const char name[8], const char extension[3]){
    return get_start_address(name, extension) > 0;
}

content* FATFileSystem::read_file(const char name[8], const char extension[3]){
    return nullptr;
}

void FATFileSystem::write_file(const char name[8], const char extension[3], uint8_t* file_data, uint64_t file_size){
    uint32_t free_entry = get_free_table_entry();
    if(free_entry == -1){
        return;
    }

    for(int i = 0; i < 8; i++){
        fat_table[free_entry]->name[i] = name[i];
    }
    for(int i = 0; i < 3; i++){
        fat_table[free_entry]->extension[i] = extension[i];
    }
    update_fat_table();
    unmount();
    mount();
}

void FATFileSystem::delete_file(const char name[8], const char extension[3]){
    uint64_t address = get_table_entry(name, extension);
    std::cout << "Deleting from 0x" << std::hex << address << std::dec << std::endl;  
    
    if(address == -1){
        return;
    }

    for(int i = 0; i < 8; i++){
        fat_table[address]->name[i] = 0;
    }
    for(int i = 0; i < 3; i++){
        fat_table[address]->extension[i] = 0; 
    }
    update_fat_table(); 
    unmount();
    mount(); 
}

root_table* FATFileSystem::get_meta_data(const char name[8], const char extension[3]){
    return nullptr;
}

void FATFileSystem::show_file_allocation_table(){
    std::cout << "File Allocation Table content" << std::endl;
    for(int i = 0; i < TABLE_SIZE; i++){
        if(fat_table[i] == nullptr){
            continue;
        }
        if(fat_table[i]->name[0] != 0){
            std::cout << fat_table[i]->name[0] << fat_table[i]->name[1] << fat_table[i]->name[2] << fat_table[i]->name[3] << fat_table[i]->name[4] << fat_table[i]->name[5] << fat_table[i]->name[6] << fat_table[i]->name[7]
             << "." << fat_table[i]->extension[0] << fat_table[i]->extension[1] << fat_table[i]->extension[2] << ": Last modified["<< fat_table[i]->last_access_date << ":" << fat_table[i]->last_access_time << "] Startaddress [" << std::hex << "0x" << fat_table[i]->start_address << "/" <<  "0x" << (512 + fat_table[i]->start_address) << std::dec << "]" << std::endl;
        }
    }
}

void FATFileSystem::update_fat_table(){
    for(uint32_t index = 0; index < TABLE_SIZE; index++){
        for(int name_index = 0; name_index < 8; name_index++){
            setByte(name_index + (index * 32),fat_table[index]->name[name_index]);    
        }
        for(int extension_index = 0; extension_index < 3; extension_index++){
            setByte(extension_index + (index * 32) + 8, fat_table[index]->extension[extension_index]);    
        }        
        setByte((12 + (index * 32)), fat_table[index]->last_access_date & 0xFF);
        setByte((13 + (index * 32)), (fat_table[index]->last_access_date >> 8) & 0xFF);

        
        setByte((15 + (index * 32)), fat_table[index]->last_access_time & 0xFF);
        setByte((16 + (index * 32)), (fat_table[index]->last_access_time >> 8) & 0xFF);
        
        setByte((17 + (index * 32)), fat_table[index]->file_size & 0xFF);
        setByte((18 + (index * 32)), (fat_table[index]->file_size >> 8) & 0xFF);
        setByte((19 + (index * 32)), (fat_table[index]->file_size >> 16) & 0xFF);
        setByte((20 + (index * 32)), (fat_table[index]->file_size >> 24) & 0xFF);
    
        setByte((21 + (index * 32)), fat_table[index]->start_address & 0xFF);
        setByte((22 + (index * 32)), (fat_table[index]->start_address >> 8) & 0xFF);
        setByte((23 + (index * 32)), (fat_table[index]->start_address >> 16) & 0xFF);
        setByte((24 + (index * 32)), (fat_table[index]->start_address >> 24) & 0xFF);

    }
}

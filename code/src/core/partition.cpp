#include "../include/core/partition.hpp"

using namespace core;

Partition::Partition(partition_definition* definition){ 
    definition = definition;
    this->path = new char[strlen(path) + 1];
    memcpy(this->path, path, strlen(path) + 1);
}

Partition::~Partition(){
    unmount();
    delete definition;
    delete[] path;
    delete[] data_m;
}

void Partition::mount(){
    if(isMounted()){
        return;
    }
    if(nullptr == definition){
        error_handler(partition_codes::no_definition);
        return;
    }
    if(!utils::file::exists(path)){
        handle_error(partition_codes::file_not_exist);
        return;
    }
    auto fd = open(path, O_RDWR, S_IRUSR | S_IWUSR);
    if(-1 == fd){
        handle_error(partition_codes::file_open_failure);
        return;
    }
    
    size = definition->end_address - definition->start_address;
    data_m = static_cast<uint8_t*>(mmap(nullptr,  static_cast<size_t>(size), PROT_READ | PROT_WRITE, MAP_SHARED, fd, definition->start_address));
    if(MAP_FAILED == data_m){
        error_handler(partition_codes::mapping_failure);
        close(fd);
        return;
    }

    mounted = true;
}

void Partition::unmount(){
    if(!isMounted()){
        return;
    }
    if(0 != munmap(data_m,  static_cast<size_t>(size))){ 
        error_handler(partition_codes::mapping_failure);
        return;
    }

    mounted = false;
}

void Partition::setByte(uint64_t address, uint8_t data){
    if(!isMounted()){
        error_handler(partition_codes::partition_not_mounted);
        return;
    }
    if(validate_address(address && validate_data())){
        
    }
}

void Partition::clearByte(uint64_t address){
    if(!isMounted()){
        error_handler(partition_codes::partition_not_mounted);
        return;
    }
    if(validate_address(address) && validate_data()){

    }
}

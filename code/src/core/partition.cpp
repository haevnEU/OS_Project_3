#include "../include/core/partition.hpp"

using namespace core;

    int Partition::counter = 0;
Partition::Partition(partition_definition* definition){ 
    this->definition = definition;
    std::cout << "Created: " << counter << std::endl;
}

Partition::~Partition(){
    std::cout << "DTOR" << std::endl;
    unmount();
    delete[] definition->path;
    delete definition;
    std::cout << "Remaining: " << counter << std::endl;
}

void Partition::mount(){
    std::cout << "Mounting partition" << std::endl;
    if(isMounted()){
        return;
    }
    std::cout << "Verify definition" << std::endl;
    if(nullptr == definition){
        handle_error(partition_codes::no_definition);
        return;
    }
    std::cout << "Verify path" << std::endl;
    if(!utils::file::exists(definition->path)){
        handle_error(partition_codes::file_not_exist);
        return;
    }
    
    std::cout << "Open disk" << std::endl;
    auto fd = open(definition->path, O_RDWR, S_IRUSR | S_IWUSR);
    if(-1 == fd){
        handle_error(partition_codes::file_open_failure);
        return;
    }
    
    size = definition->end_address - definition->start_address;

    std::cout << "Map partition" << std::endl;
    std::cout << definition->start_address << std::endl;
    data_m = static_cast<uint8_t*>(mmap(nullptr,  static_cast<size_t>(size), PROT_READ | PROT_WRITE, MAP_SHARED, fd, ((definition->start_address / 4096) * 4096)));

sysconf(_SC_PAGE_SIZE);

    close(fd);
    if(MAP_FAILED == data_m){   
        handle_error(partition_codes::mapping_failure);
        close(fd);
        return;
    }
    std::cout << "MOUNTED" << std::endl;
    std::cout << (int) getByte(0) << " " << getByte(0) << std::endl;
    wait(std::cin);
    mounted = true;
}

void Partition::unmount(){
    if(!isMounted()){
        return;
    }
    if(0 != munmap(data_m,  static_cast<size_t>(size))){ 
        handle_error(partition_codes::mapping_failure);
        return;
    }

    mounted = false;
}

void Partition::setByte(uint64_t address, uint8_t data){
    if(!isMounted()){
        handle_error(partition_codes::partition_not_mounted);
        return;
    }
    if(validate_address(address) && validate_data()){
    }
        data_m[512 + address] = data;
}

void Partition::clearByte(uint64_t address){
    if(!isMounted()){
        handle_error(partition_codes::partition_not_mounted);
        return;
    }
    if(validate_address(address) && validate_data()){

    }
}

void Partition::showPartition(uint64_t start, uint64_t end){
    if(end == -1){
        end = definition->end_address;
    }

    if(!isMounted()){
        handle_error(partition_codes::partition_not_mounted);
        return;
    }

    std::cout << start << std::endl
    << start << std::endl;
    for(uint64_t index = start; index < end; index++){
        if(index > 0)
        if(index % 16 == 0){
            std::cout << std::endl << utils::colors::RESET << std::setfill('0') << std::setw(8) << std::hex << index << utils::colors::UNDERLINE<< '\t' << utils::colors::RESET;
        }else if(index % 8 == 0){
            std::cout << "  ";
        }
        if(index <= 0x1BD){
            std::cout << utils::colors::BACK_MAGENTA << std::setfill('0') << std::setw(2) << std::hex << (int)getByte(index) << ' ';
        }else if(index <= (0x1BD + 16)){
            std::cout <<  utils::colors::BACK_CYAN << std::setfill('0') << std::setw(2) << std::hex << (int)getByte(index) << ' ';
        }else if(index <= (0x1BD + 32)){
            std::cout <<  utils::colors::BACK_GREEN << std::setfill('0') << std::setw(2) << std::hex << (int)getByte(index) << ' ';
        }else if(index <= (0x1BD + 48)){
            std::cout <<  utils::colors::BACK_CYAN << std::setfill('0') << std::setw(2) << std::hex << (int)getByte(index) << ' ';
        }else if(index <= (0x1BD + 64)){
            std::cout <<  utils::colors::BACK_GREEN << std::setfill('0') << std::setw(2) << std::hex << (int)getByte(index) << ' ';
        }else{
            std::cout <<  utils::colors::BACK_MAGENTA << std::setfill('0') << std::setw(2) << std::hex << (int)getByte(index) << ' ';    
        }
    }
}
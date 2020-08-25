#include "../../include/core/PartitionUtils.hpp"

using namespace core::utilities;


PartitionUtils::PartitionUtils(){}

PartitionUtils::~PartitionUtils(){}

void PartitionUtils::createNewPartition(){

}

void PartitionUtils::wipeExistingPartition(){

}

void PartitionUtils::eraseExistingPartition(){

}

void PartitionUtils::enter(){
    const char* entries[] = {"Create new partition", "Wipe partition", "Erase partition", "Return to BIOS"};
    int choice = -1;
    bool active = true;
    while(active){
        choice = utils::menu::printMenu("PartitionUtils", entries, 4, choice);
        switch(choice){
            case 0:
                createNewPartition();
            break;
            case 1:
                wipeExistingPartition();
            break;
            case 2:
                eraseExistingPartition();
            break; 
            case 3:
                return;
            break;
        }
        choice = -1;
        wait(std::cin);
    }
}





// Method for outside usage

void PartitionUtils::createPartition(const char* path, uint64_t start, uint64_t end, int file_system){

}

void PartitionUtils::wipePartition(const char* path, partition_definition* definitition){

}

void PartitionUtils::erasePartition(const char* path, partition_definition* definitition){

}

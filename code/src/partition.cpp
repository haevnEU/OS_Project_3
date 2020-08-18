#include "../include/partition.h"

bool Partition::isPrimary() const{
    return bootable;
}

FileSystemType Partition::fileSystemType() const{
    return fileSystemType_m;
}

uint32_t Partition::startAddress() const{
    return startAddress_m;
}

uint32_t Partition::endAddress() const{
    return endAddress_m;
}

uint32_t Partition::size() const{
    return size_m;
}

Partition::Partition(unsigned int startAddress, uint32_t endAddress, FileSystemType fileSystemType, bool isBootAble){
    this->startAddress_m = startAddress;
    this->endAddress_m = endAddress;
    this->size_m = startAddress + endAddress;
    this->bootable = isBootAble;
    this->fileSystemType_m = fileSystemType;
}


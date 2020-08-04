#include "partition.h"

bool Partition::isPrimary() const{
    return bootable;
}

FileSystemType Partition::fileSystemType() const{
    return fileSystemType_m;
}

uint32_t Partition::startAddress() const{
    return startAddress_m;
}

uint32_t Partition::size() const{
    return size_m;
}

Partition::Partition(unsigned int startAddress, unsigned int size, FileSystemType fileSystemType, bool isBootAble){
    this->startAddress_m = startAddress;
    this->size_m = size;
    this->bootable = isBootAble;
    this->fileSystemType_m = fileSystemType;
}


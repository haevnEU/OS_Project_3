#include "diskhandler.h"

DiskHandler::DiskHandler(){
    disks = new std::vector<Disk*>();
}

void DiskHandler::addDisk(Disk *disk){
    disks->push_back(disk);
}

void DiskHandler::setDisk(int index){
    if(index < 0 || index > disks->size()){
        std::cout << "Disk is out of bounds" << std::endl;
    }
    this->disk = disks->at(index);
}

std::vector<Disk*>* DiskHandler::getDisks() const{
    return disks;
}

Disk *DiskHandler::getDisk() const{
    return disk;
}



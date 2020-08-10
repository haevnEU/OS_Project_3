#include "../include/diskhandler.h"

DiskHandler::DiskHandler(){
    disks = new std::vector<Disk*>();
}

void DiskHandler::addDisk(Disk *disk){
    if(!utils::file_exists(disk->path())){
        std::cerr << utils::COLOR_RED << "[Diskhandler] Disk file does not exists, deleting object" << utils::COLOR_RESET << std::endl;
        delete(disk);
        return;
    }

    for (auto i = disks->begin(); i != disks->end(); ++i){
        if(strcmp(disk->path(), ((Disk*)*i)->path()) == 0){
            std::cerr << utils::COLOR_YELLOW << "[Diskhandler] Disk allready loaded" << utils::COLOR_RESET << std::endl;
            delete(disk);
            return;
        }
    }

    std::cout << "Add disk" << std::endl;
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



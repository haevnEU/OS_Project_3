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
    if(!utils::file_extension_equals(disk->path(), "vdf")){
        std::cerr << utils::COLOR_RED << "[Diskhandler] File does not met the required extension" << utils::COLOR_RESET << std::endl;
        delete(disk);
        return;
    }
    
    char* buf = new char[3];
    std::ifstream is(disk->path());
    if (is) {
        is.seekg(-3, is.end); // x is the number of bytes to read before the end
    }
    is.read(buf, 3);
    is.close();

    if(((uint8_t)buf[1]) != 0xC0){
        std::cerr << utils::COLOR_RED << "[Diskhandler] This is not a virtual disk file" << utils::COLOR_RESET << std::endl;
        delete(disk);
        delete[] buf;
        return;
    }
    
    if(buf[2] < MINIMUM_VIRTUAL_DISK_FILE_VERSION){
        std::cerr << utils::COLOR_RED << "[Diskhandler] This virtual disk file is no longer supported. Consider recretion or the version converter tool" << utils::COLOR_RESET << std::endl;
        delete(disk);
        delete[] buf;
        return;
    }

    delete[] buf;
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

int DiskHandler::amountDisks(){
    return disks->size();
}

Disk *DiskHandler::getDisk(int index) const{
    if(index < 0 || index > disks->size()){
        return nullptr;
    }
    return disks->at(index);
}

void DiskHandler::removeDisk(){
 for (auto i = disks->begin(); i != disks->end(); ++i){
        if(strcmp(disk->path(), ((Disk*)*i)->path()) == 0){
            delete(disk);
            disk = nullptr;
            return;
        }
    }
}

#include "utils/diskutil.h"


diskUtil::diskUtil(){}

void diskUtil::show(Disk* disk){
    this->disk = disk;
    bool active = true;
    while(active){
        utils::printHeader("Disk Utils");
        std::cout << "1) List meta information" << std::endl
                  << "2) Create MBR" << std::endl
                  << "3) List partitions" << std::endl
                  << "4) Create new partition" << std::endl
                  << "q) Back" << std::endl
                  << "Choice: ";
        char choice;
        std::cin >> choice;

        switch(choice){
        case '1':
            listMetaInformation();
            break;
        case '2':
            createMBR();
            break;
        case '3':

            break;
        case '4':
            createPartition();
            break;
        case '5':
            break;
        case '6':
            break;
        case '7':
            break;
        case '8':
            break;
        case '9':
            break;
        case 'q': return;


        default:
            break;
        }
        wait(std::cin);
    }
}


void diskUtil::createMBR(){
    utils::printModule("Disk Utils", "MBRCreator", "This module submodule creates a "
                                                   "Master Boot Record on a virtual disk"
                                                   "file.\n"
                                                   "Importan note using this on an existing"
                                                   "MBR will override its content and all"
                                                   "data will be lost.");
    if(nullptr == disk){
        std::cout << utils::COLOR_RED << "[create] Cannot create master boot record. Selected disk is nullptr" << utils::COLOR_RESET << std::endl;
        return;
    }
    std::cout << "Are you sure to create a MBR (y/N): ";
    char choice;
    std::cin >> choice;

    if(choice == 'y' || choice == 'Y'){
        disk->createMasterBootRecord();
    }else{
        std::cout << "Operation aborted." << std::endl;
        wait(std::cin);
    }
}

void diskUtil::listMetaInformation(){
    utils::printModule("Disk Utils", "MBRMetaInformer", "This submodule lists meta information"
                                                        "about a virtual disk.");
    if(nullptr == disk){
        std::cout << utils::COLOR_RED << "[list] Cannot list MBR. Selected disk is nullptr" << utils::COLOR_RESET << std::endl;
        return;
    }
    if(nullptr == disk->MBR()){
        std::cout << utils::COLOR_RED << "[list] Cannot list MBR. No MBR found on disk" << utils::COLOR_RESET << std::endl;
        return;
    }

    std::cout << "Size of disk: " << disk->size() << std::endl
              << "Mounted: " << (disk->isMounted() ? utils::ICON_ACCEPT : utils::ICON_DENIED) << std::endl;
    for(int i = 0; i < 4; ++i){
        Partition* partition = disk->MBR()->partition(i);
        std::cout << "+------------------------------" << std::endl;
        if(nullptr == partition){
            std::cout << "| Partition No: " << i << ": " << std::endl
                      << "| \t" << utils::COLOR_MAGENTA << "NULL" << utils::COLOR_RESET << std::endl
                      << "+------------------------------" << std::endl;
        }else{
            if(partition->startAddress() == 0x00 && partition->size() == 0){
                std::cout << "| Partition No: " << i << ": " << std::endl
                          << "| \t" << utils::COLOR_MAGENTA << "NULL" << utils::COLOR_RESET << std::endl
                          << "+------------------------------" << std::endl;
            }else{
                std::cout << "| Partition No: " << i << utils::COLOR_CYAN
                          << utils::COLOR_RESET << std::endl
                          << "| \t" << utils::COLOR_CYAN << "Startaddress: 0x" << std::hex << std::setw(8) << std::setfill('0') << partition->startAddress()
                          << utils::COLOR_RESET << std::endl
                          << "| \t" << utils::COLOR_CYAN << "Endaddress:   0x" << std::hex << std::setw(8) << std::setfill('0') << (partition->size() + partition->startAddress())
                          << utils::COLOR_RESET << std::endl
                          << "| \t" << utils::COLOR_CYAN << "Size:         "   << std::dec << partition->size()
                          << utils::COLOR_RESET << std::endl
                          << "| \t" << utils::COLOR_CYAN << "Primary:      "   << (partition->isPrimary() ? utils::ICON_ACCEPT : utils::ICON_DENIED)
                          << utils::COLOR_RESET << std::endl
                          << "| \t" << utils::COLOR_CYAN << "Filesystem:   "   << getFileSystemName(partition->fileSystemType())
                          << utils::COLOR_RESET << std::endl
                          << "+------------------------------" << utils::COLOR_RESET << std::endl;
                std::cout << utils::COLOR_RESET;
            }
        }
    }
    std::cout << std::endl;
}


void diskUtil::createPartition(){
    uint32_t size;
    FileSystemType fs = FileSystemType::FAT;
    bool bootable;

    utils::printModule("Disk Utils", "PartitionCreator", "This submodule provides a wizzard "
                                                         "which allows the creation of a"
                                                         "new partition.");
    std::cout << "Enter partition size: ";
    std::cin >> size;
    std::cout << "Bootable partition (y/N): ";
    char in;
    std::cin >> in;
    if(in == 'y' || in == 'Y'){
        bootable = true;
    }else{
        bootable = false;
    }

    std::cout << "Choose your filesystem" << std::endl
              << "0) NONE" << std::endl
              << "1) FAT" << std::endl
              << "2) INODE" << std::endl;
    std::cin >> in;
    if(in == '1')fs = FileSystemType::FAT;
    else if(in == '2')fs = FileSystemType::INODE;
    else fs = FileSystemType::NONE;
    disk->createPartition(size, fs, bootable);
}

void diskUtil::openPartitionTool(){

}

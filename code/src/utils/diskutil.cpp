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
                                                   "Important note using this on an existing"
                                                   "MBR will override its content and all"
                                                   "data will be lost.");
    if(nullptr == disk){
        utils::printError("Cannot create master boot record", "create", DISK_NULL);
        return;
    }
    utils::printInfo("Create a new master boot record(y/N): ", "create");
    char choice;
    std::cin >> choice;

    if(choice == 'y' || choice == 'Y'){
        disk->createMasterBootRecord();
    }else{
        utils::printWarning("Operation aborted", "create");
    }
}

void diskUtil::listMetaInformation(){
    utils::printModule("Disk Utils", "MBRMetaInformer", "This submodule lists meta information"
                                                        "about a virtual disk.");
    if(nullptr == disk){
        utils::printError("Cannot list master boot record", "list", DISK_NULL);
        return;
    }
    if(nullptr == disk->MBR()){
        utils::printError("No master boot record found", "list", MBR_NULL);
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
    int64_t size;
    FileSystemType fs = FileSystemType::FAT;
    bool bootable;

    utils::printModule("Disk Utils", "PartitionCreator", "This submodule provides a wizard "
                                                         "which allows the creation of a"
                                                         "new partition.");
    
    std::cout << "[partition] Enter partition size: ";
    std::cin >> size;
   
    if(size < 0){
        std::cerr << utils::COLOR_RED << "[partition] A negative size was entered: " << size << utils::COLOR_RESET << std::endl;
        return;
    }    
    if(size > disk->available_size()){
        std::cerr << utils::COLOR_RED << "[partition] Size of " << size << " exceeds the maximum capacity of " << disk->available_size()  << utils::COLOR_RESET << std::endl; 
        return;
    }
    
    std::cout << "[partition] Primary partition (y/N): ";
        char in;
    std::cin >> in;
    if(in == 'y' || in == 'Y'){
        bootable = true;
    }else{
        bootable = false;
    }

    std::cout << "[partition]Choose your filesystem" << std::endl
              << "[partition] 0) None" << std::endl
              << "[partition] 1) FAT" << std::endl
              << "[partition] 2) INode" << std::endl
              << "[partition] Choice: ";
    std::cin >> in;
    
    if(in == '1')fs = FileSystemType::FAT;
    else if(in == '2')fs = FileSystemType::INODE;
    else fs = FileSystemType::NONE;
    disk->createPartition(size, fs, bootable);
}

void diskUtil::openPartitionTool(){

}

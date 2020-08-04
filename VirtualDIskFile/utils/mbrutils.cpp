#include "mbrutils.h"

MBRUtils::MBRUtils()
{

}

void MBRUtils::setDisk(Disk *disk){
    this->disk = disk;
}

void MBRUtils::listMasterBootRecord(){
    utils::printModule("Master Boot Record Utils", "Details");
    for(int i = 0; i < 4; ++i){
        Partition* partition = disk->MBR()->partition(i);
        if(nullptr == partition){
            std::cout << "Partition No. " << i << " is nullptr" << std::endl;
        }else{
            std::cout << "Partition No. " << i << std::endl
                      << "Size: " << partition->size() << std::endl
                      << "Filesystem: " << partition->fileSystemType() << std::endl
                      << "Bootable: " << partition->isPrimary() << std::endl
                      << "Startaddress: " << partition->startAddress() << std::endl
                      << "Endaddress: " << (partition->startAddress() + partition->size()) << std::endl;
        }
}
}

void MBRUtils::createMasterBootRecord(){
    utils::printModule("Master Boot Record Utils", "CreateMasterBootRecord");
    disk->createMasterBootRecord();
}

void MBRUtils::wipeMasterBootRecord(){

}

void MBRUtils::enterInteractiveMode(){
    bool active = true;
    while(active){
        utils::printHeader("Master Boot Record Utils");
        std::cout << "1) MBR Details" << std::endl
                  << "2) Mount disk" << std::endl
                  << "3) Unmount disk" << std::endl
                  << "4) Create MBR" << std::endl
                  << "5) Wipe MBR" << std::endl
                  << "q) quit" << std::endl;
        char choice;
        std::cin >> choice;
        switch(choice){
            case '1':
                listMasterBootRecord();
                break;
            case '2':
                disk->mount();
                break;
            case '3':
                disk->unmount();
                break;
            case '4':
                createMasterBootRecord();
                break;
            case '5':
                wipeMasterBootRecord();
                break;

            case 'q':
                active = false;
                break;

            default:
                break;
        }

        std::cout << "Hit any key to continue";
        system("read");
    }
}

void MBRUtils::leaveInteractiveMode(){

}

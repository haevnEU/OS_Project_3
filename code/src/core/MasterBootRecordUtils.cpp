#include "../../include/core/MasterBootRecordUtils.hpp"

using namespace core::utilities;

MasterBootRecordUtils::MasterBootRecordUtils(){}

MasterBootRecordUtils::~MasterBootRecordUtils(){}


void MasterBootRecordUtils::enter(){
    const char* entries[] = {"Create Master Boot Record", "Format Master Boot Record", "Inspect a Master Boot Record", "Return to BIOS"};
    int choice = -1;
    bool active = true;
    while(active){
        choice = utils::menu::printMenu("MBRUtils", entries, 4, choice);
        switch(choice){
            case 0:
                createMBR();
            break;
            case 1:
                formatMBR();
            break;
            case 2:
                inspectMBR();
            break; 
            case 3:
                return;
            break;
        }
        choice = -1;
        wait(std::cin);
    }
}

void MasterBootRecordUtils::createMBR(){
    std::cout << utils::colors::CLEAR;
    std::cout << "This wizard allows the creation of a master boot record." << std::endl << "Enter path to virtual disk file: ";
    std::string path;
    std::cin >> path;
    if(path.size() > 0 && path[0] == 'q'){
        std::cout << utils::colors::YELLOW << "Operation aborted" << utils::colors::RESET << std::endl;
        return;
    }
    createMasterBootRecord(path.c_str());
}

void MasterBootRecordUtils::formatMBR(){
    std::cout << utils::colors::CLEAR;
    std::cout << "This wizard allows to format a master boot record." << std::endl << "Enter path to virtual disk file: ";
    std::string path;
    std::cin >> path;
    if(path.size() > 0 && path[0] == 'q'){
        std::cout << utils::colors::YELLOW << "Operation aborted" << utils::colors::RESET << std::endl;
        return;
    }
    std::cout << "1) Erase(Insecure)" << std::endl
              << "2) Wipe(secure)" << std::endl
              << "c) Cancel" << std::endl;
    std::cout << "Choice: ";
    char choice;
    std::cin >> choice;
    if('1' == choice){
        eraseMasterBootRecord(path.c_str());
    }else if('2' == choice){
        wipeMasterBootRecord(path.c_str());
    }
}

void MasterBootRecordUtils::inspectMBR(){
    std::cout << utils::colors::CLEAR;
    std::cout << "This wizard allows the inspection of a master boot record." << std::endl <<  "Enter path to virtual disk file: ";
    std::string path;
    std::cin >> path;
    if(!DiskUtils::getInstance().verifyDisk(path.c_str())){
        return;
    }
    auto fd = open(path.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
    if(-1 == fd){
        std::cout << utils::colors::RED << "Cannot inspect master boot record: open virtual disk file" << utils::colors::RESET << std::endl;
        return;
    }
    
    uint8_t* data_m = static_cast<uint8_t*>(mmap(nullptr, 0x1FF, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(MAP_FAILED == data_m){
        std::cout << utils::colors::RED << "Cannot inspect master boot record: maping failed" << utils::colors::RESET << std::endl;
        close(fd);
        return;
    }

    close(fd); 
    std::cout <<  std::setfill('0') << std::setw(8) << std::hex << 0 << '\t';
    for(int i = 0; i <= 0x1FF; i++){ 
        if(i > 0)
        if(i % 16 == 0){
            std::cout << std::endl << std::setfill('0') << std::setw(8) << std::hex << i << '\t';
        }else if(i % 8 == 0){
            std::cout << "  ";
        }
        std::cout <<  std::setfill('0') << std::setw(2) << std::hex << (int)data_m[i] << ' ';

    }

    std::cout << std::endl;
    munmap(data_m, 0x1FF);
}





// Method for outside usage

void MasterBootRecordUtils::createMasterBootRecord(const char* path){
    if(!DiskUtils::getInstance().verifyDisk(path)){
        return;
    }
    auto fd = open(path, O_RDWR, S_IRUSR | S_IWUSR);
    if(-1 == fd){
        std::cout << utils::colors::RED << "Cannot write master boot record: open virtual disk file" << utils::colors::RESET << std::endl;
        return;
    }
    
    uint8_t* data_m = static_cast<uint8_t*>(mmap(nullptr, 0x1FF, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(MAP_FAILED == data_m){
        std::cout << utils::colors::RED << "Cannot write master boot record: maping failed" << utils::colors::RESET << std::endl;
        close(fd);
        return;
    }

    close(fd);

    for(int i = 0; i < (0x1BE - 4); i += 4){
        data_m[i] = static_cast<uint8_t>(0xB0);
        data_m[i + 1] = static_cast<uint8_t>(0x01);
        data_m[i + 2] = static_cast<uint8_t>(0xC0);
        data_m[i + 3] = static_cast<uint8_t>(0xDE);
    }
    for(int i = 0x1BE; i < 0x1F8; i++){
       data_m[i] = 0;
    }
    for(int i = 0; i < 4; i++)data_m[0x1BE + (i * 16)] = static_cast<uint8_t>(0x00);

    data_m[0x1FE] = static_cast<uint8_t>(0x55);
    data_m[0x1FF] = static_cast<uint8_t>(0xAA);

    munmap(data_m, 0x1FF);
}
        
void MasterBootRecordUtils::wipeMasterBootRecord(const char* path){
    if(!DiskUtils::getInstance().verifyDisk(path)){
        return;
    }

    auto fd = open(path, O_RDWR, S_IRUSR | S_IWUSR);
    if(-1 == fd){
        std::cout << utils::colors::RED << "Cannot write master boot record: open virtual disk file" << utils::colors::RESET << std::endl;
        return;
    }
    
    uint8_t* data_m = static_cast<uint8_t*>(mmap(nullptr, 0x1FF, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(MAP_FAILED == data_m){
        std::cout << utils::colors::RED << "Cannot write master boot record: maping failed" << utils::colors::RESET << std::endl;
        close(fd);
        return;
    }

    close(fd);
 
    for(int i = 0; i < (0x1BE - 4); i += 4){
        data_m[i] = static_cast<uint8_t>(0xB0);
        data_m[i + 1] = static_cast<uint8_t>(0x01);
        data_m[i + 2] = static_cast<uint8_t>(0xC0);
        data_m[i + 3] = static_cast<uint8_t>(0xDE);
    }
    for(int i = (0x1BE + 4) ; i <= 0x1FD; i++){
       data_m[i] = 0;
    }

    data_m[0x1FE] = static_cast<uint8_t>(0x55);
    data_m[0x1FF] = static_cast<uint8_t>(0xAA);

    munmap(data_m, 0x1FF);
}

void MasterBootRecordUtils::eraseMasterBootRecord(const char* path){
    if(!DiskUtils::getInstance().verifyDisk(path)){
        return;
    }
    
    wipeMasterBootRecord(path);

    // TODO implement overriding of partition
}

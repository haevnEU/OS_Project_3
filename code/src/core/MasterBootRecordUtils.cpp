#include "../../include/core/MasterBootRecordUtils.hpp"

using namespace core::utilities;

MasterBootRecordUtils::MasterBootRecordUtils(){}

MasterBootRecordUtils::~MasterBootRecordUtils(){}


void MasterBootRecordUtils::enter()
{
    const char* entries[] = {
        "Create Master Boot Record",
        "Format Master Boot Record",
        "Inspect a Master Boot Record",
        "Return to BIOS"
    }; // TUI Options

    int choice = -1;
    bool active = true;
    utils::menu::menu_settings settings;

    ///-- Console Output: Print Terminal User Interface
    while(active){
        choice = utils::menu::print(entries, 4, "MBRUtils", settings);
        switch(choice){
        {
            ///- Create Master Boot Recod
            case 0:
                createMBR();
            break;

            ///- Format Master Boot Recod
            case 1:
                formatMBR();
            break;

            ///- Inspect Master Boot Recod
            case 2:
                inspectMBR();
            break; 

            case 3:
                return;
            break;
        }
        settings.clear_cache = true;
        settings.sub_header = last_result_message;
    }
}

void MasterBootRecordUtils::createMBR()
{
    ///-- Console Output: Request path input
    std::cout << utils::colors::CLEAR;
    std::cout << "This wizard allows the creation of a master boot record." << std::endl << "Enter path to virtual disk file: ";
    
    ///-- Console Input: Path
    std::string path;
    std::cin >> path;

    if(path.size() > 0 && path[0] == 'q'){
        ///- Handle possible abort
        last_result_message = utils::colors::YELLOW;
        last_result_message.append("Operation aborted").append(utils::colors::RESET);
        return;
    }

    ///-- Call createMasterBootRecord
    createMasterBootRecord(path.c_str());
    last_result_message = utils::colors::GREEN;
    last_result_message.append("Master boot record was successful created").append(utils::colors::RESET);
}

void MasterBootRecordUtils::formatMBR()
{
    ///-- Console Output: Request path input
    std::cout << utils::colors::CLEAR;
    std::cout << "This wizard allows to format a master boot record." << std::endl << "Enter path to virtual disk file: ";
    
    ///-- Console Input: Path
    std::string path;
    std::cin >> path;

    if(path.size() > 0 && path[0] == 'q'){
        ///- Handle possible abort
        last_result_message = utils::colors::YELLOW;
        last_result_message.append("Operation aborted").append(utils::colors::RESET);
        return;
    }

    ///-- Console Output: Request choice
    std::cout << "1) Erase(Insecure)" << std::endl
              << "2) Wipe(secure)" << std::endl
              << "c) Cancel" << std::endl;
    std::cout << "Choice: ";

    ///-- Console Input: choice
    char choice;
    std::cin >> choice;

    if('1' == choice)
    {
        eraseMasterBootRecord(path.c_str());
    }
    else if('2' == choice)
    {
        wipeMasterBootRecord(path.c_str());
    }
        last_result_message = utils::colors::GREEN;
        last_result_message.append("Master boot record was successful erased/deleted").append(utils::colors::RESET);
}

void MasterBootRecordUtils::inspectMBR()
{
    ///-- Console Output: Request path input
    std::cout << utils::colors::CLEAR;
    std::cout << "This wizard allows the inspection of a master boot record." << std::endl <<  "Enter path to virtual disk file: ";
    
    ///-- Console Input: Path
    std::string path;
    std::cin >> path;

    ///-- Verify Input: Path
    if(!DiskUtils::getInstance().verifyDisk(path.c_str())){
        last_result_message = utils::colors::RED;
        last_result_message.append("Can not inspect master boot record: Virtual disk file cannot be verified").append(utils::colors::RESET);
        return;
    }

    ///- Tries opening file
    auto fd = open(path.c_str(), O_RDWR, S_IRUSR | S_IWUSR);
    if(-1 == fd){
        last_result_message = utils::colors::RED;
        last_result_message.append("Can not inspect master boot record: Virtual disk file cannot be opened").append(utils::colors::RESET);
        return;
    }
    
    uint8_t* data_m = static_cast<uint8_t*>(mmap(nullptr, 0x1FF, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(MAP_FAILED == data_m){
        ///- Handle possible mapping failure error
        last_result_message = utils::colors::RED;
        last_result_message.append("Can not inspect master boot record: Virtual disk file cannot be mapped").append(utils::colors::RESET);
        close(fd);
        return;
    }

    close(fd);

    std::cout <<  std::setfill('0') << std::setw(8) << std::hex << 0 << '\t';
    
    for(int i = 0; i <= 0x1FF; i++){ 
        if(i > 0)
        if(i % 16 == 0){
            std::cout << std::endl << utils::colors::RESET << std::setfill('0') << std::setw(8) << std::hex << i << utils::colors::UNDERLINE<< '\t' << utils::colors::RESET;
        }else if(i % 8 == 0){
            std::cout << "  ";
        }
        if(i <= 0x1BD){
            std::cout << utils::colors::BACK_MAGENTA << std::setfill('0') << std::setw(2) << std::hex << (int)data_m[i] << ' ';
        }else if(i <= (0x1BD + 16)){
            std::cout <<  utils::colors::BACK_CYAN << std::setfill('0') << std::setw(2) << std::hex << (int)data_m[i] << ' ';
        }else if(i <= (0x1BD + 32)){
            std::cout <<  utils::colors::BACK_GREEN << std::setfill('0') << std::setw(2) << std::hex << (int)data_m[i] << ' ';
        }else if(i <= (0x1BD + 48)){
            std::cout <<  utils::colors::BACK_CYAN << std::setfill('0') << std::setw(2) << std::hex << (int)data_m[i] << ' ';
        }else if(i <= (0x1BD + 64)){
            std::cout <<  utils::colors::BACK_GREEN << std::setfill('0') << std::setw(2) << std::hex << (int)data_m[i] << ' ';
        }else{
        std::cout <<  utils::colors::BACK_MAGENTA << std::setfill('0') << std::setw(2) << std::hex << (int)data_m[i] << ' ';    
        }
    }

    std::cout << std::endl << utils::colors::RESET;
    munmap(data_m, 0x1FF);
    wait(std::cin);
}





// Method for outside usage

void MasterBootRecordUtils::createMasterBootRecord(const char* path){
    if(!DiskUtils::getInstance().verifyDisk(path)){
        ///- Verifies Disk Path
        last_result_message = utils::colors::RED;
        last_result_message.append("Can not create master boot record: Virtual disk file cannot be verified").append(utils::colors::RESET);
        return;
    }

    ///- Tries opening file
    auto fd = open(path, O_RDWR, S_IRUSR | S_IWUSR);
    if(-1 == fd){
        last_result_message = utils::colors::RED;
        last_result_message.append("Can not write master boot record: Virtual disk file cannot be opened").append(utils::colors::RESET);
        return;
    }
    
    uint8_t* data_m = static_cast<uint8_t*>(mmap(nullptr, 0x1FF, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(MAP_FAILED == data_m){
        last_result_message = utils::colors::RED;
        last_result_message.append("Can not write master boot record: Virtual disk file cannot be mapped").append(utils::colors::RESET);
        return;
    }
    
    close(fd);

    for(int i = 0; i < (0x1BE - 4); i += 4)
    {
        data_m[i] = static_cast<uint8_t>(0xB0);
        data_m[i + 1] = static_cast<uint8_t>(0x01);
        data_m[i + 2] = static_cast<uint8_t>(0xC0);
        data_m[i + 3] = static_cast<uint8_t>(0xDE);
    }

    for(int i = 0x1BE; i < 0x1F8; i++)
    {
       data_m[i] = 0;
    }

    for(int i = 0; i < 4; i++)data_m[0x1BE + (i * 16)] = static_cast<uint8_t>(0x00);

    data_m[0x1FE] = static_cast<uint8_t>(0x55);
    data_m[0x1FF] = static_cast<uint8_t>(0xAA);

    munmap(data_m, 0x1FF);

    last_result_message = utils::colors::GREEN;
    last_result_message.append("Master boot record was created").append(utils::colors::RESET);
}
        
void MasterBootRecordUtils::wipeMasterBootRecord(const char* path){
    if(!DiskUtils::getInstance().verifyDisk(path)){
        last_result_message = utils::colors::RED;
        last_result_message.append("Can not wipe master boot record: Virtual disk file cannot be verified").append(utils::colors::RESET);
        return;
    }

    ///- Tries opening file
    auto fd = open(path, O_RDWR, S_IRUSR | S_IWUSR);
    if(-1 == fd){
        last_result_message = utils::colors::RED;
        last_result_message.append("Can not wipe master boot record: Virtual disk file cannot be opened").append(utils::colors::RESET);
        return;
    }
    
    uint8_t* data_m = static_cast<uint8_t*>(mmap(nullptr, 0x1FF, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(MAP_FAILED == data_m){
        last_result_message = utils::colors::RED;
        last_result_message.append("Can not wipe master boot record: Virtual disk file cannot be mapped").append(utils::colors::RESET);
        close(fd);
        return;
    }

    close(fd);
 
    for(int i = 0; i < (0x1BE - 4); i += 4)
    {
        data_m[i] = static_cast<uint8_t>(0xB0);
        data_m[i + 1] = static_cast<uint8_t>(0x01);
        data_m[i + 2] = static_cast<uint8_t>(0xC0);
        data_m[i + 3] = static_cast<uint8_t>(0xDE);
    }

    for(int i = (0x1BE + 4) ; i <= 0x1FD; i++)
    {
       data_m[i] = 0;
    }

    data_m[0x1FE] = static_cast<uint8_t>(0x55);
    data_m[0x1FF] = static_cast<uint8_t>(0xAA);

    munmap(data_m, 0x1FF);
    last_result_message.append("Master boot record was wiped").append(utils::colors::RESET);
}

void MasterBootRecordUtils::eraseMasterBootRecord(const char* path)
{
    ///- Verifies Disk Path
    if(!DiskUtils::getInstance().verifyDisk(path)){
        return;
    }
    
    ///- Calls wipeMasterBootRecord
    wipeMasterBootRecord(path);

    // TODO implement overriding of partition
}

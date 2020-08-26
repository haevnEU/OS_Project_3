#include "../../include/core/PartitionUtils.hpp"

using namespace core::utilities;


PartitionUtils::PartitionUtils(){}

PartitionUtils::~PartitionUtils(){}

void PartitionUtils::createNewPartition(){
    std::cout << utils::colors::CLEAR;
    std::cout << "This wizard allows the creation of a master boot record." << std::endl << "Enter path to virtual disk file: ";
    std::string path;
    std::cin >> path;
    int resultCode = utils::verifyInput(path);
    if(resultCode == utils::verify_results::operation_aborted){
        error_message = utils::colors::YELLOW;
        error_message.append("Operation aborted").append(utils::colors::RESET);
        return;
    }else if(resultCode == utils::verify_results::input_to_small || resultCode == utils::verify_results::input_path_invalid){
        error_message = utils::colors::YELLOW;
        error_message.append("Given input was invalid").append(utils::colors::RESET);
        return;
    }else{
        const char* entries[] = { "INode", "FAT", "FAT hidden", "Cancel"};
        char c2;
       // int choice = utils::menu::printMenu("New Partition", entries, 4, true);
        
        utils::menu::menu_settings settings;
        settings.clear_cache = true;
        settings.row_selection_overflow = true;
        settings.preselected_row = 0;
        int choice = utils::menu::print(entries, 4, "New Partition", settings);
        uint64_t size;
        std::cout << "Enter virtual disk size: ";
        std::cin >> size;
        if(size < 0){
            error_message = utils::colors::RED;
            error_message.append("Given size is to small").append(utils::colors::RESET);
            return;
        }else if(size > MAX_DISK_SIZE){
            error_message = utils::colors::RED;
            error_message.append("Given size is to big").append(utils::colors::RESET);
            return;
        }

        createPartition(path.c_str(), size, choice);
    }
}

void PartitionUtils::wipeExistingPartition(){
    std::cout << utils::colors::CLEAR;
    std::cout << "This wizard allows the creation of a master boot record." << std::endl << "Enter path to virtual disk file: ";
    std::string path;
    std::cin >> path;
    int resultCode = utils::verifyInput(path);
     if(resultCode == utils::verify_results::operation_aborted){
        error_message = utils::colors::YELLOW;
        error_message.append("Operation aborted").append(utils::colors::RESET);
        return;
    }else if(resultCode == utils::verify_results::input_to_small || resultCode == utils::verify_results::input_path_invalid){
        error_message = utils::colors::YELLOW;
        error_message.append("Given input was invalid").append(utils::colors::RESET);
        return;
    }else{
        const char* entries[] = { "Partition 1", "Partition 2", "Partition 3", "Partition 4", "Cancel"};
        utils::menu::menu_settings settings;
        settings.clear_cache = true;
        settings.row_selection_overflow = true;
        settings.preselected_row = 0;
        int choice = utils::menu::print(entries, 5, "Wipe Partition", settings);
        if(choice == 4){
            return;
        }else if(choice < 0 || choice > 3){
            return;
        }
        wipePartition(path.c_str(), choice);
    }
}

void PartitionUtils::eraseExistingPartition(){
    std::cout << utils::colors::CLEAR;
    std::cout << "This wizard allows the creation of a master boot record." << std::endl << "Enter path to virtual disk file: ";
    std::string path;
    std::cin >> path;
    int resultCode = utils::verifyInput(path);
     if(resultCode == utils::verify_results::operation_aborted){
        error_message = utils::colors::YELLOW;
        error_message.append("Operation aborted").append(utils::colors::RESET);
        return;
    }else if(resultCode == utils::verify_results::input_to_small || resultCode == utils::verify_results::input_path_invalid){
        error_message = utils::colors::YELLOW;
        error_message.append("Given input was invalid").append(utils::colors::RESET);
        return;
    }else{
        const char* entries[] = { "Partition 1", "Partition 2", "Partition 3", "Partition 4", "Cancel"};
        utils::menu::menu_settings settings;
        settings.clear_cache = true;
        settings.row_selection_overflow = true;
        settings.preselected_row = 0;
        int choice = utils::menu::print(entries, 5, "Erase Partition", settings);
        if(choice == 4){
            return;
        }else if(choice < 0 || choice > 3){
            return;
        }
        erasePartition(path.c_str(), choice);
    }
}

void PartitionUtils::enter(){
    const char* entries[] = {"Create new partition", "Wipe partition", "Erase partition", "Return to BIOS"};
    int choice = -1;
    bool active = true;
    utils::menu::menu_settings settings;
    settings.clear_cache = false;
    settings.row_selection_overflow = true;
    settings.preselected_row = 0;
    while(active){
        choice = utils::menu::print(entries, 4, "PartitionUtils", settings);
        switch(choice){
            case 0:
                createNewPartition();
            break;
            case 1:
                wipeExistingPartition();
            break;
            case 2:
                eraseExistingPartition();
            break; 
            case 3:
                return;
            break;
        }
        choice = -1;
        settings.clear_cache = true;
        settings.sub_header = std::string(utils::colors::BOLD).append(error_message).append(utils::colors::RESET);
    }
}





// Method for outside usage

void PartitionUtils::createPartition(const char* path, uint64_t size, int file_system){
    if(!DiskUtils::getInstance().verifyDisk(path)){
        return;
    }
    auto fd = open(path, O_RDWR, S_IRUSR | S_IWUSR);
    if(-1 == fd){
        std::cout << utils::colors::RED << "Failed to create new partition: open virtual disk file" << utils::colors::RESET << std::endl;
        return;
    }
    
    uint8_t* data_m = static_cast<uint8_t*>(mmap(nullptr, 0x1FF, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(MAP_FAILED == data_m){
        std::cout << utils::colors::RED << "Failed to create new partition: maping failed" << utils::colors::RESET << std::endl;
        close(fd);
        return;
    }

    close(fd);

    int index = 0;
    uint64_t start;
    for(; index < 4; index++){
        if(data_m[0x1BE + (index * 16)] == 0x00){
            break;
        }
        start = (0) + 1;
    }
    uint64_t end = start + size;
    // Write primary partition flag and filesystem type.
    // Note an partition entry is exact 16 Bytes
    data_m[0x1BE + (index * 16)] = static_cast<uint8_t>(0x80);
    data_m[0x1BF + (index * 16)] =  static_cast<uint8_t>(file_system);

    // Ill assign the LSB to the smallest position and the HSB to the highest position.
    // Start adress of partition
    data_m[0x1C0 + (index * 16)] = static_cast<uint8_t>(start & 0xFF);
    data_m[0x1C1 + (index * 16)] = static_cast<uint8_t>((start >> 8) & 0xFF);
    data_m[0x1C2 + (index * 16)] = static_cast<uint8_t>((start >> 16) & 0xFF);
    data_m[0x1C3 + (index * 16)] = static_cast<uint8_t>((start >> 24) & 0xFF);

    // End adress of partition
    data_m[0x1C4 + (index * 16)] = static_cast<uint8_t>(end & 0xFF);
    data_m[0x1C5 + (index * 16)] = static_cast<uint8_t>((end >> 8) & 0xFF);
    data_m[0x1C6 + (index * 16)] = static_cast<uint8_t>((end >> 16) & 0xFF);
    data_m[0x1C7 + (index * 16)] = static_cast<uint8_t>((end >> 24) & 0xFF);

    // Unused, e.g. CHS address, etc
    for(uint64_t i = 0x1C8; i < (0x1CE - 4); i+= 4){
        data_m[i] = static_cast<uint8_t>(0xBA);
        data_m[i + 1] = static_cast<uint8_t>(0xDF);
        data_m[i + 2] = static_cast<uint8_t>(0xF0);
        data_m[i + 3] = static_cast<uint8_t>(0x0D);
    }


    munmap(data_m, 0x1FF);
}

void PartitionUtils::wipePartition(const char* path, int index){
    if(!DiskUtils::getInstance().verifyDisk(path)){
        return;
    }
    auto fd = open(path, O_RDWR, S_IRUSR | S_IWUSR);
    if(-1 == fd){
        std::cout << utils::colors::RED << "Failed to wipe partition: open virtual disk file" << utils::colors::RESET << std::endl;
        return;
    }
    
    uint8_t* data_m = static_cast<uint8_t*>(mmap(nullptr, 0x1FF, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(MAP_FAILED == data_m){
        std::cout << utils::colors::RED << "Failed to wipe partition: maping failed" << utils::colors::RESET << std::endl;
        close(fd);
        return;
    }

    close(fd);

    for(int i = 0x1BD; i < 0x1CE; i++){
        data_m[i + (index * 16)] = 0;
    }
    munmap(data_m, 0x1FF);
}

void PartitionUtils::erasePartition(const char* path, int index){//partition_definition* definitition){
    wipePartition(path, index);
}

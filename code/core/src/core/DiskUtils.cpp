#include "../../include/core/DiskUtils.hpp"

#define NOT_IMPLEMENTED

using namespace core::utilities;

DiskUtils::DiskUtils(){ 
    partition_definitions = new std::vector<core::partition_definition*>();
}
    
DiskUtils::~DiskUtils(){
    for(auto element = partition_definitions->begin(); element < partition_definitions->end(); element++){
        delete ((core::partition_definition*)*element);
    }
    partition_definitions->clear();
    delete partition_definitions;
}
void DiskUtils::enter(){
    const char* entries[] = {"Load existing virtual disk file", "Create new virtual disk file", "Verify virtual disk file", "Return to BIOS"};
    int choice = -1;
    bool active = true;
    utils::menu::menu_settings settings;
    
    while(active){
        choice = utils::menu::print(entries, 4, "DiskUtils", settings);
        switch(choice){
            case 0:
    	        loadExistingVirtualDiskFile();
            break;
            case 1:
                createNewVirtualDiskFile();
            break;
            case 2:
                verifyVirtualDiskFile();
            break; 
            case 3:
                active = false;
            break;
        }
        settings.clear_cache = true;
        settings.sub_header = last_result_message;
    }
}


void DiskUtils::createNewVirtualDiskFile(){
 std::cout << utils::colors::CLEAR << "This wizard guides you through the creation of a virtual disk file within an existing folder" << std::endl;
    std::string path;
    uint64_t size;
    bool pathIsValid = false;
    std::string response;
    int result;
    while(!pathIsValid){
        std::cout << "Enter disk path (e.g. /home/<USERNAME>/disk.vdf):" << std::endl;
        std::cin >> path;

        result = utils::verifyInput(path);
        if(utils::verify_results::operation_aborted == result){
            last_result_message = utils::colors::YELLOW;
            last_result_message.append("Operation aborted.").append(utils::colors::RESET);
            return;
        }else if(utils::verify_results::input_to_small == result){
            last_result_message = utils::colors::RED;
            last_result_message.append("Given path is invalid.").append(utils::colors::RESET);
        }else if(path[0] != '/' && (path[0] != '.' && path[1] != '/')){
            last_result_message = utils::colors::RED;
            last_result_message.append("Path must start with</> or <./>").append(utils::colors::RESET);
            //std::cout << utils::colors::CLEAR << utils::colors::RED << "Path must start with <" << utils::colors::RESET;
            //std::cout << "/" << utils::colors::RED << "> or <" << utils::colors::RESET << "./";
            //std::cout << utils::colors::RED << ">. Given Path is invalid: " << utils::colors::RESET << path << std::endl;
        }else if((path[path.size() - 4] != '.') && 
                 (path[path.size() - 3] != 'v') && 
                 (path[path.size() - 2] != 'd') && 
                 (path[path.size() - 1] != 'f')){
            path.append(".vdf");
            std::cout << utils::colors::YELLOW << "Added missing File Extension: " << utils::colors::RESET << path << std::endl;
            pathIsValid = true;
        } else {
            pathIsValid = true;
        }

        if(utils::file::exists(path) && pathIsValid) {
            std::cout << utils::colors::YELLOW << path << " already exists. Do you want to overwrite it? [y/n] " << utils::colors::RESET;
            while(std::cin >> response && (response != "n" || response != "y")) {
                if(response == "y") {
                    break;
                } if(response == "n") {
                    pathIsValid = false;
                    last_result_message = utils::colors::YELLOW;
                    last_result_message.append("Operation aborted.").append(utils::colors::RESET);
                    std::cout << utils::colors::CLEAR;
                    break;
                }
            }
            response = ""; 
        }
    }

    std::cout << "Disk size in bytes: ";
    std::cin >> size;
   

    if(size > MAX_DISK_SIZE){
        last_result_message = utils::colors::RED;
        last_result_message.append("Given size is to big.").append(utils::colors::RESET);
        return;
    }
    createDisk(path.c_str(), size);
    last_result_message = utils::colors::GREEN;
    last_result_message.append("Disk created under ").append(path).append(utils::colors::RESET);
    
}
void DiskUtils::loadExistingVirtualDiskFile(){
    std::cout << utils::colors::CLEAR << "This wizard allows you to load partitions from a virtual disk file."<< std::endl 
              << "This will allow the using of these partitions." << std::endl;
    
    std::string path;
    std::cout << "Virtual disk file path: ";
    std::cin >> path;
    int result = utils::verifyInput(path);
    
    if(utils::verify_results::operation_aborted == result){
        last_result_message = utils::colors::YELLOW;
        last_result_message.append("Operation aborted.").append(utils::colors::RESET);
        return;
    }else if(utils::verify_results::input_to_small == result){
        last_result_message = utils::colors::RED;
        last_result_message.append("Given path is invalid.").append(utils::colors::RESET);
        return;
    }else if(path[0] != '/' && (path[0] != '.' && path[1] != '/')){
        last_result_message = utils::colors::RED;
        last_result_message.append("Path must start with</> or <./>").append(utils::colors::RESET);
        return;
    }
    loadDisk(path.c_str());
    last_result_message = utils::colors::GREEN;
    last_result_message.append("Disk loaded from ").append(path).append(utils::colors::RESET);
}

void DiskUtils::verifyVirtualDiskFile(){
    std::cout << utils::colors::CLEAR << "This wizards helps you to verify if a virtual disk file is valid" << std::endl;
    std::string path;
    std::cout << "Virtual disk file path: ";
    std::cin >> path;
    int result = utils::verifyInput(path);
    
    if(utils::verify_results::operation_aborted == result){
        last_result_message = utils::colors::YELLOW;
        last_result_message.append("Operation aborted.").append(utils::colors::RESET);
        return;
    }else if(utils::verify_results::input_to_small == result){
        last_result_message = utils::colors::RED;
        last_result_message.append("Given path is invalid.").append(utils::colors::RESET);
        return;
    }else if(path[0] != '/' && (path[0] != '.' && path[1] != '/')){
        last_result_message = utils::colors::RED;
        last_result_message.append("Path must start with</> or <./>").append(utils::colors::RESET);
        return;
    }
    verifyDisk(path.c_str());
}





// Method for outside usage

void DiskUtils::createDisk(const char* path, uint64_t size){
    if(nullptr == path){
        return;
    }
   if(size < 0){
        std::cerr << utils::colors::RED << "A negative file size was entered: " << size << utils::colors::RESET << std::endl;

        return;
    }    
    if(size > MAX_DISK_SIZE){
        std::cerr << utils::colors::RED << "File size of " << size << " exceeds the maximum limit of " << std::numeric_limits<uint32_t>::max()  << utils::colors::RESET << std::endl; 
        return;
    }
    ProgressBar bar;
    // Perfomace decision. One step0x55ed33485740 is one percent of the size.
    uint32_t step = size / 100;
    bar.maximum(100);
;
    auto fp = fopen(path, "wb");
    if(nullptr == fp){
        std::cout << utils::colors::RED << "Open virtual disk file: " << utils::colors::ICON_DENIED << utils::colors::RESET << std::endl;
        return;
    }
    auto charval = static_cast<uint8_t>(0);
    for(uint32_t i = 0; i < size; i++){
        fwrite (&charval, 1, 1, fp);
        // update if i is a multiple of the step.
        if(i % step == 0){
            bar.update();
        }
    }
    char c0 = 0xC0;
    char version = CURRENT_VIRTUAL_DISK_FILE_VERSION;
    fwrite (&c0, 1, 1, fp);
    fwrite (&version, 1, 1, fp);   

    fclose(fp);
}

bool DiskUtils::verifyDisk(const char* path){
    if(nullptr == path){
        return false;
    }

    if(!utils::file::exists(path)){
        std::cout << utils::colors::YELLOW << "Virtual disk file does not exists" << utils::colors::RESET << std::endl;
        return false;
    }
    
    if(!utils::file::extension_equals(path, "vdf")){
        std::cout << "Extension check [" << utils::colors::ICON_DENIED << "]" << utils::colors::RESET << std::endl;
        std::cout << "Application check [" << utils::colors::ICON_DENIED << "]" << std::endl;
        std::cout << "Version check [" << utils::colors::ICON_DENIED << "]" << std::endl;
        return false;
    }else{
        std::cout << "Extension check [" << utils::colors::ICON_ACCEPT << "]" << utils::colors::RESET << std::endl;
    }


#ifndef NOT_IMPLEMENTED
    uint8_t lastByte = 0xFF;
    uint8_t secondLastByte = 0xFF;
    if(secondLastByte != 0x0C){
        std::cout << "Application check [" << utils::colors::ICON_DENIED << "]" << std::endl;
        return false;
    }else {
        std::cout << "Application check [" << utils::colors::ICON_ACCEPT << "]" << std::endl;
    }
    
    if(lastByte != CURRENT_VIRTUAL_DISK_FILE_VERSION){
        std::cout << "Version check [" << utils::colors::ICON_DENIED << "]" << std::endl;
        return false;
    }else{
        std::cout << "Version check [" << utils::colors::ICON_ACCEPT << "]" << std::endl;
    }
#endif // NOT_IMPLEMENTED
    std::cout << "Virtual disk file verify [" << utils::colors::ICON_ACCEPT << "]" << std::endl;
    return true;
}

void DiskUtils::loadDisk(const char* path){
    if(!verifyDisk(path)){
        return;
    }
    
    core::disk::virtual_disk_file* df = core::disk::load_master_boot_record(path);

    if(df->partition_1 != nullptr){
        partition_definitions->push_back(df->partition_1);
    }
    
    if(df->partition_2 != nullptr){
        partition_definitions->push_back(df->partition_2);
    }
    
    if(df->partition_3 != nullptr){
        partition_definitions->push_back(df->partition_3);
    }
    
    if(df->partition_4 != nullptr){
        partition_definitions->push_back(df->partition_4);
    }

}
#include "../../include/tui/BasiOS.hpp"

using namespace core::os;

BasicOS::BasicOS(){

}

BasicOS::~BasicOS(){

}

int BasicOS::enter(){
    const char* entries[] = {"Workspace", "Create File", "Modify File", "View File", "Delete File", "Scripter", "Shutdown", "Restart"};
    int choice = -1;
    bool active = true;
    int ret = 0;
    std::string str;
    while(active){
        str = "PartitionUtils\nSelected partition: ";
        str.append(std::to_string(partition_index)).append("\n");
        choice = utils::menu::printMenu(str.c_str(), entries, 8, choice);
        switch(choice){
            case 0: workspace();
            break;
            case 1: // Create File
                createFile();
            break;
            break; 
            case 3: // View File
                std::cout << "NOT IMPLEMENTED";
            break;
            case 4: // Delete File
                deleteFile();
            break;
            case 5: 
                std::cout << "NOT IMPLEMENTED";
            break;
            case 6: // Shutdown
                active = false;
                ret = 0;
            break;
            case 7: // Restart
                active = false;
                ret = 100;
            break;
        }
        choice = -1;
    }

    return ret;
}

#include "../../include/core/FAT.hpp"

void BasicOS::createFile(){
    std::cout << utils::colors::CLEAR;

    if(partition_index < 0){
        return;
    }

    std::string name;
    std::string ext;
    std::cout << "Enter file name without extension: ";
    std::cin >> name;
    if(name.size() < 8){
        std::cout << utils::colors::RED << "Name required 8 character" << utils::colors::RED << std::endl;
        return;
    }
    std::cout << "Enter file extension: ";
    std::cin >> ext;
    if(ext.size() < 3){
        std::cout << utils::colors::RED << "Extension required 3 character" << utils::colors::RED << std::endl;
        return;
    }

    core::utilities::DiskUtils& handler = core::utilities::DiskUtils::getInstance();
    if(dynamic_cast<core::file_allocation_table::FATFileSystem*>(handler.loadedPartitions()->at(partition_index)) != nullptr){
        core::file_allocation_table::FATFileSystem* fs = ((core::file_allocation_table::FATFileSystem*)handler.loadedPartitions()->at(partition_index));
        char name2[8] = {name[0], name[1],name[2], name[3],name[4], name[5],name[6], name[7]};
        char ext2[3] = {ext[0], ext[1], ext[2]};
        core::file_allocation_table::root_table* table = fs->get_meta_data(name2, ext2);
        fs->write_file(name2, ext2, 0, 0);
    }


}

void BasicOS::deleteFile(){
    
    if(partition_index < 0){
        return;
    }
    std::cout << utils::colors::CLEAR;
    
    std::string name;
    std::string ext;
    std::cout << "Enter file name without extension: ";
    std::cin >> name;
    if(name.size() < 8){
        std::cout << utils::colors::RED << "Name required 8 character" << utils::colors::RED << std::endl;
        return;
    }
    std::cout << "Enter file extension: ";
    std::cin >> ext;
    if(ext.size() < 3){
        std::cout << utils::colors::RED << "Extension required 3 character" << utils::colors::RED << std::endl;
        return;
    }

    core::utilities::DiskUtils& handler = core::utilities::DiskUtils::getInstance();
    if(dynamic_cast<core::file_allocation_table::FATFileSystem*>(handler.loadedPartitions()->at(partition_index)) != nullptr){
        core::file_allocation_table::FATFileSystem* fs = ((core::file_allocation_table::FATFileSystem*)handler.loadedPartitions()->at(partition_index));
        char name2[8] = {name[0], name[1],name[2], name[3],name[4], name[5],name[6], name[7]};
        char ext2[3] = {ext[0], ext[1], ext[2]};
        core::file_allocation_table::root_table* table = fs->get_meta_data(name2, ext2);
        fs->delete_file(name2, ext2);
    }
}

void BasicOS::workspace(){
    core::utilities::DiskUtils& handler = core::utilities::DiskUtils::getInstance();

    int partition_index = select_loaded_partition();
    this->partition_index = partition_index;
    if(partition_index >= 0 && partition_index < handler.loadedPartitions()->size()){
        bool active = true;
        const char* entries[6] = {"Mount/Unmount", "List Information", "Show Fragmentation", "Defrag", "Show Files", "Return"};
        utils::menu::menu_settings settings;
        settings.row_selection_overflow = true;
        while(active){
            int choice = utils::menu::print(entries, 6, "Select an operation.", settings);
            switch(choice){
                case 0:
                    if(dynamic_cast<core::file_allocation_table::FATFileSystem*>(handler.loadedPartitions()->at(partition_index)) != nullptr){
                        ((core::file_allocation_table::FATFileSystem*)handler.loadedPartitions()->at(partition_index))->toggle_mount();
                    }
                    settings.sub_header = (handler.loadedPartitions()->at(partition_index)->isMounted() ? "Mounted" : "Unmounted");
                break;
                case 1:
                    if(handler.loadedPartitions()->at(partition_index)->isMounted()){
                        std::string str = "Disk path: ";
                        str.append(handler.loadedPartitions()->at(partition_index)->part_definition()->path).append("\n");
                        str.append("File System: ").append(fs_number_to_string(handler.loadedPartitions()->at(partition_index)->part_definition()->file_system_type)).append("\n");
                        str.append("Start address: ").append(std::to_string(handler.loadedPartitions()->at(partition_index)->part_definition()->start_address)).append("\n");
                        str.append("address: ").append(std::to_string(handler.loadedPartitions()->at(partition_index)->part_definition()->end_address)).append("\n");
                        str.append("Block size: ").append(std::to_string(handler.loadedPartitions()->at(partition_index)->part_definition()->block_size)
                           .append(" Cluster size: ").append(std::to_string(handler.loadedPartitions()->at(partition_index)->part_definition()->cluster_size))).append("\n");
                        str.append("Primary: ").append(handler.loadedPartitions()->at(partition_index)->part_definition()->primary ? "True" : " False").append("\n");
                        settings.sub_header = str.c_str();
                    }else{
                        settings.sub_header = "Not mounted";
                    }
                break;


                case 4:
                    if(handler.loadedPartitions()->at(partition_index)->part_definition()->file_system_type == FAT){
                        std::cout << "FILES " << std::endl;
                        ((core::file_allocation_table::FATFileSystem*)handler.loadedPartitions()->at(partition_index))->show_file_allocation_table();
                    }else{
                        std::cout << "NO FILES" << std::endl;
                    }
                    wait(std::cin);
                break;
                case 5:
                    active = false;
                break;
            }
        }
    }
}
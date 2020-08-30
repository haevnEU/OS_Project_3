#include "../../include/tui/BasiOS.hpp"

using namespace core::os;


BasicOS::BasicOS(){ }

BasicOS::~BasicOS(){ }

int BasicOS::enter()
{
    const char* entries[] = {
        "Workspace", 
        "Create File", 
        "Modify File", 
        "View File", 
        "Delete File", 
        "Scripter", 
        "Shutdown", 
        "Restart"
    }; // TUI options

    int choice = -1;
    bool active = true;
    int ret = 0;
    
    while(active)
    {
        choice = utils::menu::printMenu("PartitionUtils", entries, 8, choice);
        switch(choice){
            case 0: workspace();
            break;

            case 1: // Create File
            break;

            case 2: // Modify File
            break; 

            case 3: // View File
            break;

            case 4: // Delete File
            break;

            case 5: // Scripter
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
    }

    return ret;
}

#include "../../include/core/FAT.hpp"

void BasicOS::workspace(){
    core::utilities::DiskUtils& handler = core::utilities::DiskUtils::getInstance();

    int partition_index = select_loaded_partition();

    if(partition_index >= 0 && partition_index < handler.loadedPartitions()->size()){
        bool active = true;
        const char* entries[6] = {"Mount/Unmount", "List Information", "Show Fragmentation", "Defrag", "Show Files", "Return"};
        utils::menu::menu_settings settings;
        settings.row_selection_overflow = true;
        while(active){
            settings.clear_cache = true;    
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
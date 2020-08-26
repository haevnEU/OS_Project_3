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
    while(active){
        choice = utils::menu::printMenu("PartitionUtils", entries, 8, choice);
        switch(choice){
            case 0: // Workspace
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
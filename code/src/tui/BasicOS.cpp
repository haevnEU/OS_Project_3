#include "../../include/tui/BasiOS.hpp"

using namespace core::os;

BasicOS::BasicOS(){

}

BasicOS::~BasicOS(){

}

void BasicOS::enter(){
    const char* entries[] = {"Workspace", "Create File", "Modify File", "View File", "Delete File", "Shutdown", "Restart(N.Impl.)"};
    int choice = -1;
    bool active = true;
    while(active){
        choice = utils::menu::printMenu("PartitionUtils", entries, 7, choice);
        switch(choice){
            case 0:
            break;
            case 1:
            break;
            case 2:
            break; 
            case 3:
            break;
            case 4:
            break;
            case 5:
            active = false;
            break;
            case 6:
            active = false;
            break;
        }
    }
}
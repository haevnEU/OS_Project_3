#include "utils/cli.h"

using namespace utils;

void CLI::addDisk(Disk *disk){}

CLI::CLI(){
    du = new diskUtil();
    currentDisk = nullptr;
}

// TODO place inside disk handler
CLI::~CLI(){
    delete du;
    for (auto i = handler.getDisks()->begin(); i != handler.getDisks()->end(); ++i){
        delete(*i);
        *i = nullptr;
    }
    handler.getDisks()->clear();
    delete handler.getDisks();
}




void CLI::printMenu(){
    utils::printHeader("Utils");
    printSelectedDisk();
    std::cout << "1) Create new Virtual Disk" << std::endl;
    std::cout << "2) Load Virtual Disk" << std::endl;
    std::cout << "3) List loaded Virtual handler.getDisks()" << std::endl;
    if(handler.getDisks()->size() <= 0){
        std::cout << utils::COLOR_YELLOW;
    }
    std::cout << "4) Select List" << std::endl << utils::COLOR_RESET;
    if(nullptr == currentDisk){
        std::cout << utils::COLOR_YELLOW;
    }
    std::cout << "5) Mount selected Virtual Disk" << std::endl;
    std::cout << "6) Unmount selected Virtual Disk" << std::endl;
    if(nullptr == currentDisk || !currentDisk->isMounted()){
        std::cout << utils::COLOR_YELLOW;
    }
    std::cout << "7) Enter Disk Utils" << std::endl << utils::COLOR_RESET;
    std::cout << "8) Enter script mode" << std::endl << utils::COLOR_RESET;

    std::cout << "q) Quit" << std::endl;
}

void CLI::printSelectedDisk(){
    if(nullptr != currentDisk){
        bool mounted = currentDisk->isMounted();
        std::cout << "Mounted: " << (mounted ? utils::ICON_ACCEPT : utils::ICON_DENIED) << utils::COLOR_RESET << std::endl
                  << "Path:    " << currentDisk->path() << std::endl
                  << "Size:    " << currentDisk->size() << std::endl;
    }
}

void CLI::createDisk(){
    utils::printModule("Disk Util", "DiskCreator", "This wizard allows the creation"
                                                   "of a new Virtual Disk File.");
    std::cout << "Enter path: ";
    std::string path = "";
    std::cin >> path;
    
    if(path.size() < 0){  
        utils::printError("Invalid pathing", "create", UNKOWN_ERROR); 
        return;
    }
    if(path[0] == 'q'){
        utils::printWarning("Operation aborted", "create");
        return;
    }
    if(path[0] != '/'){
        utils::printError("Invalid pathing", "create", FILE_WRONG_FILE_SYSTEM);
        return;
    }

    std::cout << "Enter size in bytes: ";
    int64_t size;
    std::cin >> size;
    if(size <= 512){
        utils::printError("Size is invalid", "create", DISC_SIZE_INVALID);
        return;
    }
    Disk* disk = new Disk(path.c_str());
    disk->createDisk(size);
    DiskHandler::getInstance().addDisk(disk);
}

void CLI::loadDisk(){
    utils::printModule("Util", "Load Disk", "This module loads a Virtual Disk File.\nEnter the full path to the VDF file.");
    std::cout << "Path: ";
    std::string path;
    std::cin >> path;
    
    if(path.size() < 0){  
        utils::printError("Invalid pathing", "load", UNKOWN_ERROR); 
        return;
    }
    if(path[0] == 'q'){
        utils::printWarning("Operation aborted", "load");
        return;
    }
    if(path[0] != '/'){
        utils::printError("Invalid pathing", "load", FILE_WRONG_FILE_SYSTEM);
        return;
    }
    
    if(access(path.c_str(), F_OK ) == -1) {
        utils::printError("File does not exists", "load", FILE_IO_NOT_FOUND);
    }else{
        
        std::cout << "Disk loaded" << std::endl;
        Disk* disk = new Disk(path.c_str());
        DiskHandler::getInstance().addDisk(disk);
    }
}

void printElement(Disk* disk, int index, char filler = ' '){
    bool mounted = disk->isMounted();

    std::cout << std::setw(5) << std::setfill(' ') << index << " " << filler
              << "    " << (mounted ? utils::ICON_ACCEPT : utils::ICON_DENIED)
              << "    " << filler << ' '
              << disk->path();
}

void CLI::listDisks(){
    printHeader("Utils");
    int idx = 1;
    std::cout << std::endl << "Index | Mounted | Path" << std::endl;
    std::cout << "------+---------+-------------------------" << std::endl;
    for (auto i = handler.getDisks()->begin(); i != handler.getDisks()->end(); ++i){
        printElement(((Disk*)*i), idx);
        std::cout <<std::endl;
        idx++;
    }
}

void CLI::selectDisk(){
    if(handler.getDisks()->size() <= 0){
        utils::printError("No disks loaded", "select", INDEX_OUT_OF_BOUND);
        return;
    }
    listDisks();
    std::cout << "Back) 0" << std::endl;
    int choice;
    std::cout << "Choice: ";
    std::cin >> choice;
    if(choice == 'q' || choice == 0){
        return;
    }
    choice--;
    if(choice < handler.getDisks()->size()){
       currentDisk = handler.getDisks()->at(choice);
    }else{
        utils::printError("Index out of bound", "select", INDEX_OUT_OF_BOUND);
    }
}

void CLI::mountDisk(){
    if(nullptr == currentDisk){
        utils::printError("No disk selected", "mount", DISK_NULL);
        return;
    }
    currentDisk->mount();
}

void CLI::unmountDisk(){
    if(nullptr == currentDisk){
        utils::printError("No disk selected", "unmount", DISK_NULL);
        return;
    }
    currentDisk->unmount();
}

void CLI::startDiskUtils(){
    if(nullptr == currentDisk){
        utils::printError("No disk selected", "start utils",DISK_NULL);
        return;
    }

    if(!currentDisk->isMounted()){
        utils::printError("Selected disk is not mounted", "start utils", DISC_NOT_MOUNTED);
        return;
    }

    du->show(currentDisk);
}

void CLI::scriptMode(){
    utils::printModule("Utils", "Scripter", "This module allows the execution of lua scripts.\n"
                                            "Entering a path to a script will execute it.\n"
                                            "It is possible to call the LUA API directly.\n"
                                            "For information about the API visit the documentation.\n"
                                            "Hit q to quit this mode");

    std::cout << "Executing script: Enter the full path to the script file" << std::endl
              << "Executing a command: Enter the LUA command" << std::endl
              << "Quit this mode: Type q" << std::endl
              << "Request this help: Type ?" << std::endl;

    wait(std::cin, "Hit [ENTER] to start");
    std::cout << utils::TERMINAL_CLEAR << std::endl;
    API api = API::getInstance();
    while(true){
        std::string in;
        std::cout << ">" << utils::COLOR_BLUE;
        std::cin >> in;
        std::cout << utils::COLOR_RESET;
        if(in.size() > 0){
            if(in[0] == 'q'){
                return;
            }else if(in[0] == '/'){
                API::getInstance().executeScript(in.c_str());
            }else if(in[0] == '?'){
                std::cout << "Executing script: Enter the full path to the script file" << std::endl
                          << "Executing a command: Enter the LUA command" << std::endl
                          << "Quit this mode: Type q" << std::endl
                          << "Request this help: Type ?" << std::endl;
            }else{
                api.executeCommand(in.c_str());
            }

        }
    }
}

void CLI::enterInteractiveMode(){
    bool active = true;
    while(active){

        printMenu();
        char choice;
        std::cout << "Choice: ";
        std::cin >> choice;
        switch(choice){
        case '1':
            createDisk();
            break;
        case '2':
            loadDisk();
            break;
        case '3':
            listDisks();
            break;
        case '4':
            selectDisk();
            break;
        case '5':
            mountDisk();
            break;
        case '6':
            unmountDisk();
            break;
        case '7':
            startDiskUtils();
            break;
        case '8':
            scriptMode();
            break;

        case 'q':
            return;
        }
        wait(std::cin);
    }
}

#include "api.h"

API::API(){

    L = luaL_newstate();

    // 2. Open required libraries
    luaL_openlibs(L);
    registerAPI();
}

void API::close(){
    lua_close(L);
}


API::~API(){}

void API::registerAPI(){    

    lua_pushcfunction(L, LUA_api_help);
    lua_setglobal(L, "api");

    lua_pushcfunction(L, LUA_print);
    lua_setglobal(L, "print");


    lua_pushcfunction(L, LUA_clear_screen);
    lua_setglobal(L, "clear");

    lua_pushcfunction(L, LUA_clear_screen);
    lua_setglobal(L, "cls");


    lua_pushcfunction(L, LUA_create_new_disk);
    lua_setglobal(L, "createDisk");

    lua_pushcfunction(L, LUA_load_disk);
    lua_setglobal(L, "loadDisk");

    lua_pushcfunction(L, LUA_select_disk);
    lua_setglobal(L, "selectDisk");

    lua_pushcfunction(L, LUA_mount_disk);
    lua_setglobal(L, "mount");

    lua_pushcfunction(L, LUA_unmount_disk);
    lua_setglobal(L, "unmount");
    

    lua_pushcfunction(L, LUA_create_master_boot_record);
    lua_setglobal(L, "createMasterBootRecord");
    
    lua_pushcfunction(L, LUA_list_master_boot_record);
    lua_setglobal(L, "listMasterBootRecord");
    
    lua_pushcfunction(L, LUA_wipe_master_boot_record);
    lua_setglobal(L, "wipeMasterBootRecord");
    
    lua_pushcfunction(L, LUA_create_createPartition_boot_record);
    lua_setglobal(L, "createPartition");

    lua_pushcfunction(L, LUA_delete_partition);
    lua_setglobal(L, "deletePartition");

    lua_pushcfunction(L, LUA_wipe_partition);
    lua_setglobal(L, "wipePartition");
}

void API::executeScript(const char* path){
    std::cout << path << std::endl;
    int result = luaL_dofile(L, path);
    if (result != LUA_OK) {
        printError(lua_tostring(L, -1), path);
    }
}

void API::executeCommand(const char* command){
    int result = luaL_dostring(L, command);
    if (result != LUA_OK) {
        printError(lua_tostring(L, -1), "command");        
    }
}


void API::printError(const char* message){ 
    auto end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cerr << utils::COLOR_RED 
              << "Time: " << std::ctime(&end_time) << std::endl
              << "Message: " << message << std::endl 
              << utils::COLOR_RESET << std::endl;
}

void API::printError(const char* message, int code){ 
    auto end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cerr << utils::COLOR_RED 
              << "Time: " << std::ctime(&end_time) << std::endl
              << "Message: " << message << std::endl 
              << "Code: " << code2str(code) << "(" << std::hex << code << std::dec << ")" << std::endl 
              << utils::COLOR_RESET << std::endl;
}

void API::printError(const char* message, const char* syntax){     
    auto end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cerr << utils::COLOR_RED 
              << "Time: " << std::ctime(&end_time) << std::endl
              << "Message: " << message << std::endl 
              << "Syntax: " << syntax << std::endl 
              << utils::COLOR_RESET << std::endl;
}

int API::LUA_api_help(lua_State* L){
    std::cout << utils::COLOR_YELLOW 
    << "API commands" << std::endl
    << "• " << api_commands.print << std::endl
    << "• " << api_commands.clear << std::endl
    << "• " << api_commands.createDisk << std::endl
    << "• " << api_commands.loadDisk << std::endl
    << "• " << api_commands.listDisk << std::endl
    << "• " << api_commands.selectDisk << std::endl
    << "• " << api_commands.mount << std::endl
    << "• " << api_commands.unmount << std::endl
    << "• " << api_commands.createMasterBootRecord << std::endl
    << "• " << api_commands.listMasterBootRecord << std::endl
    << "• " << api_commands.wipeMasterBootRecord << std::endl
    << "• " << api_commands.createPartition << std::endl
    << "• " << api_commands.deletePartition << std::endl
    << "• " << api_commands.wipePartition << utils::COLOR_RESET << std::endl;
    return 0;

}

int API::LUA_print(lua_State *L){
    std::cout << lua_tostring(L, -1) << std::endl;
    return 0;
}

int API::LUA_clear_screen(lua_State *L){
    std::cout << utils::TERMINAL_CLEAR;
    return 0;
}

int API::LUA_create_new_disk(lua_State* L){
    if(!lua_isnumber(L, -1) || !lua_isstring(L, -2)){
        printError("One argument is not valid.", api_commands.createDisk);
        return 0;
    }


    const char* path = lua_tostring(L, -2);
    uint32_t size = (uint32_t)lua_tonumber(L, -1);
    Disk* disk = new Disk(path);
    disk->createDisk(size);
    if(OPERATION_SUCCEED == disk->lastError()){  
        std::cout << utils::COLOR_GREEN << "Disk created" << utils::COLOR_RESET << std::endl;
        DiskHandler::getInstance().addDisk(disk);
    }else{
        printError("Operation failed.", disk->lastError());
    }
    return 0;
}

int API::LUA_load_disk(lua_State* L){
    if(!lua_isstring(L, -1)){
        printError("One argument is not valid.", api_commands.loadDisk);
        return 0;
    }
    const char* path = lua_tostring(L, -1);


    Disk* disk = new Disk(path);
    DiskHandler::getInstance().addDisk(disk);
    if(OPERATION_SUCCEED == DiskHandler::getInstance().lastError()){
        std::cout << utils::COLOR_GREEN << "Disk added" << utils::COLOR_RESET << std::endl;
    }else{
        printError("Operation failed.", disk->lastError());
    }
    return 0;
}

int API::LUA_delete_disk(lua_State *L){
    return 0;
}

 int API::LUA_select_disk(lua_State *L){
    if(!lua_isnumber(L, -1)){
        printError("One argument is invalid", api_commands.selectDisk);
        return 0;
    }     
    int index = lua_tonumber(L, -1);
    DiskHandler& handler = DiskHandler::getInstance();
    if(index < 0 || index >= handler.amountDisks()){
        printError("Operation failed. Index is out of bounds");
        return 0;
    }


    handler.setDisk(index);
    if(OPERATION_SUCCEED == DiskHandler::getInstance().lastError()){
        std::cout << utils::COLOR_GREEN << "Disk selected" << utils::COLOR_RESET << std::endl;
    }else{        
        printError("Operation failed.", DiskHandler::getInstance().lastError());
    }
    return 0;

 }

 int API::LUA_mount_disk(lua_State *L){
    DiskHandler& handler = DiskHandler::getInstance();
    Disk* disk = handler.disk;
    if(nullptr == disk){
        printError("Selected disk is nullptr");
        return 0;
    }


    disk->mount();
    if(OPERATION_SUCCEED == disk->lastError()){  
        std::cout << utils::COLOR_GREEN << "Disk mounted" << utils::COLOR_RESET << std::endl;
        DiskHandler::getInstance().addDisk(disk);
    }else{
        printError("Operation failed.", disk->lastError());
    }
    return 0;
 }

 int API::LUA_unmount_disk(lua_State *L){
    DiskHandler& handler = DiskHandler::getInstance();
    Disk* disk = handler.getDisk();
    if(nullptr == disk){
        printError("Selected disk is nullptr");
        return 0;
    }


    disk->unmount();
    if(OPERATION_SUCCEED == disk->lastError()){
        std::cout << utils::COLOR_RED << "Disk not unmounted" << utils::COLOR_RESET << std::endl;
    }else{
        std::cout << utils::COLOR_GREEN << "Disk unmounted" << utils::COLOR_RESET << std::endl;
    }
    return 0;
 }

int API::LUA_create_master_boot_record(lua_State* L){
    DiskHandler& handler = DiskHandler::getInstance();
    Disk* disk = handler.getDisk();    
    if(nullptr == disk){
        printError("Selected disk is nullptr");
        return 0;
    }
    if(!disk->isMounted()){
        printError("Selected disk is not mounted");
        return 0;
    }
 

    disk->createMasterBootRecord();
    if(OPERATION_SUCCEED == disk->lastError()){  
        std::cout << utils::COLOR_GREEN << "Master boot record created" << utils::COLOR_RESET << std::endl;
        DiskHandler::getInstance().addDisk(disk);
    }else{
        printError("Operation failed.", disk->lastError());
    }
    return 0;
}
    
int API::LUA_list_master_boot_record(lua_State* L){
    DiskHandler& handler = DiskHandler::getInstance();
    Disk* disk = handler.getDisk();    
    if(nullptr == disk){
        printError("Selected disk is nullptr");
        return 0;
    }
    if(!disk->isMounted()){
        printError("Selected disk is not mounted");
        return 0;
    }

    if(nullptr == disk->MBR()){
        printError("Selected disk has no master boot record");
        return 0;
    }


    std::cout << "Size of disk: " << disk->size() << std::endl
              << "Mounted: " << (disk->isMounted() ? utils::ICON_ACCEPT : utils::ICON_DENIED) << std::endl;
    for(int i = 0; i < 4; ++i){
        Partition* partition = disk->MBR()->partition(i);
        std::cout << "+------------------------------" << std::endl;
        if(nullptr == partition){
            std::cout << "| Partition No: " << i << ": " << std::endl
                      << "| \t" << utils::COLOR_MAGENTA << "NULL" << utils::COLOR_RESET << std::endl
                      << "+------------------------------" << std::endl;
        }else{
            if(partition->startAddress() == 0x00 && partition->size() == 0){
                std::cout << "| Partition No: " << i << ": " << std::endl
                          << "| \t" << utils::COLOR_MAGENTA << "NULL" << utils::COLOR_RESET << std::endl
                          << "+------------------------------" << std::endl;
            }else{
                std::cout << "| Partition No: " << i << utils::COLOR_CYAN
                          << utils::COLOR_RESET << std::endl
                          << "| \t" << utils::COLOR_CYAN << "Startaddress: 0x" << std::hex << std::setw(8) << std::setfill('0') << partition->startAddress()
                          << utils::COLOR_RESET << std::endl
                          << "| \t" << utils::COLOR_CYAN << "Endaddress:   0x" << std::hex << std::setw(8) << std::setfill('0') << (partition->size() + partition->startAddress())
                          << utils::COLOR_RESET << std::endl
                          << "| \t" << utils::COLOR_CYAN << "Size:         "   << std::dec << partition->size()
                          << utils::COLOR_RESET << std::endl
                          << "| \t" << utils::COLOR_CYAN << "Primary:      "   << (partition->isPrimary() ? utils::ICON_ACCEPT : utils::ICON_DENIED)
                          << utils::COLOR_RESET << std::endl
                          << "| \t" << utils::COLOR_CYAN << "Filesystem:   "   << getFileSystemName(partition->fileSystemType())
                          << utils::COLOR_RESET << std::endl
                          << "+------------------------------" << utils::COLOR_RESET << std::endl;
                std::cout << utils::COLOR_RESET;
            }
        }
    }
    std::cout << std::endl;
    return 0;
}
    
int API::LUA_wipe_master_boot_record(lua_State* L){
    std::cout << utils::COLOR_YELLOW << "DEPRECATED CALL" << utils::COLOR_RESET << std::endl;
    DiskHandler& handler = DiskHandler::getInstance();
    Disk* disk = handler.getDisk();    
    if(nullptr == disk){
        printError("No disk was selected");
        return 0;
    }
    if(!disk->isMounted()){
        printError("Selected disk is not mounted");
        return 0;
    }


    disk->createMasterBootRecord();
    if(OPERATION_SUCCEED == disk->lastError()){  
        std::cout << utils::COLOR_GREEN << "Disk cleared" << utils::COLOR_RESET << std::endl;
        DiskHandler::getInstance().addDisk(disk);
    }else{
        printError("Operation failed.", disk->lastError());
    }
    return 0;
}

int API::LUA_create_createPartition_boot_record(lua_State* L){
    int64_t size;
    char primary;
    int64_t file_system;
    
    if(!lua_isnumber(L, 1)){
        printError("One argument is invalid", api_commands.createPartition);
        return 0;
    }else{
        size = lua_tointeger(L, 1);
    }

    if(!lua_isstring(L, 2)){
        printError("One argument is invalid", api_commands.createPartition);
        return 0;
    }else{
        primary = lua_tostring(L, 2)[0];
    }

    if(!lua_isnumber(L, 1)){
        printError("One argument is invalid", api_commands.createPartition);
        return 0;
    }else{
        file_system = lua_tointeger(L, 3);
    }
    bool primary_b = true;
    if(primary == 'n' || primary == 'N'){
        primary_b = false;
    }

    DiskHandler& handler = DiskHandler::getInstance();
    Disk* disk = handler.getDisk();    
    if(nullptr == disk){
        printError("No disk was selected");
        return 0;
    }
    if(!disk->isMounted()){
        printError("Selected disk is not mounted");
        return 0;
    }
    if(nullptr == disk->MBR()){
        printError("Selected disk has no master boot record");
        return 0;
    }


    disk->createPartition(size, numberToFileSystemTyp(file_system), primary_b);
    if(OPERATION_SUCCEED == disk->lastError()){  
        std::cout << utils::COLOR_GREEN << "Partition created";
        DiskHandler::getInstance().addDisk(disk);
    }else{
        printError("Operation failed.", disk->lastError());
    }
    return 0;
}



int API::LUA_delete_partition(lua_State* L){
    if(!lua_isnumber(L, 1)){
        printError("One argument is invalid", api_commands.deletePartition);
        return 0;
    }
    int index = lua_tointeger(L, 1);
    
    if(index < 0 || index > 3){
        printError("Index is out of bounds");
        return 0;
    }

    Disk* disk = DiskHandler::getInstance().getDisk();
    if(nullptr == disk){
        printError("Selected disk is nullptr");
        return 0;
    }
    

    disk->removePartition(index);
    if(OPERATION_SUCCEED == disk->lastError()){  
        std::cout << utils::COLOR_GREEN << "Partition removed" << utils::COLOR_RESET << std::endl;
        DiskHandler::getInstance().addDisk(disk);
    }else{
        printError("Operation failed.", disk->lastError());
    }
    return 0;
}


int API::LUA_wipe_partition(lua_State* L){
    if(!lua_isnumber(L, 1)){
        printError("One argument is invalid", api_commands.wipePartition);
        return 0;
    }
    int index = lua_tointeger(L, 1);
    
    if(index < 0 || index > 3){
        std::cerr << utils::COLOR_RED << "Index is out of bounds" << utils::COLOR_RESET << std::endl;
        printError("Index out of bounds");
        return 0;
    }
    
    Disk* disk = DiskHandler::getInstance().getDisk();
    if(nullptr == disk){
        printError("Selected disk is nullptr");
        std::cerr << utils::COLOR_RED << "Selected disk is nullptr" << utils::COLOR_RESET << std::endl;
        return 0;
    }


    disk->wipePartition(index);    
    if(OPERATION_SUCCEED == disk->lastError()){  
        std::cout << utils::COLOR_GREEN << "Partition wiped" << utils::COLOR_RESET << std::endl;
        DiskHandler::getInstance().addDisk(disk);
    }else{
        printError("Operation failed.", disk->lastError());
    }
    return 0;
}




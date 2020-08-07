#include "api.h"
#include "disk.h"
#include "diskhandler.h"
#include "utils/cli.h"

API::API(){

    L = luaL_newstate();

    // 2. Open required libraries
    luaL_openlibs(L);

    registerAPI();
}

void
API::close(){
    lua_close(L);
}


API::~API(){
}

void API::registerAPI(){

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

    lua_pushcfunction(L, LUA_list_disk);
    lua_setglobal(L, "listDisks");

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

}

void API::executeScript(const char* path){
    std::cout << path<< std::endl;
    int result = luaL_dofile(L, path);
    if (result != LUA_OK) {
        std::cout << utils::COLOR_RED << lua_tostring(L,-1) << utils::COLOR_RESET << std::endl;
    }
}

void API::executeCommand(const char* command){
    int result = luaL_dostring(L, command);
    if (result != LUA_OK) {
        std::cout << utils::COLOR_RED << lua_tostring(L,-1) << utils::COLOR_RESET << std::endl;
    }
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
        utils::printError("Wrong parameter.");
        return 0;
    }
    const char* path = lua_tostring(L, -2);
    uint32_t size = (uint32_t)lua_tonumber(L, -1);
    Disk* disk = new Disk(path);
    disk->createDisk(size);
    DiskHandler::getInstance().addDisk(disk);
    return 0;
}

int API::LUA_load_disk(lua_State* L){
    if(!lua_isstring(L, -1)){
        utils::printError("Wrong parameter.");
        return 0;
    }
    const char* path = lua_tostring(L, -1);
    Disk* disk = new Disk(path);
    DiskHandler::getInstance().addDisk(disk);
    return 0;
}

int API::LUA_list_disk(lua_State* L){
    int idx = 1;
    std::cout << std::endl << "Index | Mounted | Path" << std::endl;
    std::cout << "------+---------+-------------------------" << std::endl;
    DiskHandler& handler = DiskHandler::getInstance();
    for (auto i = handler.getDisks()->begin(); i != handler.getDisks()->end(); ++i){
        printElement(((Disk*)*i), idx);
        std::cout <<std::endl;
        idx++;
    }
    return 0;
}

int API::LUA_delete_disk(lua_State *L){
    return 0;
}

 int API::LUA_select_disk(lua_State *L){
    if(!lua_isnumber(L, -1)){
        utils::printError("Argument is not a number");
        return 0;
    }     
    int index = lua_tonumber(L, -1);;
    DiskHandler& handler = DiskHandler::getInstance();
    handler.setDisk(index);
    return 0;

 }

 int API::LUA_mount_disk(lua_State *L){
    DiskHandler& handler = DiskHandler::getInstance();
    Disk* disk = handler.disk;
    if(nullptr == disk){
        utils::printError("No disk was selected");
        return 0;
    }
    disk->mount();
    return 0;
 }

 int API::LUA_unmount_disk(lua_State *L){
    DiskHandler& handler = DiskHandler::getInstance();
    Disk* disk = handler.getDisk();
    if(nullptr == disk){
        utils::printError("No disk was selected");
        return 0;
    }
    disk->unmount();
    return 0;
 }

int API::LUA_create_master_boot_record(lua_State* L){
    DiskHandler& handler = DiskHandler::getInstance();
    Disk* disk = handler.getDisk();    if(nullptr == disk){
        utils::printError("No disk was selected");
        return 0;
    }
    if(!disk->isMounted()){
        utils::printError("Selected disk is not mounted");
        return 0;
    }

    disk->createMasterBootRecord();

    return 0;
}
    
int API::LUA_list_master_boot_record(lua_State* L){
    DiskHandler& handler = DiskHandler::getInstance();
    Disk* disk = handler.getDisk();    if(nullptr == disk){
        utils::printError("No disk was selected");
        return 0;
    }
    if(!disk->isMounted()){
        utils::printError("Selected disk is not mounted");
        return 0;
    }

   if(nullptr == disk->MBR()){
        utils::printError("Selected disk has no master boot record");
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
    DiskHandler& handler = DiskHandler::getInstance();
    Disk* disk = handler.getDisk();    if(nullptr == disk){
        utils::printError("No disk was selected");
        return 0;
    }
    if(!disk->isMounted()){
        utils::printError("Selected disk is not mounted");
        return 0;
    }

    disk->createMasterBootRecord();

    return 0;
}

int API::LUA_create_createPartition_boot_record(lua_State* L){
    int64_t size;
    char primary;
    int64_t file_system;
    
    if(!lua_isnumber(L, 1)){
        utils::printError("First argument is not a number");
        return 0;
    }else{
        size = lua_tointeger(L, 1);
    }

    if(!lua_isstring(L, 2)){
        utils::printError("Second argument is not a valid string");
        return 0;
    }else{
        primary = lua_tostring(L, 2)[0];
    }

    if(!lua_isnumber(L, 1)){
        utils::printError("Third argument is not a number");
        return 0;
    }else{
        file_system = lua_tointeger(L, 3);
    }
    bool primary_b = true;
    if(primary == 'n' || primary == 'N'){
        primary_b = false;
    }

    DiskHandler& handler = DiskHandler::getInstance();
    Disk* disk = handler.getDisk();    if(nullptr == disk){
        utils::printError("No disk was selected");
        return 0;
    }
    if(!disk->isMounted()){
        utils::printError("Selected disk is not mounted");
        return 0;
    }
    if(nullptr == disk->MBR()){
        utils::printError("Selected disk has no master boot record");
        return 0;
    }


    disk->createPartition(size, numberToFileSystemTyp(file_system), primary_b);

    return 0;
}







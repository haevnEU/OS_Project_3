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
    lua_setglobal(L, "listDisk");



}

void API::executeScript(const char* path){
    std::cout << path<< std::endl;
    int result = luaL_dofile(L, path);
    if (result != LUA_OK) {
        std::cout << "[" << utils::ICON_DENIED << "][" << result << "]" << lua_tostring(L,-1) << std::endl;
    }
}

void API::executeCommand(const char* command){
    int result = luaL_dostring(L, command);
    if (result != LUA_OK) {
        std::cout << "[" << utils::ICON_DENIED << "][" << result << "]" << lua_tostring(L,-1) << std::endl;
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
    if(!lua_isnumber(L, -1) || !lua_isstring(L, -1)){
        std::cout << "Wrong parameter. Usage: createDisk(string, number);" << std::endl;
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
        std::cout << "Wrong parameter. Usage: createDisk(string);" << std::endl;
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
     int index = -1;
     DiskHandler& handler = DiskHandler::getInstance();

     if(lua_isnumber(L, -1)){
        index = lua_tonumber(L, -1);
        handler.setDisk(index);
     }else{
         std::cout << "Argument is not a number." << std::endl;
     }
     return 0;
 }

 int API::LUA_mount_disk(lua_State *L){
     return 0;
 }

 int API::LUA_unmount_disk(lua_State *L){
     return 0;
 }







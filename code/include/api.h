#ifndef API_H
#define API_H

#include <iostream>
#include <ctime>
#include <chrono>

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/luaconf.h"
#include "lua/lauxlib.h"
}


#include "disk.h"
#include "diskhandler.h"

#include "lua/luaapihelp.h"

#include "utils/cli.h"
#include "utils/utils.h"


static lua_State* L;
/**
 * @brief The API class
 * @details This class provides an API for the application.
 *          As scripting language lua is choosen.
 */
class API{
private methods:

    API();
    void registerAPI();

public static_methods:

    /**
     * @brief getInstance
     * @details This function returns the instance of this class
     * @return Instance of this class
     */
    static API getInstance(){
        static API instance;
        return instance;
    }

public static_methods: // API Methods

    static int LUA_api_help(lua_State* L);

    static int LUA_print(lua_State* L);

    static int LUA_clear_screen(lua_State* L);

    static int LUA_create_new_disk(lua_State* L);


    static int LUA_load_disk(lua_State* L);

    static int LUA_delete_disk(lua_State *L);

    static int LUA_select_disk(lua_State *L);

    static int LUA_mount_disk(lua_State *L);

    static int LUA_unmount_disk(lua_State *L);


    static int LUA_create_master_boot_record(lua_State* L);
    
    static int LUA_list_master_boot_record(lua_State* L);
    
    static int LUA_wipe_master_boot_record(lua_State* L);

    
    static int LUA_create_createPartition_boot_record(lua_State* L);

    static int LUA_delete_partition(lua_State* L);

    static int LUA_wipe_partition(lua_State* L);
    

    static void printElement(Disk* disk, int index, char filler = ' '){
        bool mounted = disk->isMounted();

        std::cout << std::setw(5) << std::setfill(' ') << index << " " << filler
                  << "    " << (mounted ? "\x1B[32m\xE2\x9C\x93" : "\x1B[31mx") << utils::COLOR_RESET
                  << "    " << filler << ' '
                  << disk->path();
    }

private methods:
    static void printError(const char* message);
    static void printError(const char* message, int code);
    static void printError(const char* message, const char* syntax);

public methods:

    void close();

    ~API();

    /**
     * @brief executeScript
     * @details This method executes a script from the hard disk
     * @param path Path to the disk
     */
    void executeScript(const char* path);

    /**
     * @brief executeCommand
     * @details This method executes a LUA command.
     * @param command Command which should be executed
     */
    void executeCommand(const char* command);
};

#endif // API_H

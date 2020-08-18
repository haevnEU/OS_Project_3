#ifndef LUAAPIHELP_H
#define LUAAPIHELP_H

struct lua_api_commands{
    const char* print = "print(string message) or print(int number)";
    const char* clear = "clear()";
    const char* createDisk = "createDisk(string path, int size)";
    const char* loadDisk = "loadDisks(string path)";
    const char* listDisk = "listDisk()";
    const char* selectDisk = "selectDisk(int index)";
    const char* mount = "mount()";
    const char* unmount = "unmount()";
    const char* createMasterBootRecord = "createMasterBootRecord()";
    const char* listMasterBootRecord = "listMasterBootRecord()";
    const char* wipeMasterBootRecord = "wipeMasterBootRecord()";
    const char* createPartition = "createPartition(int size, char primary, int fileSystem)";
    const char* deletePartition = "deletePartition(int index)";
    const char* wipePartition = "wipePartition(int index)";
};

static lua_api_commands api_commands;

#endif //LUAAPIHELP_H
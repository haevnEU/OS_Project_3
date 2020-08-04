#ifndef MBRUTILS_H
#define MBRUTILS_H

#include "utils.h"

#include "disk.h"
#include "FileSystemType.h"

class MBRUtils{

    Disk* disk;

public:
    MBRUtils();

    void setDisk(Disk* disk);

    void listMasterBootRecord();

    void createMasterBootRecord();

    void wipeMasterBootRecord();

    void enterInteractiveMode();

    void leaveInteractiveMode();
};

#endif // MBRUTILS_H

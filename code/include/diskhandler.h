#ifndef DISKHANDLER_H
#define DISKHANDLER_H

#include <vector>

#include "utils/utils.h"
#include "disk.h"

class DiskHandler{

private attributes:

private methods:

    DiskHandler();

    /// Contains all loaded disks
    std::vector<Disk*>* disks;
public static_methods:
    static DiskHandler& getInstance(){
        static DiskHandler instance;
        return instance;
    }

public methods:

    DiskHandler(DiskHandler const&) = delete;
    void operator=(DiskHandler const&) = delete;

    void addDisk(Disk* disk);

    std::vector<Disk*>* getDisks() const;
    Disk* disk;

    Disk* getDisk() const;
    void setDisk(int index);
};

#endif // DISKHANDLER_H

#ifndef FILESYSTEMTYPE_H
#define FILESYSTEMTYPE_H

enum FileSystemType{
    NONE = 0x00,
    FAT = 0x04,
    FAT_HIDDEN = 0x14,
    INODE = 0x83
};

/**
 * @brief getFileSystemName
 * @details Converts a FileSystem Enum to string
 * @param fileSystemType ...
 * @return String representation of the enum
 */
static const char* getFileSystemName(FileSystemType fileSystemType){
    switch(fileSystemType){
        case FileSystemType::NONE: return "None";
        case FileSystemType::FAT: return "FAT";
        case FileSystemType::FAT_HIDDEN: return "FAT_HIDDEN";
        case FileSystemType::INODE: return "INODE";
    }
    return "UNKNOWN";
}

/**
 * @brief numberToFileSystemTyp
 * @details Converts an unsinged char to a filesystem enum
 * @param fileSystemType ...
 * @return Enum representation of the given param
 */
static FileSystemType numberToFileSystemTyp(unsigned char fileSystemType){
    switch(fileSystemType){
        case 0x04: return FileSystemType::FAT;
        case 0x14: return FileSystemType::FAT_HIDDEN;
        case 0x83: return FileSystemType::INODE;
    }
    return FileSystemType::NONE;
}
#endif // FILESYSTEMTYPE_H

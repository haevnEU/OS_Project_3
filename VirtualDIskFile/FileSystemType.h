#ifndef FILESYSTEMTYPE_H
#define FILESYSTEMTYPE_H

enum FileSystemType{
    NONE = 0,
    FAT = 16,
    INODE = 32
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
        case 0: return FileSystemType::NONE;
        case 16: return FileSystemType::FAT;
        case 32: return FileSystemType::INODE;
        default: return FileSystemType::NONE;
    }
}
#endif // FILESYSTEMTYPE_H

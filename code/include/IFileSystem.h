#ifndef _IFILE_SYSTEM_H_
#define _IFILE_SYSTEM_H_

#include "utils/utils.h"

class IFileSystem{
    public methods:

    void init() = 0;

    virtual bool createFile(const char* path) = 0;
    virtual bool fileExists(const char* path) = 0;
    virtual bool deleteFile(const char* path) = 0;
    virtual bool writeFile(const char* path, const char* data) = 0;
    virtual bool appendFile(const char* path, const char* data) = 0;

    virtual double calculateFreeSpace() = 0;
    virtual void format() = 0;
    virtual void defrag() = 0;
};

#endif //_IFILE_SYSTEM_H_

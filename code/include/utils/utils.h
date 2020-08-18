#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <iomanip>
#include <cstring>
#include <limits>

#define getter
#define setter

#define methods
#define attributes

#define static_methods
#define static_attributes

#define CURRENT_VIRTUAL_DISK_FILE_VERSION 0x02
#define MINIMUM_VIRTUAL_DISK_FILE_VERSION 0x02

#define UNKOWN_ERROR            0x05FFFFFF // This is an unknown exception, should never occur

#define FILE_IO_NOT_FOUND       0x05F10404 // File not found
#define FILE_WRONG_FILE_SYSTEM  0x51F1BADF // The input doenst started with /
#define FILE_WRONG_PATH         0x51F1BADA // The path is invalid

#define INDEX_OUT_OF_BOUND      0x51A1FFFF // Index was out of boundary

#define DISK_NULL               0x51B1DEAD // Disc is nullptr
#define DISC_SIZE_INVALID       0x51B10101 // Requested disk size is not possible to create
#define DISC_NOT_MOUNTED        0x05B12003 // Disc is not mounted

#define MBR_NULL                0x05C1DEAD // MBR is nullptr
// 05xxxxxx = This project
// xxD1xxxx = Disk module
// xxxx0xxx = General error
// xxxx1xxx = File IO catergory
// xxxx2xxx = State error
#define DISC_GENERIC_ERROR   0x05D10000 // Generic disc error
#define DISC_CORRUPTED       0x05D10BAD // Disc is corrupted

#define DISC_PATHING_ERROR   0x05D11001 // Path input dont started with a /
#define DISC_FILE_IO_ERROR   0x05D11002 // File operation failed
#define DISC_FILE_NOT_FOUND  0x05D11003 // VDF file not found

#define DISC_NOT_LOAD        0x05D12002 // Disc is not loaded

// General result codes
#define OPERATION_SUCCEED 0x00000000
#define UNKNOWN_ERROR     0xFFFFFFFF
#define INDEX_OUT_OF_BOUNDS 0x00005555
#define NOT_IMPLEMENTED 0xABCD0123

// File IO result codes
#define FILE_NOT_FOUND        0x01F10404
#define FILE_VERSION_MISMATCH 0x01F10BAD
#define FILE_OPEN_FAILED      0x01F1FF04

// Memory management result codes
#define MAPPING_FAILED 0x01AADEAD
#define STAT_FAILED    0x01AADEFF

// Disk result codes
#define DISK_ALREADY_LOADED 0x01D10500
#define REQUESTED_DISK_SIZE_NEGATIVE 0x01D10002
#define REQUESTED_DISK_SIZE_TO_BIG   0x01D1FFF0
#define DISK_UNMOUNTED   0x01D10010
#define DISK_MOUNTED     0x01D10011

// Master boot record result codes
#define NO_MASTER_BOOT_RECORD   0x01C10404
#define FULL_MASTER_BOOT_RECORD 0x01C1FFF0

// Partition result code
#define REQUESTED_PARTITION_SIZE_NEGATIVE  0x01B10002
#define REQUESTED_PARTITION_SIZE_TO_BIG    0x01B1FFF0
#define REQUESTED_PARTITION_ALREADY_EXISTS 0x01B1AAAA
#define REQUESTED_PARTITION_ALREADY_WIPED  0x01B1AABB

// Filesystem result codes


#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)
#define MAX_DISK_SIZE 5368709120

static const char* code2str(int code){
    switch(code){
        case OPERATION_SUCCEED: return "\x1B[32moperation succeed";
        case UNKNOWN_ERROR: return "unknown error";
        case INDEX_OUT_OF_BOUNDS: return "Index out of bounds";
        case NOT_IMPLEMENTED: return "Not implemented";

        case FILE_NOT_FOUND: return "file not found";
        case FILE_OPEN_FAILED: return "file opening failed";

        case MAPPING_FAILED: return "mapping failed";
        case STAT_FAILED: return "stat failed";

        case REQUESTED_DISK_SIZE_NEGATIVE: return "requested disk size was negative";
        case REQUESTED_DISK_SIZE_TO_BIG: return "requested disk size was to big";
        case DISK_UNMOUNTED: return "disk is not mounted";
        case DISK_MOUNTED: return "disk is mounted";

        case REQUESTED_PARTITION_SIZE_NEGATIVE: return "requested partition size is negative";
        case REQUESTED_PARTITION_SIZE_TO_BIG: return "requested partition size is to big";
        case REQUESTED_PARTITION_ALREADY_EXISTS: return "requested partition already exists";
        case REQUESTED_PARTITION_ALREADY_WIPED: return "requested partition already wiped";

        default: return "Unknown code";
    }
}

/**
 * @brief wait This method waits for input
 * @param in Stream in which should be waited
 */
static void wait(std::istream& in, const char* msg = "Press [RETURN] to continue ..."){
    in.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    in.clear();
    std::cout << msg;
    std::cin.get();
}

namespace utils{

    const static char* COLOR_RESET     = "\x1B[0m";
    const static char* COLOR_RED       = "\x1B[31m";
    const static char* COLOR_GREEN     = "\x1B[32m";
    const static char* COLOR_YELLOW    = "\x1B[33m";
    const static char* COLOR_BLUE      = "\x1B[34m";
    const static char* COLOR_MAGENTA   = "\x1B[35m";
    const static char* COLOR_CYAN      = "\x1B[36m";
    const static char* COLOR_WHITE     = "\x1B[37m";
    const static char* COLOR_BOLD      = "\x1B[1m";
    const static char* COLOR_UNDERLINE = "\x1B[4m";

    const static char* TERMINAL_CLEAR = "\e[1;1H\e[2J";

    const static char* ICON_ACCEPT_RAW = "✓";// "\xE2\x9C\x93";
    const static char* ICON_DENIED_RAW = "✗";
    const static char* ICON_ACCEPT = "\x1B[32m\xE2\x9C\x93\x1B[0m";
    const static char* ICON_DENIED = "\x1B[31mx\x1B[0m";

    static bool file_extension_equals(const std::string& file, const std::string& ext){
        std::string::size_type idx;
        idx = file.rfind('.');
        if(idx != std::string::npos){
            std::string extension = file.substr(idx+1);     
            for(int i = 0; ext[i] != 0; ++i){
                if(extension[i] != ext[i]){
                    return false;
                }
            }   
        } else {        
            return false;
        }
        return true;
    }

    static bool file_exists(const std::string& name) {
        struct stat buffer;   
        return (stat (name.c_str(), &buffer) == 0); 
    }

    static void printError(std::string message, std::string module_name, uint64_t error_code = 0){
        std::cerr << COLOR_RED << "[" << module_name << "]" << message << std::endl;
        if(error_code != 0){
            std::cerr << "Error code: 0x" << std::hex << error_code << std::dec;
        }
        std::cout << COLOR_RESET << std::endl;
    }

    static void printError(std::string message, int error_code = 0){
        std::cerr << COLOR_RED << "[Unknown]" << message << std::endl;
        if(error_code != 0){
            std::cerr << "Error code: " << std::hex << error_code << std::dec;
        }
        std::cout << COLOR_RESET << std::endl;
    }
    
    static void printWarning(std::string message, std::string module_name, int warn_code = 0){
        std::cerr << COLOR_YELLOW << "[" << module_name << "]" << message << std::endl;
        if(warn_code != 0){
            std::cerr << "Warning code: 0x" << std::hex << warn_code << std::dec;
        }
        std::cout << COLOR_RESET << std::endl;
    }

    static void printWarning(std::string message, int warn_code = 0){
        std::cerr << COLOR_YELLOW << "[Unknown]" << message << std::endl;
        if(warn_code != 0){
            std::cerr << "Warn code: 0x" << std::hex << warn_code << std::dec;
        }
        std::cout << COLOR_RESET << std::endl;
    }
    
    static void printInfo(std::string message, std::string module_name, int info_code = 0){
        std::cerr << COLOR_WHITE << "[" << module_name << "]" << message << std::endl;
        if(info_code != 0){
            std::cerr << "Info code: 0x" << std::hex << info_code << std::dec;
        }
        std::cout << COLOR_RESET << std::endl;
    }

    static void printInfo(std::string message, int info_code = 0){
        std::cerr << COLOR_WHITE << "[Unknown]" << message << std::endl;
        if(info_code != 0){
            std::cerr << "Info code: 0x" << std::hex << info_code << std::dec;
        }
        std::cout << COLOR_RESET << std::endl;
    }
        
    


    static void printHeader(const char* name){
        
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        std::cout << TERMINAL_CLEAR;
        std::cout << '+';
        for(int i = 1; i < (size.ws_col - 1); ++i){
            std::cout << '-';
        }
        std::cout << '+' << std::endl;
        int textWidth = strlen(name);
        std::cout << '|';
        int spaces = ((size.ws_col - textWidth) * .5);
        for(int i = 1; i < spaces; ++i){
            std::cout << ' ';
        }
        std::cout << name;
        for(int i = ((size.ws_col + textWidth) * .5); i < (size.ws_col - 1); ++i){
            std::cout << ' ';
        }
        std::cout << '|' << std::endl;
        std::cout << '+';
        for(int i = 1; i < (size.ws_col - 1); ++i){
            std::cout << '-';
        }
        std::cout << '+' << std::endl;
    }

    static void printModule(const char* header, const char* module, const char* description = nullptr){
        printHeader(header);
        std::cout << std::endl << "Module: " << module << std::endl;
        if(nullptr != description){
            std::cout << description << std::endl;
        }
        std::cout << std::endl;
    }
}

#endif // UTILS_H

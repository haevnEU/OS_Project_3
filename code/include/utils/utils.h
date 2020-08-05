#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/ioctl.h>
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

    const static char* ICON_ACCEPT_RAW = "\xE2\x9C\x93";
    const static char* ICON_DENIED_RAW = "x";
    const static char* ICON_ACCEPT = "\x1B[32m\xE2\x9C\x93\x1B[0m";
    const static char* ICON_DENIED = "\x1B[31mx\x1B[0m";


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
        std::cout << std::endl << "Module: " << module << std::endl << std::endl;
        if(nullptr != description){
            std::cout << description << std::endl<< std::endl;
        }
    }
}

#endif // UTILS_H

#pragma once

#include <ostream>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>

#include <iomanip>
#include <limits>
#include <cstdint>

extern "C"{
    #include <termios.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <sys/stat.h>
    #include <unistd.h>
}

#include "file.h"
#include "logger.h"
#include "progressbar.h"
#include "terminal_colors.h"
//#include "diskutils.hpp"

static bool DEBUG = false;

/**
 * @brief Current version
 */
#define CURRENT_VIRTUAL_DISK_FILE_VERSION 0x02

/**
 * @brief Minimum required version
 */
#define MINIMUM_VIRTUAL_DISK_FILE_VERSION 0x02

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)
#define MAX_DISK_SIZE 5368709120

/**
 * @brief wait This method waits for input
 * @param in Stream in which should be waited
 */
static void wait(std::istream& in, const char* message = "Press [RETURN] to continue ..."){
    in.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    in.clear();
    std::cout << message;
    std::cin.get();
}

namespace utils::do_not_use{
    static struct termios old, current;

    /* Initialize new terminal i/o settings */
    /**
     * @brief Initializes a new terminal settings
     * @param echo Enables/Disabled echo mode
     */
    static void initTermios(int echo) {
        tcgetattr(0, &old); /* grab old terminal i/o settings */
        current = old; /* make new settings same as old settings */
        current.c_lflag &= ~ICANON; /* disable buffered i/o */
        if (echo) {
            current.c_lflag |= ECHO; /* set echo mode */
        } else {
            current.c_lflag &= ~ECHO; /* set no echo mode */
        }
        tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
    }

    /**
     * @brief Resets the old terminal
     */
    static void resetTermios(void) {
        tcsetattr(0, TCSANOW, &old);
    }

    /**
     * @brief Read 1 char from terminal
     * @param echo Enables/Disables echo mode
     * @return char Character which was read
     */
    static char getch_(int echo) {
        char ch;
        initTermios(echo);
        fflush(stdin);
        ch = getchar();
        resetTermios();
        return ch;
    }
}

/**
 * @brief Reads one character from the terminal
 * @return char Character which was read
 */
static char getch(void) {
    return utils::do_not_use::getch_(0);
}

namespace utils{

    /**
     * @brief This methods gets the current date and time
     * @return const char* 
     */
    static const char* dateTime(){
        auto end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char* time = std::ctime(&end_time);
        if (time[strlen(time)-1] == '\n'){
            time[strlen(time)-1] = '\0'; 
        }
        return time;
    }

/*
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

    static void createEmptyVirtualDiskFile(const char* path){
        a
    }

    */

}

namespace utils::menu{

    /**
     * @brief Prints a menu entry to the terminal
     * @param message Message which should be printed
     * @param row Row index
     * @param current_row Index of current row
     */
    static void inline printEntry(const char* message, int row, int current_row){
        const char* color = utils::colors::BACK_GREEN;
        if(row == current_row){
            std::cout << color<< utils::colors::BLACK << ">"; 
        }else{
            std::cout << utils::colors::RESET;     
        }
        std::cout << message; 
        if(row == current_row){
            std::cout << color << utils::colors::BLACK << "<"; 
        }
        std::cout << utils::colors::RESET << std::endl;     
        
    }

    /**
     * @brief Prints am menu to the terminal
     * @details This method prints a menu to the terminal. it Will also return the selected item.
     * @param entries Menu entries
     * @param size Size of the entries array
     * @param menu_selected_index Previous selected index
     * @param overflow Enables the overflow of menu selection
     * @return int Selected menu entry
     */
    static inline int printMenu(const char* menu_header, const char* entries[], int size, int menu_selected_index, bool menu_selection_overflow = true){
        char c = 'D';
        int row = menu_selected_index;
        while(true){
            std::system("clear");
            std::cout << utils::colors::CLEAR << menu_header << " " << utils::dateTime() << std::endl;

            std::cout << "Use W/S to navigate and <ENTER> to select" << std::endl<< std::endl;
            for(int i = 0; i < size; i++){
                printEntry(entries[i], i, row);
            }
            c = getch();
            if(c == 'W' || c == 'w'){
                row--;
                if(row < 0){
                    row = ((menu_selection_overflow) ? size - 1 : 0);
                }
            }
            if(c == 'S' || c == 's'){
                row++;
                if(row >= (size)){
                    row = ((menu_selection_overflow) ? 0 : size - 1);
                }
            }
            if(c == 10){
                return row;
            }
        }
        return -1;
    }
    

    /**
     * @brief Prints am menu to the terminal
     * @details This method prints a menu to the terminal. it Will also return the selected item.
     * @param entries Menu entries
     * @param size Size of the entries array
     * @param overflow Enables the overflow of menu selection
     * @return int Selected menu entry
     */
    static inline int printMenu(const char* menu_header, const char* entries[], int size, bool menu_selection_overflow = true){
        return printMenu(menu_header, entries, size, -1, menu_selection_overflow);
    }
    
}

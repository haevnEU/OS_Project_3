#pragma once

#include <ostream>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>
#include <iomanip>

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


#define INODE 0x82
#define FAT 0x0C
#define FAT_HIDDEN 0x1C

static const char* fs_number_to_string(uint8_t number){
    switch(number){
        case 0x82: return "INODE";
        case 0x0C: return "FAT";
        case 0x1C: return "FAT HIDDEN";
    }
    return "UNKNOWN";
}

/**
 * @brief wait This method waits for input
 * @param in Stream in which should be waited
 */
static void wait(std::istream& in, const char* message = "Press [RETURN] to continue ..."){
    std::cout << message;
    in.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    in.clear();
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
        ch = getchar();
        resetTermios();
        return ch;
    }
};




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
    static inline const char* dateTime(){
        auto end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char* time = std::ctime(&end_time);
        if (time[strlen(time)-1] == '\n'){
            time[strlen(time)-1] = '\0'; 
        }
        return time;
    }

    namespace verify_results{
        /**
         * @brief States that the operation was ok
         */
        static const int operation_ok = 0;
        
        /**
         * @brief States that the operation was canceled
         */
        static const int operation_aborted = 1;
        
        /**
         * @brief States that the path size is invalid
         */
        static const int input_to_small = -1;
        
        /**
         * @brief States that the path is invalid
         */
        static const int input_path_invalid = -2;
    }

    /**
     * @brief This method verifies a path
     * @details This method first checks if the size is bigger than 0
     *          Next its checked if the first letter is a quit(q/Q) letter
     *          Finally the first letter is checked if its a valid path starter.
     * @param path Path which should be validated
     * @return int Result code
     */
    static int inline verifyInput(const std::string path){
        if(path.size() <= 0){
            std::cout << utils::colors::RED << "Given input was invalid." << utils::colors::RESET << std::endl;
            return verify_results::input_to_small;
        }else if(path[0] ==  'q' || path[0] == 'Q'){
            std::cout << utils::colors::YELLOW << "Operation aborted" << utils::colors::RESET << std::endl;
            return verify_results::operation_aborted;
        }

        return verify_results::operation_ok;
    }
}



namespace utils::menu{

    /**
     * @brief This struct contains all information for a menu 
     */
    struct menu_settings{
        /**
         * @brief This enables the over/underflow of line selection
         */
        bool row_selection_overflow = true;

        /**
         * @brief This clears the the input cache
         */
        bool clear_cache = false;

        /**
         * @brief This is the previous selected row
         */
        int preselected_row = 0;

        /**
         * @brief This is an optional error message
         */
        std::string sub_header;

        /**
         * @brief This is the background color
         */
        const char* background = utils::colors::BACK_GREEN;

        /**
         * @brief This is the foreground color
         */
        const char* foreground = utils::colors::BLACK;

        /**
         * @brief This is the line selection indicator
         */
        const char* line_selector[2] = {" >", "< "};

        /**
         * @brief The up selection key
         */
        char up_key = 'w';
        
        /**
         * @brief The down selection key
         */
        char down_key = 's';
    };


    /**
     * @brief Prints a menu entry to the terminal
     * @param message Message which should be printed
     * @param row Row index
     * @param current_row Index of current row
     */
    static void inline printEntry(const char* message, int row, int current_row, menu_settings& settings){
        const char* color = settings.background;
        if(row == current_row){
            std::cout << color << settings.foreground << settings.line_selector[0]; 
        }else{
            std::cout << ' ' << ::utils::colors::RESET;     
        }
        std::cout << message; 
        if(row == current_row){
            std::cout << color << settings.foreground << settings.line_selector[1]; 
        }else{
            std::cout << ' ';
        }
        std::cout << ::utils::colors::RESET << std::endl;     
    }

    /**
     * @brief This methods prints a menu on the terminal
     * @details This method prints every menu entry on the terminal and allows the selection
     *          with specified keys. The menu is customizable via a settings argument.
     *          The result value correspond to the provided entries, e.g. result 0 <=> entries[0]
     *          A known bug is that the input stream is filled after some operation, therefore
     *          setting the settings entry clear_cache is recommended
     * @param entries Menu entries which should be printed
     * @param amount_entries Amount of the entries
     * @param menu_header Header of the menu
     * @param settings Settings object
     * @return int Selected 0 based index
    */
    static inline int print(const char* entries[], int amount_entries, const char* menu_header, menu_settings& settings){
        char c = 'D';
        int row = settings.preselected_row;
        if(settings.clear_cache){
            char c2;
            while ((c2 = getchar()) != '\n' && c2 != EOF) { }
        }

        while(true){
            std::system("clear");
            std::cout << ::utils::colors::CLEAR << menu_header << " " << ::utils::dateTime() << std::endl;

            std::cout << "Use W/S to navigate and <ENTER> to select" << std::endl;
            if(settings.sub_header.size() > 0){
                std::cout << settings.sub_header << std::endl;
            }

            std::cout << std::endl;
            for(int i = 0; i < amount_entries; i++){
                printEntry(entries[i], i, row, settings);
            }
            c = getch();

            if(c == 'W' || c == settings.up_key){
                row--;
                if(row < 0){
                    row = ((settings.row_selection_overflow) ? amount_entries - 1 : 0);
                }
            }
            if(c == 'S' || c == settings.down_key){
                row++;
                if(row >= (amount_entries)){
                    row = ((settings.row_selection_overflow) ? 0 : amount_entries - 1);
                }
            }
            if(c == 10){
                break;
            }
        }

        return row;
    }


    /**
     * @brief This methods prints a menu on the terminal
     * @details This method prints every menu entry on the terminal and allows the selection
     *          with specified keys. The menu is customizable via a settings argument.
     *          The result value correspond to the provided entries, e.g. result 0 <=> entries[0]
     *          A known bug is that the input stream is filled after some operation, therefore
     *          setting the settings entry clear_cache is recommended
     * @param entries Menu entries which should be printed
     * @param amount_entries Amount of the entries
     * @param menu_header Header of the menu
     * @param settings Settings object
     * @return int Selected 0 based index
    */
    static inline int print(char* entries[], int amount_entries, const char* menu_header, menu_settings& settings){
        char c = 'D';
        int row = settings.preselected_row;
        if(settings.clear_cache){
            char c2;
            while ((c2 = getchar()) != '\n' && c2 != EOF) { }
        }

        while(true){
            std::system("clear");
            std::cout << ::utils::colors::CLEAR << menu_header << " " << ::utils::dateTime() << std::endl;

            std::cout << "Use W/S to navigate and <ENTER> to select" << std::endl;
            if(settings.sub_header.size() > 0){
                std::cout << settings.sub_header << std::endl;
            }

            std::cout << std::endl;
            for(int i = 0; i < amount_entries; i++){
                printEntry(entries[i], i, row, settings);
            }
            c = getch();

            if(c == 'W' || c == settings.up_key){
                row--;
                if(row < 0){
                    row = ((settings.row_selection_overflow) ? amount_entries - 1 : 0);
                }
            }
            if(c == 'S' || c == settings.down_key){
                row++;
                if(row >= (amount_entries)){
                    row = ((settings.row_selection_overflow) ? 0 : amount_entries - 1);
                }
            }
            if(c == 10){
                break;
            }
        }

        return row;
    }

    /**
     * @brief This is deprectaed and must be replaced with print
     * @deprecated
     */
    static inline int printMenu(const char* menu_header, const char* entries[], int size, int menu_selected_index, bool clear_input = true){
        menu_settings setting;
        setting.preselected_row = menu_selected_index;
        return print(entries, size, menu_header, setting);
    }
}
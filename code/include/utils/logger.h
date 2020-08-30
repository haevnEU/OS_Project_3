#ifndef UTILS__LOGGING__LOGGER_H
#define UTILS__LOGGING__LOGGER_H

#include <fstream>
#include <ostream>
#include <cstdarg>
#include <string>
#include <iostream>
#include <chrono>

extern "C"{
    #include <time.h>
}

#include "utils.h"
#include "terminal_colors.h"

// use this for colore output to terminal
#define msg utils::logging::Logger::message()
#define msgln utils::logging::Logger::message_ln()
#define debug utils::logging::Logger::debug_f()
#define info utils::logging::Logger::info_f()
#define warn utils::logging::Logger::warn_f()
#define error utils::logging::Logger::error_f()

namespace utils::logging{

    enum log_level{
        none_t = 0, debug_t = 1, info_t = 2, warn_t = 3, error_t = 4, message_t = 5, messageln_t = 6
    };

    class Logger{
    
    public:

    static Logger& message(){
        static Logger instance;
        instance.setLevel(log_level::message_t);
        return instance;
    }

    static Logger& message_ln(){
        static Logger instance;
        instance.setLevel(log_level::messageln_t);
        return instance;
    }

    static Logger& debug_f(){
        static Logger instance;
        instance.setLevel(log_level::debug_t);
        return instance;
    }

    static Logger& info_f(){
        static Logger instance;
        instance.setLevel(log_level::info_t);
        return instance;
    }

    static Logger& warn_f(){
        static Logger instance;
        instance.setLevel(log_level::warn_t);
        return instance;
    }

    static Logger& error_f(){
        static Logger instance;
        instance.setLevel(log_level::error_t);
        return instance;
    }


    private:
        Logger(){}
        
        void setLevel(log_level level){
            this->level = level;
        }
        log_level level = log_level::none_t;
        std::ostream& stream = std::cout;

       
    public:

    Logger operator<<(const std::string& message){
        auto end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char* time = std::ctime(&end_time);
        if (time[strlen(time)-1] == '\n'){
            time[strlen(time)-1] = '\0'; 
        }
        
        if(level == log_level::none_t){
            return *this;
        }else if(level == log_level::debug_t){
            stream << utils::colors::MAGENTA << "[DEBUG][" << time <<"] " << message << utils::colors::RESET << std::endl;
        }else if(level == log_level::info_t){
           stream << utils::colors::CYAN <<    "[INFO][" << time <<"] " << message << utils::colors::RESET << std::endl;
        }else if(level == log_level::warn_t){
           stream << utils::colors::YELLOW <<  "[WARN][" << time <<"] " << message << utils::colors::RESET << std::endl;
        }else if(level == log_level::error_t){
           stream << utils::colors::RED <<     "[ERROR][" << time <<"] " << message << utils::colors::RESET << std::endl;
        }else if(level == log_level::message_t){
           stream << utils::colors::WHITE << message << utils::colors::RESET;
        }else if(level == log_level::messageln_t){
            stream << utils::colors::WHITE << message << utils::colors::RESET << std::endl;
        }
        return *this;
    }

    };
}



#endif // UTILS__LOGGING__LOGGER_H

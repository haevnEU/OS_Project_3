#ifndef UTILS__LOGGING__LOGGER_H
#define UTILS__LOGGING__LOGGER_H

#include <fstream>
#include <ostream>
#include <cstdarg>
#include <string>
#include <iostream>
#include <chrono>

// c include
extern "C"
{
    #include <time.h>
}

// project includes
#include "utils.h"
#include "terminal_colors.h"

// use this for terminal color output terminal
#define msg utils::logging::Logger::message()
#define msgln utils::logging::Logger::message_ln()
#define debug utils::logging::Logger::debug_f()
#define info utils::logging::Logger::info_f()
#define warn utils::logging::Logger::warn_f()
#define error utils::logging::Logger::error_f()

/*!
 *  \addtogroup utils
 *  @{
 */

/**
 * @brief Logging utilities
 */
namespace utils::logging
{
    /**
     * @brief Various Log Levels
     * @version 1.0.0.0
     */
    enum log_level
    {
        none_t = 0,         ///< No Log Level
        debug_t = 1,        ///< Debug
        info_t = 2,         ///< Information
        warn_t = 3,         ///< Warning
        error_t = 4,        ///< Error
        message_t = 5,      ///< Message
        messageln_t = 6     ///< Message
    };


    class Logger
    {
    
    public: /* methods *************************************/

        /**
         * @brief Logger message
         */
        static Logger& message()
        {
            static Logger instance;
            instance.setLevel(log_level::message_t);
            return instance;
        }

        /**
         * @brief Logger message
         */
        static Logger& message_ln()
        {
            static Logger instance;
            instance.setLevel(log_level::messageln_t);
            return instance;
        }

        /**
         * @brief Logger debug
         */
        static Logger& debug_f()
        {
            static Logger instance;
            instance.setLevel(log_level::debug_t);
            return instance;
        }

        /**
         * @brief Logger information
         */
        static Logger& info_f()
        {
            static Logger instance;
            instance.setLevel(log_level::info_t);
            return instance;
        }

        /**
         * @brief Logger warning
         */
        static Logger& warn_f()
        {
            static Logger instance;
            instance.setLevel(log_level::warn_t);
            return instance;
        }

        /**
         * @brief Logger error
         */
        static Logger& error_f()
        {
            static Logger instance;
            instance.setLevel(log_level::error_t);
            return instance;
        }


    private: /* methods & attributes ************************/
        
        /**
         * @brief constructor 
         */
        Logger(){}
        
        /**
         * @brief Log level setter 
         */
        void setLevel(log_level level)
        {
            this->level = level;
        }
        
        log_level level = log_level::none_t;
        std::ostream& stream = std::cout;

       
    public: /* operator ************************************/

    /**
     * @brief Logger operator 
     */
    Logger operator<<(const std::string& message)
    {
        ///- Sets current time
        auto end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char* time = std::ctime(&end_time);

        if (time[strlen(time)-1] == '\n')
        {
            time[strlen(time)-1] = '\0'; 
        }
        
        ///- Print Log based on loglevel
        if(level == log_level::none_t)
        {
            return *this;
        }
        else if(level == log_level::debug_t)
        {
            stream << utils::colors::MAGENTA << "[DEBUG][" << time <<"] " << message << utils::colors::RESET << std::endl;
        }
        else if(level == log_level::info_t)
        {
           stream << utils::colors::CYAN <<    "[INFO][" << time <<"] " << message << utils::colors::RESET << std::endl;
        }
        else if(level == log_level::warn_t)
        {
           stream << utils::colors::YELLOW <<  "[WARN][" << time <<"] " << message << utils::colors::RESET << std::endl;
        }
        else if(level == log_level::error_t)
        {
           stream << utils::colors::RED <<     "[ERROR][" << time <<"] " << message << utils::colors::RESET << std::endl;
        }
        else if(level == log_level::message_t)
        {
           stream << utils::colors::WHITE << message << utils::colors::RESET;
        }
        else if(level == log_level::messageln_t)
        {
            stream << utils::colors::WHITE << message << utils::colors::RESET << std::endl;
        }
        return *this;
    }

    };
}



#endif // UTILS__LOGGING__LOGGER_H

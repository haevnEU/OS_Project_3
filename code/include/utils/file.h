/**
 * @file file.h
 * @brief This file is utility to handle file related operation
 * @details Extension check and existence check are implemented here
 * @version 0.1
 * @date 2020-08-26
 * 
 */
#pragma once

#include <string>
#include <iomanip>
#include <cstring>

// c include
extern "C"
{
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <sys/stat.h>
    #include <unistd.h>
}

/*!
 *  \addtogroup utils
 *  @{
 */

/**
 * @brief File Utilities
 */
namespace utils::file
{

    /**
     * @brief Checks file extension
     * @param file File which should be evaluated
     * @param ext Extension which the file should equal
     * @return True if the file extension equals ext 
     */
    static inline bool extension_equals(const std::string& file, const std::string& ext)
    {
        std::string::size_type idx;
        idx = file.rfind('.');

        if(idx != std::string::npos)
        {
            std::string extension = file.substr(idx+1);    

            for(int i = 0; ext[i] != 0; ++i)
            {
                if(extension[i] != ext[i])
                {
                    return false;
                }
            }
        }
        else
        {        
            return false;
        }

        return true;
    }

    /**
     * @brief Checks if a file exists
     * @param name Filename
     * @return True if the file exists 
     */
    static inline bool exists(const std::string& name)
    {
        struct stat buffer;   
        return (stat (name.c_str(), &buffer) == 0); 
    } 
}

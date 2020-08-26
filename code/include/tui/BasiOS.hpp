#pragma once

// project includes
#include "../utils/utils.h"

namespace core::os{
    /**
     * @brief This class is something like an OS.
     * @details This class is the the virtual operating system which only handles file IO with loaded partition.
     *          It utilizes the core::utility namespace to operate with a virtual disk and implements the basic
     *          File IO with loaded disk. It also handles the disk management and defragmentation.
     */
    class BasicOS{
    private:

    public:
        /**
         * @brief Construct a new Basic O S object
         */
        BasicOS();
        
        /**
         * @brief Destroy the Basic O S object
         */
        ~BasicOS();

        /**
         * @brief Enters the tui
         * @details This method invokes the TUI of this class, it will work via the terminal interface.
         *          The private wrapper methods are called for the api usage.
         * @details Return code of the OS, if 100 then restart
         */
        int enter();
        
    };
}
#pragma once

// project includes
#include "../utils/utils.h"

#include "../core/DiskUtils.hpp"

/*!
 *  \addtogroup core
 *  @{
 */

/**
 * @brief OS Simulation elements
 */
namespace core::os
{
    /**
     * @brief Basic OS Simulation.
     * @details This class is the the virtual operating system which only handles file IO with loaded partition.
     *          It utilizes the core::utility namespace to operate with a virtual disk and implements the basic
     *          File IO with loaded disk. It also handles the disk management and defragmentation.
     */
    class BasicOS
    {
    private:

        int partition_index = -1;

        /**
         * @brief Workspace routine
         */
        void workspace();

        /**
         * @brief File create routine
         */
        void createFile();

        /**
         * @brief File delete routine
         */
        void deleteFile();

        /**
         * @brief This method prints all disk which the bios loaded
         */
        void inline print_loaded_partitions(){
            uint64_t counter = 0;
            core::utilities::DiskUtils& handler = core::utilities::DiskUtils::getInstance();
            std::cout << utils::colors::CLEAR << "\033[4m";
            for(auto element = handler.loadedPartitions()->begin(); element < handler.loadedPartitions()->end(); element++){
                std::cout << counter << ")" << ((core::Partition*)*element)->part_definition()->path << " " << (((core::Partition*)*element)->isMounted() ? "Mounted" : "Not mounted") << std::endl;
                counter++;
            }
        }

        /**
         * @brief This methods allow the selecion of a disk
         * @return int Disk index
         */
        int inline select_loaded_partition(){
            core::utilities::DiskUtils& handler = core::utilities::DiskUtils::getInstance();
            std::cout << utils::colors::CLEAR << "SELECT A DISK TO CONTINUE\n\033[4m";
            char* entries[handler.loadedPartitions()->size()];
            
            for(int i = 0; i < handler.loadedPartitions()->size(); i++){
                std::string str = handler.loadedPartitions()->at(i)->part_definition()->path;
                str.append(" ").append(handler.loadedPartitions()->at(i)->isMounted() ? "Mounted" : "Not mounted");
                std::cout << i << ")" << str << std::endl;
            }
            //utils::menu::menu_settings settings;
            //settings.row_selection_overflow = true;
            //int choice = utils::menu::print(entries, handler.loadedPartitions()->size(), "Select a disk to operate on", settings);
            int choice;
            std::cout << utils::colors::RESET << "Choice: ";
            std::cin >> choice;
            return choice;
        }

    public:
        /**
         * @brief Constructor
         * @details Constructs a new Basic O S object
         */
        BasicOS();
        
        /**
         * @brief Destructor
         * @details Destroys the Basic O S object
         */
        ~BasicOS();

        /**
         * @brief Enters the tui
         * @details This method invokes the TUI of this class, it will work via the terminal interface.
         *          The private wrapper methods are called for the api usage.
         * @details Return code of the OS, if 100 then restart
         */
        int enter();
        
        static void error_(uint8_t code){
            std::cout << "Error: " << (int)code << std::endl;
        }
    };
}
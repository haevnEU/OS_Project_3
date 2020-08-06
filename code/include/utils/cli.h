#ifndef CLI_H
#define CLI_H

#include <vector>

#include "utils.h"
#include "diskutil.h"
#include "mbrutils.h"
#include "../diskhandler.h"

namespace utils{

    /**
     * @brief The CLI class
     * @details This class provides a CLI for this application
     */
    class CLI{
        /// Diksutil TUI
        diskUtil* du;
        MBRUtils* mbru;

        /// Contains all loaded disks
        //std::vector<Disk*>* disks;
        DiskHandler& handler = DiskHandler::getInstance();

        // Remove?
        void addDisk(Disk* disk);

        /// This is the current disk, which the CLI uses
        Disk* currentDisk;


    public methods:
        /**
         * @brief CLI
         * @details Creates a new object of this class
         */
        CLI();
        ~CLI();

        /**
         * @brief printMenu
         * @details This methods prints the menu
         */
        void printMenu();

        /**
         * @brief printSelectedDisk
         * @details This methods shows all loaded disks and allows the selection which is used as
         *          currentDisk
         */
        void printSelectedDisk();

        /**
         * @brief createDisk
         * @details This module invokes a wizard to create a new virtual disk file
         */
        void createDisk();

        /**
         * @brief loadDisk
         * @details This module invokes a wizard to load a virtual disk file
         */
        void loadDisk();
     
        /**
         * @brief listDisks
         * @details This module prints all loaded virtual disk files
         */
        void listDisks();
     
        /**
         * @brief selectDisk
         * @details This module priits all loaded virtual disk files
         *          and let the user choose which is used as the currentDisk
         */
        void selectDisk();

        /**
         * @brief mountDisk
         * @details This module mounts the currentDisk as usable
         */
        void mountDisk();

        /**
         * @brief unmountDisk
         * @details This module unmounts the currentDisk as unusable
         */
        void unmountDisk();

        /**
         * @brief startDiskUtils
         * @details This module invokes the DiskUtils submodule.
         */
        void startDiskUtils();

        /**
         * @brief scriptMode
         * @details This is the scripting mode, it allows the execution of LUA scripts
         *          and provides a REPL interface
         */
        void scriptMode();

        /**
         * @brief enterInteractiveMode
         * @details "Mainloop" for the module
         */
        void enterInteractiveMode();
    };

}
#endif // CLI_H

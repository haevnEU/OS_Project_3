#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <csignal>

#include "disk.h"
#include "utils/cli.h"
#include "api.h"

#include "utils/utils.h"


void signalHandler(int signum){
    std::cerr << utils::COLOR_RED << "[MAIN] An Interrupt was caught." << std::endl
              << "If you open a new issue ticket please provide the following information" << std::endl
              << "Replace <ACTION> with your previous actions" << std::endl<< std::endl
              << "Issue: " << strsignal(signum) << "[" << signum <<"]" << std::endl
              << "Previous action: <ACTION>" << std::endl
              << std::endl 
              << "Application state: " << std::endl
              << "Diskhandler: " << std::endl;
    DiskHandler& handler = DiskHandler::getInstance();
    int amountDisks = handler.amountDisks();
    Partition* partition;
    MasterBootRecord* mbr;
    Disk* disk;

    std::cout << "Loaded disks: " << amountDisks << std::endl;
    for(int i = 0; i < amountDisks; i++){
        std::cout << "Disk No: " << i << "/" << amountDisks << std::endl;
        disk = handler.getDisk(i);
        if(nullptr == disk){
            std::cout << "Disk is nullptr" << std::endl;
            continue;
        }
        
        std::cout << disk->available_size() << "/" << disk->size() << " bytes used" << std::endl;
        std::cout << "State: " << (disk->isMounted() ? "Mounted" : "Unmounted") << std::endl;
        std::cout << "Location: " << disk->path() << std::endl;
        // Disk infos 

        mbr = disk->MBR();
        if(nullptr == mbr){
            std::cout << "Master Boot Record is nullptr" << std::endl;
            continue;
        }else{
            for(int i = 0; i < 4; i++){
                partition = mbr->partition(i);
                std::cout << "+------------------------------" << std::endl;
                if(nullptr == partition){
                    std::cout << "| Partition No: " << i << ": " << std::endl
                            << "| \tNULL" << std::endl
                            << "+------------------------------" << std::endl;
                }else{
                    if(partition->startAddress() == 0x00 && partition->size() == 0){
                        std::cout << "| Partition No: " << i << ": " << std::endl
                                << "| \t" << "NULL" << std::endl
                                << "+------------------------------" << std::endl;
                    }else{
                        std::cout << "| Partition No: " << i << std::endl
                                << "| \t" << "Startaddress: 0x" << std::hex << std::setw(8) << std::setfill('0') << partition->startAddress() << std::endl
                                << "| \t" << "Endaddress:   0x" << std::hex << std::setw(8) << std::setfill('0') << (partition->size() + partition->startAddress()) << std::endl
                                << "| \t" << "Size:         "   << std::dec << partition->size() << std::endl
                                << "| \t" << "Primary:      "   << (partition->isPrimary() ? utils::ICON_ACCEPT_RAW : utils::ICON_DENIED_RAW) << std::endl
                                << "| \t" << "Filesystem:   "   << getFileSystemName(partition->fileSystemType()) << std::endl
                                << "+------------------------------" << std::endl;
                
                    }
                }
            }
        }
        
    }
    std::cout << utils::COLOR_RESET;
    exit(signum);
}

void quit(int signum){
    std::cout << utils::COLOR_RESET;
    exit(signum);

    if(signum == SIGINT){
        char choice;
        std::cout << utils::TERMINAL_CLEAR << "[MAIN] Are you sure to quit (y/N): ";
        std::cin >> choice;
        if(choice == 'Y' || choice == 'y'){
            exit(signum);
        }
        
    }
}
int main(int argc, char *argv[]){

    signal(SIGINT, quit);
    signal(SIGABRT, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGSEGV, signalHandler);
  
    std::cout << "\e[1;1H\e[2J";

    utils::CLI cli;
    cli.enterInteractiveMode();

    API::getInstance().close();

    return 0;
}

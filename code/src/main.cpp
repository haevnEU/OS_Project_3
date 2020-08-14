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
              << "If you create a new issue ticket, please provide previous interaction and the following information" << std::endl
              << "Interrupt [" << signum << "]" << std::endl
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
        std::cout << "State: " << (disk->isMounted() ? "Mounted" : "Unmounted");
        std::cout << "Location: " << disk->path();
        // Disk infos 

        mbr = disk->MBR();
        if(nullptr == mbr){
            std::cout << "Master Boot Record is nullptr" << std::endl;
            continue;
        }
        
    }
    exit(signum);
}

void quit(int signum){
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

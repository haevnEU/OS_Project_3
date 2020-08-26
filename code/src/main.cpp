#include <iostream>
#include <csignal>

#include "../include/utils/utils.h"

#include "../include/core/DiskUtils.hpp"
#include "../include/core/MasterBootRecordUtils.hpp"
#include "../include/core/PartitionUtils.hpp"
#include "../include/tui/BasiOS.hpp"


void signalHandler(int signum){
    std::cout << utils::colors::RED << "An important signal was caught: " << strsignal(signum) << ": " << signum << utils::colors::RESET << std::endl;
    std::cout << utils::colors::RED << "Please open a new issue ticket on github with your previous interaction, if possible including detailed description and screenshot" << utils::colors::RESET << std::endl;
    exit(signum);
}


#include <stdio.h>



int main(int argc, const char* argv[]) {
    std::cout << utils::colors::CLEAR;
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-d") == 0){
            DEBUG = true;
            std::cout << utils::colors::MAGENTA << "DEBUG ENABLED" << utils::colors::RESET << std::endl;
        }
    }   
    
    signal(SIGABRT, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGSEGV, signalHandler);
    const char* entries[] = {"Disk", "Master Boot Record", "Partition", "Shutdown", "Start OS"};
    core::utilities::DiskUtils& du = core::utilities::DiskUtils::getInstance();
    core::utilities::MasterBootRecordUtils& mbru = core::utilities::MasterBootRecordUtils::getInstance();
    core::utilities::PartitionUtils& pu = core::utilities::PartitionUtils::getInstance();
    int choice;
    bool active = true;

    utils::menu::menu_settings settings;
    while(active){
        //choice = utils::menu::printMenu("BIOS", entries, 5, 0);
        choice = utils::menu::print(entries, 5, "BIOS", settings);
        switch(choice){
            case 0:
                du.enter();
            break;
            case 1:
                mbru.enter();
            break;
            case 2:
                pu.enter();
            break; 
            case 3: return 0;
            break;
            case 4:
                active = false;
            break;
        }
    }
    core::os::BasicOS os;
    int ret = os.enter();
    
    if(100 == ret){
        execve(argv[0], const_cast<char**>(argv), nullptr);
    }
    
    return ret;
}


/*

int main(int argc, char *argv[]){

//    signal(SIGINT, quit);
//    signal(SIGABRT, signalHandler);
//    signal(SIGFPE, signalHandler);
//    
//    signal(SIGSEGV, signalHandler);
  
    std::cout << "\e[1;1H\e[2J";
    int c;
   while (c=getch()!='\r') //enter or whatever
{
printf ("The char %c has the ASCII code %d\n", c, c);
} 
 //   tui::SetupView setup;
 //   setup.enter();

    return 0;
}*/

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>

#include "disk.h"
#include "utils/cli.h"
#include "api.h"

int main(int argc, char *argv[]){

  
    std::cout << "\e[1;1H\e[2J";

    utils::CLI cli;
    cli.enterInteractiveMode();

    API::getInstance().close();
   return 0;
}

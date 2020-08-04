#include "disk.h"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <QCoreApplication>
#include <utils/cli.h>

#include "api/api.h"

int main(int argc, char *argv[]){

    QCoreApplication a(argc, argv);

    std::cout << "\e[1;1H\e[2J";

    utils::CLI cli;
    cli.enterInteractiveMode();

    API::getInstance().close();
   return 0;
}

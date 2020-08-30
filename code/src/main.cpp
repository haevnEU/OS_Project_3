#include <iostream>
#include <csignal>

#include "../include/utils/utils.h"

#include "../include/core/DiskUtils.hpp"
#include "../include/core/MasterBootRecordUtils.hpp"
#include "../include/core/PartitionUtils.hpp"
#include "../include/tui/BasiOS.hpp"

/*! \mainpage Documentation for OS Project 3
 *
 * \section introEN_sec Introduction
 *
 * This is the introduction.
 * 
 * \subsection runDE_sec Program sequence
 * 
 * A TUI (Terminal User Interface) will be displayed after the program started. You can navigate within this TUI using [W] and [S] keys to move your
 * selection up- or downwards. To confirm your selection press [RETURN].
 * 
 * <h5>Example for a possible program sequence</h5>
 * 
 * Select ><b>DISK</b><, confirm with <b>[RETURN]</b>.
 * 
 * Select ><b>Create new virtual disk file</b><, confirm with <b>[RETURN]</b>.
 * 
 * Enter any file path. We use <b>/home/user/disk.vdf</b> in this example.
 * <small><i>Hint: Replace user with your individual user name. A disk can only be created within folder with appropriate permissions.</i></small>
 * 
 * The disk size can be chosen freely. We use <b>3221225472</b> Byte in this example.
 * 
 * Navigate back to the main menu. Select ><b>Master Boot Record</b><, confirm with <b>[RETURN]</b>.
 * 
 * Select ><b>Create Master Boot Record</b>< in the following menu, confirm with <b>[RETURN]</b>.
 * 
 * Enter the disk path. We used <b>/home/user/disk.vdf</b> in this example.
 * 
 * Navigate back to the main menu. Select ><b>Partition</b><, confirm with <b>[RETURN]</b>.
 * 
 * Im folgenden Menü ><b>Create new partition</b>< markieren, confirm with <b>[RETURN]</b>.
 * 
 * Enter the disk path. We used <b>/home/user/disk.vdf</b> in this example.
 * 
 * Select ><b>FAT</b>< in the following menu, confirm with <b>[RETURN]</b>.
 * 
 * The partition size can be chosen freely.
 *
 * \subsection docEN_sec Update the documentation
 * 
 * Um die Dokumentation zu aktualisieren wird Doxygen benötigt. 
 * <ol>
 *  <li>
 *   <h5>Doxywizard öffnen</h5>
 *   <p>Recommended version: <a href="https://www.doxygen.nl/download.html">1.8.18</a></p>
 *  </li>
 *  <li>
 *   <h5>Use Doxywizard to open Doxyfile</h5>
 *   <p>The doxyfile is placed here: [project_root]/doc</p>
 * </li>
 *  <li>
 *   <h5>Run Doxywizard</h5>
 *   <p>The wizard will update the documentation itself.</p>
 *  </li>
 * </ol>
 * 
 * ===================================================================================================================
 * 
 * \section introDE_sec Einleitung
 * 
 * Dieses Projekt simuliert ein 
 * 
 * \section runDE_sec Programmablauf
 * 
 * Nach dem Starten wird ein TUI (Terminal User Interface) dargestellt. Mithilfe der Tasten [W] und [S] kann in diesem auf-, bzw. abwärts navigiert werden. 
 * Eine getroffene Auswahl wird mit [ENTER] bestätigt.
 * Im simulierten OS können Partitionsinformationen und  eingesehen
 * 
 * <h5>Beispiel für einen möglichen Programmablauf</h5>
 * \subsection step1 1. Disk erstellen
 *  <ol>
 *      <li>Eintrag ><b>DISK</b>< markieren, mit <b>[ENTER]</b> bestätigen</li>
 *      <li>Eintrag ><b>Create new virtual disk file</b>< markieren, mit <b>[ENTER]</b> bestätigen</li>
 *      <li>
 *          Einen beliebigen Pfad eingeben. In diesem Beispiel wird <b>./disk.vdf</b> gewählt.
 *          <small><i>Anmerkung: Eine Disk kann nur in einem Verzeichnis mit entsprechender Rechtevergabe erstellt werden.</i></small>
 *      </li>
 *      <li>Eine beliebige Größe für die zu erzeugende Disk festlegen. In diesem Beispiel wird <b>500000000</b> Byte gewählt</li>
 *  </ol>
 * 
 * \subsection step2 2. Disk verifizieren
 *  <ol>
 *      <li>Eintrag ><b>Verify virtual disk file</b>< markieren, mit <b>[ENTER]</b> bestätigen</li>
 *  </ol>
 * 
 * \subsection step3 3. Master Boot Record erstellen
 *  <ol>
 *     <li>Im Hauptmenü ><b>Master Boot Record</b>< markieren, mit <b>[ENTER]</b> bestätigen</li>
 *     <li>Im folgenden Menü ><b>Create Master Boot Record</b>< markieren, mit <b>[ENTER]</b> bestätigen</li>
 *     <li>Den Pfad zur erstellten Disk angeben, in diesem Beispiel <b>./disk.vdf</b></li>
 * </ol>
 * 
 * \subsection step4 4. Partition erstellen
 *  <ol>
 *      <li>Im Hauptmenü ><b>Partition</b>< markieren, mit <b>[ENTER]</b> bestätigen</li>
 *      <li>Im folgenden Menü ><b>Create new partition</b>< markieren, mit <b>[ENTER]</b> bestätigen</li>
 *      <li>Den Pfad zur erstellten Disk angeben, in diesem Beispiel <b>/home/user/disk.vdf</b></li>
 *      <li>Im folgenden Menü ><b>FAT</b>< markieren, mit <b>[ENTER]</b> bestätigen</li>
 *      <li>Eine beliebige Größe für die zu erzeugende Partition festlegen In diesem Beispiel wird <b>250000000</b> Byte gewählt</li>
 *      <li><b>Wichtig:</b> Nach dem erstellen einer Partition unbedingt die Disk neu laden. (<b>Disk</b> => <b>Load existing virtual disk file</b>)
 * </ol>
 * 
 * \subsection step5 5. OS Starten
 * <ol>
 *      <li>Im Hauptmenü ><b>Start OS</b>< markieren, mit <b>[ENTER]</b> bestätigen</li>
 *      <li>Im folgenden Menü ><b>Workspace</b>< markieren, mit <b>[ENTER]</b> bestätigen</li>
 *      <li>
 *          Den Pfad zur vorbereiteten Disk angeben. In diesem Beispiel <b>/home/user/disk.vdf</b>
 *          <small><i>Anmerkung: Oberhalb des Menüs wird nun der Mount-Status angezeigt.</i></small>
 *      </li>
 *      <li>Um die Disk zu mounten ><b>Mount/Unmount</b>< markieren, mit <b>[ENTER]</b> bestätigen</li>
 *      <li>Nun sind folgende Optionen möglich:
 *          <ul>
 *              <li>Disk Informationen anzeigen: <b>List Information</b></li>
 *              <li>Dateien auf Disk anzeigen: <b>Show Files</b></li>
 *          </ul>
 * 
 * </ol>
 * 
 * \section updateDocDE Dokumentation aktualisieren
 * 
 * Um die Dokumentation zu aktualisieren wird Doxygen benötigt.
 * <ol>
 *      <li>
 *          <h5>Doxywizard öffnen</h5>
 *          <p>Empfohlene Version: <a href="https://www.doxygen.nl/download.html">1.8.18</a></p>
 *      </li>
 *      <li>
 *          <h5>Mit Doxywizard die Doxyfile öffnen</h5>
 *          <p>Die doxyfile befindet sich im Verzeichnis [project_root]/doc</p>
 *      </li>
 *      <li>
 *          <h5>Doxywizard starten</h5>
 *          <p>Der Wizard wird die Dokumetation automatisch aktualisieren</p>
 *      </li>
 * </ol>
 */

/**
 * @brief This is a function pointer for an error handler
 * @details This fp is called iff an error occurred during one operation
 * @param code The error code of the operation
 */
void signalHandler(int signum){
    std::cout << utils::colors::RESET << std::dec << std::endl << utils::colors::RED 
            << "An important signal was caught: " << strsignal(signum) << "[" << signum << "]" << std::endl
            << "Please open a new issue ticket on github with your previous interaction, if possible including detailed description and screenshot" << utils::colors::RESET << std::endl;
    exit(signum);
}

void signalINT(int signum){
    if(signum == SIGINT){
        const char* entries[2] = {"Yes", "No"};
        utils::menu::menu_settings settings; 
        settings.preselected_row = 1;
        settings.sub_header = "You are going to quit the driver application, are you sure?";
        int choice = utils::menu::print(entries, 2, "BIOS", settings);
        if(0 == choice){
            exit(signum);
        }
    }else{
        exit(signum);
    }
}

#include <stdio.h>
#include <sys/stat.h>


/************************************************
 * Main function
 ***********************************************/
int main(int argc, const char* argv[]) {

    std::cout << utils::colors::CLEAR;
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-d") == 0){
            std::cout << utils::colors::MAGENTA << "DEBUG ENABLED" << utils::colors::RESET << std::endl;
        }
        std::cout<<argv[i];
    }   
    
    signal(SIGABRT, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGSEGV, signalHandler);

    signal(SIGINT, signalINT);
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

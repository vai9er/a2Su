#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "composite.h"
#include "processFD.h"
#include "systemWide.h"
#include "INode.h"
#include "thresh.h"


int main(int argc, char *argv[]) {
    int per_process = 0; // Flag for per-process option
    int system_wide = 0; // Flag for system-wide option
    int vnodes = 0; // Flag for vnodes option
    int composite = 0; // Flag for composite option
    int threshold = -1; // Threshold value
    int pid = -1; // Process ID

    static struct option long_options[] = {
        {"per-process", no_argument, 0, 'p'},
        {"systemWide", no_argument, 0, 's'},
        {"Vnodes", no_argument, 0, 'v'},
        {"composite", no_argument, 0, 'c'},
        {"threshold", required_argument, 0, 't'},
        {0, 0, 0, 0}
    };

    while (1) {
        int option_index = 0;
        int c = getopt_long(argc, argv,"t:", long_options,&option_index);
        
        if (c == -1)
            break;

        switch (c) {
            case 't':
                threshold = atoi(optarg); // Set threshold value from argument
                break;
            case 'p':
                per_process = 1; // Set per-process flag to true
                break;
            case 's':
                system_wide = 1; // Set system-wide flag to true
                break;
            case 'v':
                vnodes = 1; // Set vnodes flag to true
                break;
            case 'c':
                composite = 1; // Set composite flag to true
                break;
            case '?':
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }

   if (optind < argc) {
      pid=atoi(argv[optind]); 
      optind++;
      if(optind<argc){
          printf("Error: Too many positional arguments\n");
          exit(EXIT_FAILURE);
      }
   }
   
   //below is the logic we use for the flags. We really just care about what to do 
   //whether there is a positional arguement or not (basically the difference between printing ALL printing
   //ALL processes vs just one

    if (per_process != 0){
        if(pid == -1){
        getALLProcessFD();
        }
        else{
            print_fds(pid);
        }
    }

    if(system_wide != 0){
        if(pid == -1){
        allSWide();}

        else {systemWideFD(pid);}
    }

    if (vnodes != 0){
        if(pid == -1){
        printAllINode();}

        else {
            printINodeForPID(pid);
            }
    }

    if (composite != 0){
        if (pid == -1){
        printComposite();}

        else{
            char* str2;
            sprintf(str2, "%d", pid);
            printCompositeForPid(str2);
            }
    }

    if (threshold != -1){
        thresh(threshold);
    }

    if (per_process == 0 && system_wide == 0 && vnodes == 0 && composite == 0 && threshold == -1 && pid == -1){
        printComposite();
    }

    if (per_process == 0 && system_wide == 0 && vnodes == 0 && composite == 0 && threshold == -1 && pid != -1){
        char* str3;
        sprintf(str3, "%d", pid);
        printCompositeForPid(str3);
    }

    
   return EXIT_SUCCESS;
}
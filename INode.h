#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

// Function to print inodes for all processes
void printAllINode() {
    // Declare variables
    DIR *dirp;
    struct dirent *dp;
    char path[1024];
    struct stat sb;

    // Print header for output table
    printf("     PID     Inode\n");
    printf("==================\n");

    // Open directory stream to /proc directory
    dirp = opendir("/proc");
    
    // Loop through entries in /proc directory
    while ((dp = readdir(dirp)) != NULL) {
        // Check if entry name is a number (i.e. a process ID)
        if (dp->d_name[0] >= '0' && dp->d_name[0] <= '9') {
            // Create path string to process's file descriptor directory
            sprintf(path, "/proc/%s/fd", dp->d_name);
            
            // Open directory stream to file descriptor directory
            DIR *dirp2 = opendir(path);
            
            // If directory stream was successfully opened
            if (dirp2) {
                struct dirent *dp2;
                
                // Loop through entries in file descriptor directory
                while ((dp2 = readdir(dirp2)) != NULL) {
                    // Check if entry name is a number (i.e. a file descriptor)
                    if (dp2->d_name[0] >= '0' && dp2->d_name[0] <= '9') {
                        // Create path string to file descriptor
                        sprintf(path, "/proc/%s/fd/%s", dp->d_name, dp2->d_name);
                        
                        // Get file status information and store in sb variable
                        lstat(path,&sb);
                        
                        // Print process ID and inode number of file associated with file descriptor
                        printf("%10s %10ld\n", dp->d_name,
                               sb.st_ino);
                    }
                }
                
                // Close directory stream to file descriptor directory
                closedir(dirp2);
            }
        }
    }
    
    // Close directory stream to /proc directory
    closedir(dirp);
    printf("==================\n");
}


// Function to print VNODES table for a given process ID (pid)
void printINodeForPID(pid_t pid) {
    // Declare variables
    DIR *dirp;
    struct dirent *dp;
    char path[1024];
    struct stat sb;

    // Print header for output table
    printf("     PID     Inode\n");
    printf("==================\n");

    // Create path string to process's file descriptor directory
    sprintf(path, "/proc/%d/fd", pid);
    
    // Open directory stream to file descriptor directory
    DIR *dirp2 = opendir(path);
    
    // If directory stream was successfully opened
    if (dirp2) {
        struct dirent *dp2;
        
        // Loop through entries in the directory
        while ((dp2 = readdir(dirp2)) != NULL) {
            // Check if entry name is a number (i.e. a file descriptor)
            if (dp2->d_name[0] >= '0' && dp2->d_name[0] <= '9') {
                // Create path string to file descriptor
                sprintf(path, "/proc/%d/fd/%s", pid, dp2->d_name);
                
                // Get file status information and store in sb variable
                lstat(path,&sb);
                
                // Print process ID and inode number of file associated with file descriptor
                printf("%10d %10ld\n", pid,
                       sb.st_ino);
            }
        }
        
        // Close directory stream
        closedir(dirp2);
        // Print footer for output table
    }
    printf("==================\n");
}
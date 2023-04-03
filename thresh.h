#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void thresh(int x) {
    DIR *dirp; // Pointer to a directory stream
    struct dirent *dp; // Pointer to a directory entry structure
    char path[1024]; // Character array to store the path
    int pid; // Process ID
    int max_fd; // Maximum file descriptor

    printf("## Offending processes:\n");

    dirp = opendir("/proc"); // Open the /proc directory
    while ((dp = readdir(dirp)) != NULL) { // Read entries from the /proc directory
        if (sscanf(dp->d_name, "%d", &pid) == 1) { // Check if the entry name can be read as an integer (i.e. it is a process ID)
            sprintf(path, "/proc/%d/fd", pid); // Create the path to the process's file descriptor directory
            DIR *dirp2 = opendir(path); // Open the file descriptor directory
            if (dirp2 != NULL) {
                max_fd = 0; // Initialize max_fd to 0 for each process
                struct dirent *dp2;
                while ((dp2 = readdir(dirp2)) != NULL) { // Read entries from the file descriptor directory
                    int fd;
                    if (sscanf(dp2->d_name, "%d", &fd) == 1 && fd > max_fd)
                        max_fd = fd; // Update max_fd if a larger file descriptor is found
                }
                closedir(dirp2); 
                if (max_fd > x)
                    printf("%d (%d), ", pid, max_fd); 
            }
        }
    }
    closedir(dirp);
    printf("\n");
}

// int main(){
//     thresh(6);
//     return 0;
// }



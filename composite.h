#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void printComposite() {
    DIR *dirp;
    struct dirent *dp;
    char path[1024];
    char link[1024];
    struct stat sb;

    printf("     PID     FD      Filename                Inode\n");
    printf("=================================================\n");

    dirp = opendir("/proc"); // Open the /proc directory
    while ((dp = readdir(dirp)) != NULL) { // Read entries from the /proc directory
        if (dp->d_name[0] >= '0' && dp->d_name[0] <= '9') { // Check if the entry name is a number (i.e. a process ID)
            sprintf(path, "/proc/%s/fd", dp->d_name); // Create path to the process's file descriptor directory
            DIR *dirp2 = opendir(path); // Open the file descriptor directory
            if (dirp2) {
                struct dirent *dp2;
                while ((dp2 = readdir(dirp2)) != NULL) { // Read entries from the file descriptor directory
                    if (dp2->d_name[0] >= '0' && dp2->d_name[0] <= '9') { // Check if the entry name is a number (i.e. a file descriptor)
                        sprintf(path, "/proc/%s/fd/%s", dp->d_name, dp2->d_name); // Create path to the file descriptor
                        memset(link, 0, sizeof(link));
                        readlink(path, link, sizeof(link)-1); // Read the symbolic link to get the filename associated with the file descriptor
                        lstat(path,&sb);
                        printf("%10s %6s %22s %10ld\n", dp->d_name,
                               dp2->d_name,
                               link,
                               sb.st_ino); // Print process ID, file descriptor number, filename and inode number
                    }
                }
                closedir(dirp2);
            }
        }
    }
    closedir(dirp);
    printf("=================================================\n");
}

//does the exact same as printComposite() except its just for 1 pid
void printCompositeForPid(char *pid) {
    DIR *dirp;
    struct dirent *dp;
    char path[1024];
    char link[1024];
    struct stat sb;

    printf("     PID     FD      Filename                Inode\n");
    printf("=================================================\n");

    sprintf(path, "/proc/%s/fd", pid);
    DIR *dirp2 = opendir(path);
    if (dirp2) {
        struct dirent *dp2;
        while ((dp2 = readdir(dirp2)) != NULL) {
            if (dp2->d_name[0] >= '0' && dp2->d_name[0] <= '9') {
                sprintf(path, "/proc/%s/fd/%s", pid, dp2->d_name);
                memset(link, 0, sizeof(link));
                readlink(path, link, sizeof(link)-1);
                lstat(path,&sb);
                printf("%10s %6s %22s %10ld\n", pid,
                       dp2->d_name,
                       link,
                       sb.st_ino);
            }
        }
        closedir(dirp2);
    }
     printf("=================================================\n");
}
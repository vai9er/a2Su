#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>



#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

void print_header() {
    printf("    PID  FD  Filename\n");
    printf("    =========================\n");
}

// A function that prints out a footer for the table
void print_footer() {
    printf("    =========================\n");
}

// A function that prints out the file descriptors of a given process id
void systemWideFDPositional(pid_t pid) {
    // Open the directory /proc/<pid>/fd where the file descriptors are listed
    char path[256];
    sprintf(path, "/proc/%d/fd", pid);
    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("CANNOT OPEN %d", pid);
        return;
    }
    print_header();
    // Loop through each entry in the directory
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip . and .. entries
        if (entry->d_name[0] == '.') continue;
        
        // Convert the entry name to an integer (the file descriptor number)
        int fd = atoi(entry->d_name);
        
        // Get the path of the file descriptor using readlink()
        char fd_path[256];
        sprintf(fd_path, "/proc/%d/fd/%s", pid, entry->d_name);
        char path_buf[PATH_MAX];
        ssize_t len = readlink(fd_path, path_buf, sizeof(path_buf));
        if (len == -1) {
            perror("CANNOT READLINK");
            continue;
        }
        path_buf[len] = '\0';
        // Print out the process id, file descriptor number, and path
        printf("%8d %3d %s\n", pid, fd, path_buf);
    }

    // Close the directory
    closedir(dir);
    print_footer();
}

// A function that prints out the file descriptors of a given process id
void systemWideFD(pid_t pid) {
    // Open the directory /proc/<pid>/fd where the file descriptors are listed
    char path[256];
    sprintf(path, "/proc/%d/fd", pid);
    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("CANNOT OPEN %s\n", path);
        return;
    }
    // Loop through each entry in the directory
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip . and .. entries
        if (entry->d_name[0] == '.') continue;
        
        // Convert the entry name to an integer (the file descriptor number)
        int fd = atoi(entry->d_name);
        
        // Get the path of the file descriptor using readlink()
        char fd_path[256];
        sprintf(fd_path, "/proc/%d/fd/%s", pid, entry->d_name);
        char path_buf[PATH_MAX];
        ssize_t len = readlink(fd_path, path_buf, sizeof(path_buf));
        if (len == -1) {
            perror("CANNOT READLINK");
            continue;
        }
        path_buf[len] = '\0';
        // Print out the process id, file descriptor number, and path
        printf("%8d %3d %s\n", pid, fd, path_buf);
    }

    // Close the directory
    closedir(dir);
}

void allSWide() {
    // Open the /proc directory
    DIR *proc_dir = opendir("/proc");
    if (proc_dir == NULL) {
        return;
    }

    print_header();
    // Loop through each entry in the /proc directory
    struct dirent *entry;
    while ((entry = readdir(proc_dir)) != NULL) {
        // Skip entries that are not directories
        if (entry->d_type != DT_DIR) continue;

        // Skip entries that are not numbers (i.e. not process IDs)
        int pid = atoi(entry->d_name);
        if (pid == 0 && strcmp(entry->d_name, "0") != 0) continue;

        // Check if the process is owned by the user
        char status_path[256];
        sprintf(status_path, "/proc/%d/status", pid);
        FILE *status_file = fopen(status_path, "r");
        if (status_file == NULL) continue;

        uid_t uid;
        char line[256];
        while (fgets(line, sizeof(line), status_file) != NULL) {
            if (sscanf(line, "Uid: %d", &uid) == 1) break;
        }
        
       fclose(status_file);

       if (uid == getuid()) {
           systemWideFD(pid);
       }
   }

   closedir(proc_dir);
   print_footer();
}
// int main(){
//     printFDPath(8591);
//     return 0;
// }


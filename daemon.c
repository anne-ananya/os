#include <stdio.h>
#include <stdlib.h>

int main() {
    char buffer[128];
    int daemon_count = 0;
    
    // Use the 'ps' command to list all processes and capture their PIDs and names
    FILE *ps_output = popen("ps -e -o pid,comm=", "r");

    if (ps_output == NULL) {
        perror("Failed to run 'ps'");
        return 1;
    }

    printf("Daemon processes:\n");

    // Read the output line by line and count daemon processes
    while (fgets(buffer, sizeof(buffer), ps_output) != NULL) {
        int pid;
        char name[64];
        if (sscanf(buffer, "%d %s", &pid, name) == 2) {
            if (pid > 1) {  // Exclude the init process (PID 1)
                printf("PID: %d, Name: %s\n", pid, name);
                daemon_count++;
            }
        }
    }

    pclose(ps_output);

    printf("Number of daemon processes: %d\n", daemon_count);

    return 0;
}

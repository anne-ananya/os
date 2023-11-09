#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t child_pid = fork();

    if (child_pid == 0) {
        // This is the child process
        printf("Child process ID: %d\n", getpid());
        sleep(10);  // Sleep to keep the child process running

        // The child process will be an orphan after the parent exits
        printf("Orphan child's parent ID: %d\n", getppid());
    } else if (child_pid > 0) {
        // This is the parent process
        printf("Parent process ID: %d\n", getpid());

        // Parent process exits immediately
        exit(0);
    } else {
        perror("Fork failed");
    }

    return 0;
}

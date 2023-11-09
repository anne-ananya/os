#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipe1[2], pipe2[2];
    pid_t child1, child2;
    
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }
    
    child1 = fork();
    
    if (child1 < 0) {
        perror("Fork 1 failed");
        exit(1);
    } else if (child1 == 0) {
        // Child 1
close(pipe1[1]);
close(pipe2[0]);
close(pipe2[1]);

char message[100];
pid_t child1_pid = getpid();
pid_t parent_pid = getppid();
ssize_t bytesRead = read(pipe1[0], message, sizeof(message));

if (bytesRead > 0) {
    message[bytesRead] = '\0'; 
    std::cout << "Child 1: " << message << " " << child1_pid << ", my parent's ID is " << parent_pid << std::endl;
} else {
    perror("Child 1 read error");
}

close(pipe1[0]);

    } else {
        child2 = fork();
        
        if (child2 < 0) {
            perror("Fork 2 failed");
            exit(1);
        } else if (child2 == 0) {
            // Child 2
close(pipe1[0]);
close(pipe1[1]);
close(pipe2[1]);

char message[100];
pid_t child2_pid = getpid();
pid_t parent_pid = getppid();
ssize_t bytesRead = read(pipe2[0], message, sizeof(message));

if (bytesRead > 0) {
    message[bytesRead] = '\0'; 
    std::cout << "Child 2: " << message << " " << child2_pid << ", my parent's ID is " << parent_pid << std::endl;
} else {
    perror("Child 2 read error");
}

close(pipe2[0]);

        } else {
         // Parent
close(pipe1[0]);
close(pipe2[0]);

char message1[100];
char message2[100];

pid_t parent_pid = getpid();
pid_t child1_pid = child1;
pid_t child2_pid = child2;

snprintf(message1, sizeof(message1), "My process ID is %d, and my sibling's ID is %d", child1_pid, child2_pid);
snprintf(message2, sizeof(message2), "My process ID is %d, and my sibling's ID is %d", child2_pid, child1_pid);

write(pipe1[1], message1, sizeof(message1));
write(pipe2[1], message2, sizeof(message2));

close(pipe1[1]);
close(pipe2[1]);

wait(NULL);
wait(NULL);


        }
    }
    
    return 0;
}

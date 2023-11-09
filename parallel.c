#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_PROCESSES 100 
pid_t running_processes[MAX_PROCESSES]; 
int num_processes = 0; 

int fibonacci(int n) {
    if (n <= 1) {
        printf("Process %d calculating F(%d) = %d\n", getpid(), n, n);
        return n;
    } else {
        int fib1, fib2;
        pid_t child1, child2;

        child1 = fork();

        if (child1 < 0) {
            perror("Fork failed");
            exit(1);
        } else if (child1 == 0) {
            printf("Process %d calculating F(%d)...\n", getpid(), n - 1);
            fib1 = fibonacci(n - 1);
            exit(fib1);
        } else {
            running_processes[num_processes++] = child1; // Store child1's PID

            child2 = fork();

            if (child2 < 0) {
                perror("Fork failed");
                exit(1);
            } else if (child2 == 0) {
                printf("Process %d calculating F(%d)...\n", getpid(), n - 2);
                fib2 = fibonacci(n - 2);
                exit(fib2);
            } else {
                running_processes[num_processes++] = child2; // Store child2's PID

                int status1, status2;
                waitpid(child1, &status1, 0);
                waitpid(child2, &status2, 0);
                fib1 = WEXITSTATUS(status1);
                fib2 = WEXITSTATUS(status2);
                printf("Process %d calculated F(%d) + F(%d) = %d + %d = %d\n", getpid(), n - 1, n - 2, fib1, fib2, fib1 + fib2);

                
                printf("Processes running in parallel:\n");
                for (int i = 0; i < num_processes; i++) {
                    printf("PID: %d\n", running_processes[i]);
                }

                return fib1 + fib2;
            }
        }
    }
}

int main() {
    int n;
    printf("Enter the value of n to find the n-th Fibonacci number: ");
    scanf("%d", &n);

    if (n < 0) {
        printf("Invalid input. n should be non-negative.\n");
        return 1;
    }

    int result = fibonacci(n);
    printf("F(%d) = %d\n", n, result);

    return 0;
}


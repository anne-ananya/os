#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    } else {
        int fib1, fib2;
        pid_t child1, child2;
        child1 = fork();

        if (child1 < 0) {
            perror("Fork failed");
            exit(1);
        } else if (child1 == 0) {
            fib1 = fibonacci(n - 1);
            exit(fib1);
        } else {
            child2 = fork();

            if (child2 < 0) {
                perror("Fork failed");
                exit(1);
            } else if (child2 == 0) {
                fib2 = fibonacci(n - 2);
                exit(fib2);
            } else {
                int status;
                waitpid(child1, &status, 0);
                fib1 = WEXITSTATUS(status);
                waitpid(child2, &status, 0);
                fib2 = WEXITSTATUS(status);
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


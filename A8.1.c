#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int isLeapYear(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    key_t key = ftok("/tmp", 'A');
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);

    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    int *year = (int *)shmat(shmid, NULL, 0);

    if (year == (int *)-1) {
        perror("shmat");
        exit(1);
    }

    int pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        while (1) {
            int inputYear;

            printf("Enter a year: ");
            scanf("%d", &inputYear);

            if (inputYear < 1000 || inputYear > 9999) {
                printf("Invalid year. Please enter a 4-digit year.\n");
                continue;
            }

            *year = inputYear;

            if (isLeapYear(*year)) {
                printf("Child process: %d is a leap year\n", *year);
            } else {
                printf("Child process: %d is not a leap year\n", *year);
            }

            sleep(1);
        }
    } else {
        while (1) {
            (*year)++;
            sleep(1);
        }
    }

    shmdt(year);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

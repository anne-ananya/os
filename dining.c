#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5  // Number of philosophers

sem_t forks[N];
pthread_t philosophers[N];

int left(int i) {
    return i;
}

int right(int i) {
    return (i + 1) % N;
}

void *philosopher(void *arg) {
    int id = *((int *)arg);

    // Think
    printf("Philosopher %d is thinking\n", id);
    sleep(1);

    // Try to grab the lower-numbered fork first
    if (id % 2 == 0) {
        sem_wait(&forks[left(id)]);
        sem_wait(&forks[right(id)]);
    } else {
        sem_wait(&forks[right(id)]);
        sem_wait(&forks[left(id)]);
    }

    printf("Philosopher %d grabbed the forks and is eating\n", id);

    // Eat
    sleep(2);

    // Release the forks
    sem_post(&forks[left(id)]);
    sem_post(&forks[right(id)]);
    printf("Philosopher %d released the forks and finished eating\n", id);

    return NULL;
}

int main() {
    int i;
    int philosopher_ids[N];

    // Initialize semaphores (forks)
    for (i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1);
    }

    // Create philosopher threads
    for (i = 0; i < N; i++) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
        // Add a delay to allow each philosopher to complete one round of eating
        usleep(1000000); // 1 second
    }

    // Join philosopher threads
    for (i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy semaphores
    for (i = 0; i < N; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}

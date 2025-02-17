/*
 * File: semaphore_solution.c
 * Author: Daniel Grijalva
 * Course: CST-315
 * Assignment: Monitors and Semaphores - Print Queue Synchronization
 * Date: 02/16/25
 *
 * Description:
 * This program simulates a print queue system using semaphores to synchronize 
 * access to a shared printer. Multiple producer threads generate print jobs, 
 * while a single consumer thread processes them. Semaphores ensure mutual 
 * exclusion and control access to the buffer.
 *
 * Synchronization Mechanism:
 * - `sem_t empty`: Tracks available buffer slots.
 * - `sem_t full`: Tracks the number of pending jobs.
 * - `pthread_mutex_t mutex`: Ensures mutual exclusion while modifying the buffer.
 *
 * Usage:
 * Compile: gcc -pthread semaphore_solution.c -o semaphore_solution
 * Run: ./semaphore_solution
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

sem_t empty, full;
pthread_mutex_t mutex;
int buffer[BUFFER_SIZE];
int in = 0, out = 0;

void *producer(void *param) {
    int item;
    while (1) {
        item = rand() % 100;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("Produced: %d\n", item);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
    }
}

void *consumer(void *param) {
    int item;
    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        item = buffer[out];
        printf("Consumed: %d\n", item);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(2);
    }
}

int main() {
    pthread_t prod, cons;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    return 0;
}

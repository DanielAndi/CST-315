#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <queue>

using namespace std;

// Shared Buffer
queue<int> buffer;
const int BUFFER_SIZE = 5;

// Mutex and Condition Variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_produce = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consume = PTHREAD_COND_INITIALIZER;

// Produce function
int produce() {
    static int product = 0;
    return product++;
}

// Consume function
void consume(int item) {
    cout << "Consumed: " << item << endl;
}

// Producer Thread
void* producer(void* arg) {
    while (true) {
        pthread_mutex_lock(&mutex);

        while (buffer.size() == BUFFER_SIZE) {
            pthread_cond_wait(&cond_produce, &mutex); // Wait if buffer is full
        }

        int item = produce();
        buffer.push(item);
        cout << "Produced: " << item << endl;

        pthread_cond_signal(&cond_consume); // Signal consumer
        pthread_mutex_unlock(&mutex);

        sleep(1); // Simulate production time
    }
    return nullptr;
}

// Consumer Thread
void* consumer(void* arg) {
    while (true) {
        pthread_mutex_lock(&mutex);

        while (buffer.empty()) {
            pthread_cond_wait(&cond_consume, &mutex); // Wait if buffer is empty
        }

        int item = buffer.front();
        buffer.pop();
        consume(item);

        pthread_cond_signal(&cond_produce); // Signal producer
        pthread_mutex_unlock(&mutex);

        sleep(2); // Simulate consumption time
    }
    return nullptr;
}

// Main Function
int main() {
    pthread_t producerThread, consumerThread;

    // Create Threads
    pthread_create(&producerThread, nullptr, producer, nullptr);
    pthread_create(&consumerThread, nullptr, consumer, nullptr);

    // Join Threads
    pthread_join(producerThread, nullptr);
    pthread_join(consumerThread, nullptr);

    return 0;
}


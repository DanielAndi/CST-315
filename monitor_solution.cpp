/*
 * File: monitor_solution.cpp
 * Author: Daniel Grijalva
 * Course: CST-315
 * Assignment: Monitors and Semaphores - Print Queue Synchronization
 * Date: 02/16/25
 *
 * Description:
 * This program simulates a print queue system using a monitor-based approach. 
 * The monitor encapsulates synchronization mechanisms (mutex and condition variables) 
 * to manage access to a shared printer. Multiple producer threads generate print 
 * jobs, while a single consumer thread processes them in FIFO order.
 *
 * Synchronization Mechanism:
 * - `std::mutex mtx`: Ensures mutual exclusion.
 * - `std::condition_variable cond_var`: Controls thread execution.
 * - FIFO order is naturally enforced using a queue.
 *
 * Usage:
 * Compile: g++ -std=c++11 monitor_solution.cpp -o monitor_solution -pthread
 * Run: ./monitor_solution
 */

#include <iostream>
#include <queue>
#include <thread>
#include <condition_variable>
#include <chrono>

#define BUFFER_SIZE 5

std::queue<int> buffer;
std::mutex mtx;
std::condition_variable cond_var;

void producer() {
    int item;
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cond_var.wait(lock, [] { return buffer.size() < BUFFER_SIZE; });

        item = rand() % 100;
        buffer.push(item);
        std::cout << "Produced: " << item << std::endl;

        lock.unlock();
        cond_var.notify_all();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void consumer() {
    int item;
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cond_var.wait(lock, [] { return !buffer.empty(); });

        item = buffer.front();
        buffer.pop();
        std::cout << "Consumed: " << item << std::endl;

        lock.unlock();
        cond_var.notify_all();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int main() {
    std::thread prod(producer);
    std::thread cons(consumer);

    prod.join();
    cons.join();
    return 0;
}

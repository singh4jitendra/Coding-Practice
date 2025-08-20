#include <iostream>
#include <queue>
#include <thread>
#include <semaphore.h>
#include <mutex>
#include <chrono>
using namespace std;

#define BUFFER_SIZE 10 // Max buffer capacity

queue<int> bufferQ;
sem_t sem_empty, sem_full;
mutex mtx;

void producer() {
    while (true) {
        int num = rand() % 100; // Produce item
        sem_wait(&sem_empty);   // Wait for empty slot
        {
            lock_guard<mutex> lock(mtx);
            bufferQ.push(num);
            cout << "Produced: " << num << " | Buffer size: " << bufferQ.size() << endl;
        }
        sem_post(&sem_full); // Signal that item is available
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void consumer() {
    while (true) {
        sem_wait(&sem_full); // Wait for available item
        int item;
        {
            lock_guard<mutex> lock(mtx);
            item = bufferQ.front();
            bufferQ.pop();
            cout << "Consumed: " << item << " | Buffer size: " << bufferQ.size() << endl;
        }
        sem_post(&sem_empty); // Signal empty slot available
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

int main() {
    sem_init(&sem_empty, 0, BUFFER_SIZE); // Initially all slots empty
    sem_init(&sem_full, 0, 0);           // Initially no full slots

    cout << "Producer-Consumer Problem Simulation\n";
    cout << "Using semaphores and mutex for synchronization.\n";

    thread prod(producer);
    thread cons(consumer);

    prod.join();
    cons.join();

    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);

    return 0;
}


﻿#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
using namespace std;

mutex mu;
atomic<int> parentSemaphore(1); 
atomic<int> childSemaphore(0);  

void print(const string& text, atomic<int>& semaphore1, atomic<int>& semaphore2) {
    while (true) {
        unique_lock<mutex> lock(mu);
        if (semaphore1) {
            cout << text << endl;
            semaphore1 = 0;
            semaphore2 = 1;
            break;
        }
    }
}

void threadPrint() {
    for (int i = 0; i < 10; ++i) {
        print("Дочерний поток: пример...", childSemaphore, parentSemaphore);
    }
}

int main() {
    setlocale(LC_ALL, "Ru");

    thread childThread(threadPrint);

    for (int i = 0; i < 10; ++i) {
        print("Родительский поток: пример...", parentSemaphore, childSemaphore);
    }

    childThread.join();
    return 0;
}

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

mutex mu;

void print() {
    lock_guard<mutex> guard(mu);

    cout << std::this_thread::get_id() << ": Print text..." << endl;

}

void threadPrint() {
    for (int i = 0; i < 10; ++i) {
        print();
        chrono::milliseconds timeout(50);
        this_thread::sleep_for(timeout);
    }
}



int main() {
    setlocale(LC_ALL, "Ru");

    thread childThread(threadPrint);

    threadPrint();

    childThread.join();
    return 0;
}
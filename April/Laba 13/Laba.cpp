#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex mu;
condition_variable cv;
bool isChildTurn = false;

void print(const string& text, bool isChild) {
    unique_lock<mutex> lock(mu);
    cv.wait(lock, [isChild] { return isChild == isChildTurn; }); 
    cout << text << endl;
    isChildTurn = !isChildTurn;
    cv.notify_one(); 
}

void threadPrint() {
    for (int i = 0; i < 10; ++i) {
        print("Дочерний поток: пример...", true);
    }
}

int main() {
    setlocale(LC_ALL, "Ru");

    thread childThread(threadPrint);

    for (int i = 0; i < 10; ++i) {
        print("Родительский поток: пример...", false);
    }

    childThread.join();
    return 0;
}
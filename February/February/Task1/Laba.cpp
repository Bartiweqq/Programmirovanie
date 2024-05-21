S#include <iostream>
#include <thread>
using namespace std;

void print() {
    for (int i = 0; i < 10; ++i) {
         cout << "Дочерний поток: строка " << i + 1 <<  endl;
    }
} 

int main() {
    setlocale(LC_ALL, "Ru");

    thread childThread(print);
    childThread.join();
    for (int i = 0; i < 10; ++i) {
         cout << "Родительский поток: строка " << i + 1 <<  endl;
    }S

    return 0;
}
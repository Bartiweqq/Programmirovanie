S#include <iostream>
#include <thread>
#include <pthread.h>
#include <vector>
#include <string>
using namespace std;


void printStrings(const vector<string>& strings) {
    for (const auto& str : strings) {
        cout << str << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Ru");

    vector<string> strings1 = { "Поток 1: Строка 1",
        "Поток 1: Строка 2",
        "Поток 1: Строка 3" };
    vector<string> strings2 = { "Поток 2: Строка 1",
        "Поток 2: Строка 2",
        "Поток 2: Строка 3" };
    vector<string> strings3 = { "Поток 3: Строка 1",
        "Поток 3: Строка 2",
        "Поток 3: Строка 3" };
    vector<string> strings4 = { "Поток 4: Строка 1",
        "Поток 4: Строка 2",
        "Поток 4: Строка 3" };

    thread thread1(printStrings, strings1);
    thread thread2(printStrings, strings2);
    thread thread3(printStrings, strings3);
    thread thread4(printStrings, strings4);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();

    return 0;
}
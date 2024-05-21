#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <algorithm>
#include <unistd.h> // Для usleep

using namespace std;

// Функция, исполняемая потоком
void sleepAndPrint(const string& str) {
    // Время ожидания пропорционально длине строки
    usleep(str.length() * 100000); // 100000 микросекунд = 0.1 секунды на символ
    cout << str << endl;
}

int main() {
    // Ввод строк
    vector<string> strings;
    string input;
    while (getline(cin, input)) {
        strings.push_back(input);
        if (strings.size() >= 100) break; // Не более 100 строк
    }

    // Создание и запуск потоков
    vector<thread> threads;
    for (const auto& str : strings) {
        threads.push_back(thread(sleepAndPrint, str));
    }

    // Ожидание завершения всех потоков
    for (auto& th : threads) {
        th.join();
    }

    return 0;
}

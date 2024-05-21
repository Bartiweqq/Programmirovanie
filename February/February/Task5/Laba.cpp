#include <iostream>
#include <thread>
#include <pthread.h>
#include <chrono>
using namespace std;

void printExitMessage(void* arg) {
    cout << "Поток завершается." << endl;
}

void* printStrings(void* parm) {
    pthread_cleanup_push(printExitMessage, nullptr);
    for (int i = 1; i < 7; i++) {
        pthread_testcancel();
        cout << "Строка " << i << endl;
        
        this_thread::sleep_for(chrono::milliseconds(300));
    }
    pthread_cleanup_pop(1); // 1 для вызова функции очистки
    return NULL;
}

int main() {
    setlocale(LC_ALL, "Ru");

    int rc = 0;
    pthread_t thread1;
    rc = pthread_create(&thread1, NULL, printStrings, NULL);
    if (rc) {
        cout << "Ошибка: невозможно создать поток, код ошибки: " << rc << endl;
        return -1;
    }

    this_thread::sleep_for(chrono::milliseconds(2000));
    rc = pthread_cancel(thread1);
    if (rc) {
        cout << "Ошибка: невозможно отменить поток, код ошибки: " << rc << endl;
        return -1;
    }

    pthread_join(thread1, NULL);

    return 0;
}

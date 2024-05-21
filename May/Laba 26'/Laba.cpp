#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
using namespace std;

mutex mPrint;
void print(string str) {
    mPrint.lock();

    cout << str << endl;

    mPrint.unlock();
}

class MessageQueue {
private:
    queue<string> queue_;
    mutex mutex_;
    condition_variable messageAvailable_;
    const size_t maxSize_ = 10;

public:
    void put(const string& msg) {
        unique_lock<mutex> lock(mutex_);
        messageAvailable_.wait(lock, [this] { return queue_.size() < maxSize_; });
        queue_.push(msg.substr(0, 80)); // обрезаем до 80 символов
        lock.unlock();
        messageAvailable_.notify_one();
    }

    string get() {
        unique_lock<mutex> lock(mutex_);
        messageAvailable_.wait(lock, [this] { return !queue_.empty(); });
        string msg = queue_.front();
        queue_.pop();
        lock.unlock();
        return msg;
    }

    void drop() {
        lock_guard<mutex> lock(mutex_);
        messageAvailable_.notify_all();
    }
};

void producer(MessageQueue& queue, int id) {
    for (int i = 0; i < 5; ++i) {
        string msg = "Сообщение от производителя " + to_string(id) + ", номер " + to_string(i);
        queue.put(msg);
        this_thread::sleep_for(chrono::milliseconds(100)); // simulate some work
    }
}

void consumer(MessageQueue& queue, int id) {
    for (int i = 0; i < 5; ++i) {
        string msg = queue.get();
        print("Потребитель " + std::to_string(id) + " получил: " + msg);
        this_thread::sleep_for(chrono::milliseconds(500)); // simulate some work
    }
}

int main() {
    setlocale(LC_ALL, "Ru");

    MessageQueue queue;

    thread producer1(producer, ref(queue), 1);
    thread producer2(producer, ref(queue), 2);
    thread consumer1(consumer, ref(queue), 1);
    thread consumer2(consumer, ref(queue), 2);

    producer1.join();
    producer2.join();
    consumer1.join();
    consumer2.join();

    return 0;
}
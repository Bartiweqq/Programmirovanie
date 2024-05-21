S#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <vector>
using namespace std;

struct Node {
    string data;
    Node* next;
    mutex mtx;

    Node(const string& data) : data(data), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    mutex headMtx;

public:
    LinkedList() : head(nullptr) {}

    void insert(const string& data) {
        Node* newNode = new Node(data);
        lock_guard<mutex> lock(headMtx);
        newNode->next = head;
        head = newNode;
    }

    void print() {
        lock_guard<mutex> lock(headMtx);
        Node* current = head;
        while (current != nullptr) {
            lock_guard<mutex> nodeLock(current->mtx);
            cout << current->data << endl;
            current = current->next;
        }
    }

    void bubbleSort() {
        if (!head) return;
        bool swapped;
        Node* ptr1;
        Node* lptr = nullptr;
        do {
            swapped = false;
            lock_guard<mutex> lock(headMtx);
            ptr1 = head;
            Node* prev = nullptr;
            while (ptr1->next != lptr) {
                lock_guard<mutex> lock1(ptr1->mtx);
                lock_guard<mutex> lock2(ptr1->next->mtx);
                if (ptr1->data > ptr1->next->data) {
                    swap(ptr1->data, ptr1->next->data);
                    swapped = true;
                }
                prev = ptr1;
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);
    }
};

void sortingThread(LinkedList& list, int interval) {
    while (true) {
        this_thread::sleep_for(chrono::seconds(interval));
        list.bubbleSort();
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    LinkedList list;
    vector<thread> sortingThreads;
    int numThreads = 2;
    int intervalSeconds = 5;

    for (int i = 0; i < numThreads; ++i) {
        sortingThreads.emplace_back(sortingThread, ref(list), intervalSeconds);
    }

    while (true) {
        string input;
        getline(cin, input);
        if (input.empty()) {
            cout << "Текущее состояние списка:" << endl;
            list.print();
        } else {
            if (input.length() > 80) {
                for (size_t i = 0; i < input.length(); i += 80) {
                    string substr = input.substr(i, 80);
                    list.insert(substr);
                }
            } else {
                list.insert(input);
            }
        }
    }

    for (auto& thread : sortingThreads) {
        thread.join();
    }
    return 0;
}
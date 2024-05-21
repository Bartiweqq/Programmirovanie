#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
using namespace std;

struct Node {
    string data;
    Node* next;
    Node(const string& data) : data(data), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    mutex mtx;

public:
    LinkedList() : head(nullptr) {}

    void insert(const string& data) {
        lock_guard<mutex> lock(mtx);
        Node* newNode = new Node(data);
        newNode->next = head;
        head = newNode;
    }

    void print() {
        lock_guard<mutex> lock(mtx);
        Node* current = head;
        while (current != nullptr) {
            cout << current->data << endl;
            current = current->next;
        }
    }

    void bubbleSort() {
        lock_guard<mutex> lock(mtx);
        if (!head) return;
        bool swapped;
        Node* ptr1;
        Node* lptr = nullptr;
        do {
            swapped = false;
            ptr1 = head;
            while (ptr1->next != lptr) {
                if (ptr1->data > ptr1->next->data) {
                    swap(ptr1->data, ptr1->next->data);
                    swapped = true;
                }
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);
    }
};

void sortingThread(LinkedList& list) {
    while (true) {
        this_thread::sleep_for(chrono::seconds(5));
        list.bubbleSort();
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


    LinkedList list;
    thread sorting(sortingThread, ref(list));
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
    sorting.join();
    return 0;
}
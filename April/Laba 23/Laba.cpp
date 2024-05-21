#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

mutex listMutex;

struct Node {
    string data;
    Node* next;
    Node(const string& str) : data(str), next(nullptr) {}
};

Node* head = nullptr;

void AddToList(const string& str) {
    Node* newNode = new Node(str);
    lock_guard<mutex> lock(listMutex);
    if (!head) {
        head = newNode;
        return;
    }
    Node* current = head;
    while (current->next) {
        current = current->next;
    }
    current->next = newNode;
}

void sleepSort(const string& str) {
    int sleepTime = str.length() * 100 +1000;
    this_thread::sleep_for(chrono::milliseconds(sleepTime));
    AddToList(str);
}

void PrintList() {
    Node* current = head;
    while (current) {
        cout << current->data << endl;
        current = current->next;
    }
}

int main() {
    setlocale(LC_ALL, "Ru");

    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Ошибка открытия файла" << endl;
        return 1;
    }

    vector<string> strings;

    string input;
    while (getline(inputFile, input)) {
        strings.push_back(input);
    }

    inputFile.close();

    vector<thread> threads;
    for (const auto& str : strings) {
        threads.push_back(thread(sleepSort, str));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    PrintList();

    Node* current = head;
    while (current) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }

    return 0;
}
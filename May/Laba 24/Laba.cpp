#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

mutex mtxA;
mutex mtxB;
mutex mtxC;
mutex mtxBolt;
condition_variable cv;

int A_count = 0;
int B_count = 0;
int C_count = 0;

const int MAX_PRODUCTS = 5;

struct Bolt {
    bool isAReady;
    bool isBReady;
    bool isCReady;
    bool isProduced;
};

vector<Bolt> initBoltsVector() {
    vector<Bolt> bolts(MAX_PRODUCTS);
    return bolts;
}

void makeA(vector<Bolt>& bolts) {
    for (int i = 0; i < MAX_PRODUCTS; ++i) {
        this_thread::sleep_for(chrono::seconds(1));
        {
            lock_guard<mutex> lck(mtxA);
            A_count++;
            bolts[i].isAReady = true;
            cout << "Деталь A изготовлена. Всего произведено: " << A_count << endl;
        }
        cv.notify_all();
    }
}

void makeB(vector<Bolt>& bolts) {
    for (int i = 0; i < MAX_PRODUCTS; ++i) {
        this_thread::sleep_for(chrono::seconds(2));
        {
            lock_guard<mutex> lck(mtxB);
            B_count++;
            bolts[i].isBReady = true;
            cout << "Деталь B изготовлена. Всего произведено: " << B_count << endl;
        }
        cv.notify_all();
    }
}

void makeC(vector<Bolt>& bolts) {
    for (int i = 0; i < MAX_PRODUCTS; ++i) {
        this_thread::sleep_for(chrono::seconds(3));
        {
            lock_guard<mutex> lck(mtxC);
            C_count++;
            bolts[i].isCReady = true;
            cout << "Деталь C изготовлена. Всего произведено: " << C_count << endl;
        }
        cv.notify_all();
    }
}

void makeBolt(vector<Bolt>& bolts) {
    for (int i = 0; i < MAX_PRODUCTS; ++i) {
        unique_lock<mutex> lck(mtxBolt);
        cv.wait(lck, [&bolts, i] {
            return (bolts[i].isAReady && bolts[i].isBReady && bolts[i].isCReady) && !bolts[i].isProduced;
            });
        bolts[i].isProduced = true;
        cout << "Винтик готов!" << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Ru");

    vector<Bolt> boltsVector = initBoltsVector();

    thread threadA(makeA, ref(boltsVector));
    thread threadB(makeB, ref(boltsVector));
    thread threadC(makeC, ref(boltsVector));
    thread boltThread(makeBolt, ref(boltsVector));

    threadA.join();
    threadB.join();
    threadC.join();
    boltThread.join();

    return 0;
}
#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <string>
#include <thread>
#include <future>
#include <math.h>
#include <conio.h>
#include <csignal>
#include <atomic>
#include <cmath>
#include <signal.h>

using namespace std;

int iterations = 1000000;

bool* isFinishedExecuting = new bool(false);

long double calculate_pi_portion(int start, int end) {
    long double sum = 0;
    for (int i = start; i <= end; i++) {
        sum += ((pow(-1, i)) / (2 * i + 1));


        if (i % iterations == 0 && *isFinishedExecuting == true) {
            break;
        }

    }
    return sum;
}





void cancel_program(int num) {
    cout << "Cancel!!!!!!!!!!" << endl;
    *isFinishedExecuting = true;
}


int main()
{
    signal(SIGINT, cancel_program);
    const int NUM_THREADS = 10;
    int iterations_per_thread = 10000000000 / NUM_THREADS;
    long double pi = 0;

    std::vector<std::future<long double>> futures(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++) {
        int start = i * iterations_per_thread;
        int end = (i == NUM_THREADS - 1) ? 10000000000 - 1 : start + iterations_per_thread - 1;
        futures[i] = std::async(std::launch::async, calculate_pi_portion, start, end);
    }
    for (auto& future : futures) {
        long double result = future.get();
        pi = pi + result;
    }

    pi = pi * 4;

    std::cout << setprecision(10) << pi << endl;
    delete isFinishedExecuting;

    return 0;
}
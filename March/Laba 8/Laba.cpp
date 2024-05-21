#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <sys/stat.h>
//#include <dirent.h>
#include <pthread.h>
#include <filesystem>
#include <chrono>
#include <future>
#include <math.h>
#include <conio.h>
#include <signal.h>
using namespace std;


int iterations = 200000000;


long double calculate_pi_portion(int start, int end) {
    long double sum = 0;
    for (int i = start; i <= end; i++) {
        sum += ((pow(-1, i)) / (2 * i + 1));
    }
    return sum;
}

void task8Single() {
    long double pi = 0;
    for (int i = 0; i < iterations; i++) {
        pi += ((pow(-1, i)) / (2 * i + 1));
    }
    pi *= 4;
    cout << setprecision(10) << pi << endl;
}

void task8Multi() {
    const int NUM_THREADS = 10;
    int iterations_per_thread = iterations / NUM_THREADS;
    long double pi = 0;
    
    std::vector<std::future<long double>> futures(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++) {
        int start = i * iterations_per_thread;
        int end = (i == NUM_THREADS - 1) ? iterations - 1 : start + iterations_per_thread - 1;
        futures[i] = std::async(std::launch::async, calculate_pi_portion, start, end);
    }
    for (auto& future : futures) {
        long double result = future.get();
        pi = pi + result;
    }

    pi = pi * 4;

    std::cout << setprecision(10) << pi << endl;
}


int main()
{
    auto start = chrono::high_resolution_clock::now();
    task8Single();
    auto end = chrono::high_resolution_clock::now();
    auto ms_int = chrono::duration_cast<chrono::milliseconds>(end - start);
    std::cout << "Single thread: " << ms_int.count() << " ms\n";
        
    
    start = chrono::high_resolution_clock::now();
    task8Multi();
    end = chrono::high_resolution_clock::now();
    ms_int = chrono::duration_cast<chrono::milliseconds>(end - start);
    std::cout << "Multi thread: " << ms_int.count() << " ms\n";

    return 0;
}
#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <sys/stat.h>
#include <pthread.h>
#include <chrono>
//#include <future>
//#include <math.h>
//#include <conio.h>
#include <signal.h>
#include <condition_variable>
using namespace std;

mutex printM;
void print(string str) {
    printM.lock();

    cout << str << endl;
  
    printM.unlock();
}

int main()
{
    setlocale(LC_ALL, "Russian");
    string text;
    const int number_of_philosophers = 5;

    struct Forks {
    public:
        Forks() : is_left_available(true), is_right_available(true) {}
        std::mutex mu;
        std::condition_variable left_cond, right_cond;
        bool is_left_available;
        bool is_right_available;
    };

    auto eat = [](Forks& left_fork, Forks& right_fork, int philosopher_number) {
        std::unique_lock<std::mutex> lock(left_fork.mu);

        left_fork.left_cond.wait(lock, [&] { return left_fork.is_left_available; });

        int right_fork_index = (number_of_philosophers + (philosopher_number - 1)) % number_of_philosophers;
        {
            std::unique_lock<std::mutex> right_lock(right_fork.mu);
            right_fork.left_cond.wait(lock, [&] { return right_fork.is_right_available; });
            
            print("Философ " + std::to_string(philosopher_number) + " ест...");
           
            std::chrono::milliseconds timeout(1500);
            std::this_thread::sleep_for(timeout);
            print("Философ " + std::to_string(philosopher_number) + " закончил есть и думает...");
            
            left_fork.is_left_available = true;
            left_fork.left_cond.notify_one();  

            right_fork.is_right_available = true;
            right_fork.right_cond.notify_one();
        }
        };


    Forks forks[number_of_philosophers];
    std::thread philosopher[number_of_philosophers];

    for (int i = 0; i < number_of_philosophers; ++i) {
        auto philosopher_number = i + 1;
        print("Философ " + std::to_string(philosopher_number) + " думает...");
       
        int previous_fork_index = (number_of_philosophers + (i - 1)) % number_of_philosophers;
        philosopher[i] = std::thread(eat, std::ref(forks[i]), std::ref(forks[previous_fork_index]), philosopher_number);
    }

    for (auto& ph : philosopher) {
        ph.join();
    }

    return 0;
}

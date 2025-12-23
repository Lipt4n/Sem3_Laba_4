#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <map>
#include "syncs.hpp"

using namespace std;

int main() {
    const int num_threads = 4;
    const int num_chars = 100;
    vector<thread> threads;
    map<int, string> syncs = {
        {0, "Without Sync"},
        {1, "Mutex"},
        {2, "Semaphore"},
        {3, "Barrier"},
        {4, "SpinLock"},
        {5, "SpinWait"},
        {6, "Monitor"},
    };

    cout << "Select mode:" << endl;
    for (auto& i : syncs) {
        cout << i.first << ". " << i.second << endl;
    }

    int selected;
    cin >> selected;

    auto start = chrono::steady_clock::now();
    for (int i = 0; i < num_threads; ++i) {
        switch (selected) {
            case (0): {
                threads.emplace_back(print_random_chars, i, num_chars);
                break;
            }
            case (1): {
                threads.emplace_back(print_random_chars_mutex, i, num_chars);
                break;
            }
            case (2): {
                threads.emplace_back(print_random_chars_sem, i, num_chars);
                break;
            }
            case (3): {
                threads.emplace_back(print_random_chars_barrier, i, num_chars);
                break;
            }
            case (4): {
                threads.emplace_back(print_random_chars_spinlock, i, num_chars);
                break;
            }
            case (5): {
                threads.emplace_back(print_random_chars_spinwait, i, num_chars);
                break;
            }
            case (6): {
                threads.emplace_back(print_random_chars_monitor, i, num_chars);
                break;
            }
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = chrono::steady_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Time: " << elapsed.count() << " seconds" << endl;

    return 0;
}
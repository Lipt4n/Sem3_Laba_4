#include <random>
#include <iostream>
#include <semaphore>
#include <mutex> 
#include <barrier>
#include <atomic>
#include <thread>
#include "syncs.hpp"

using namespace std;

char gen_char() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(33, 126);
    return static_cast<char>(dis(gen));
}

//WITHOUT SYNC
void print_random_chars(int thread_id, int num_chars) {
    for (int i = 0; i < num_chars; ++i) {
        char c = gen_char();
        cout << "Thread " << thread_id << ": " << c << endl;
    }
}

//MUTEX
mutex cout_mutex;

void print_random_chars_mutex(int thread_id, int num_chars) {
    for (int i = 0; i < num_chars; ++i) {
        char c = gen_char();
        lock_guard<mutex> lock(cout_mutex);
        cout << "Thread " << thread_id << ": " << c << endl;
    }
}

//SEMAPHORE
counting_semaphore<1> cout_sem(1);

void print_random_chars_sem(int thread_id, int num_chars) {
    for (int i = 0; i < num_chars; ++i) {
        char c = gen_char();
        cout_sem.acquire();
        cout << "Thread " << thread_id << ": " << c << endl;
        cout_sem.release();
    }
}

//BARRIER
barrier sync_barrier(4);

void print_random_chars_barrier(int thread_id, int num_chars) {
    for (int i = 0; i < num_chars; ++i) {
        char c = gen_char();
        sync_barrier.arrive_and_wait();
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Thread " << thread_id << ": " << c << endl;
        }
    }
}

//SPINLOCK
class SpinLock {
    atomic<bool> locked{false};
public:
    void lock() {
        while (locked.exchange(true, memory_order_acquire));
    }
    void unlock() {
        locked.store(false, memory_order_release);
    }
};

SpinLock cout_spinlock;

void print_random_chars_spinlock(int thread_id, int num_chars) {
    for (int i = 0; i < num_chars; ++i) {
        char c = gen_char();
        cout_spinlock.lock();
        cout << "Thread " << thread_id << ": " << c << endl;
        cout_spinlock.unlock();
    }
}

//SPINWAIT
class SpinWait {
private:
    atomic<bool> locked{false};
public:
    void lock() {
        while (locked.exchange(true, std::memory_order_acquire)) {
            this_thread::yield();
        }
    }
    void unlock() {
        locked.store(false, std::memory_order_release);
    }
};

SpinWait cout_spinwait;

void print_random_chars_spinwait(int thread_id, int num_chars) {
    for (int i = 0; i < num_chars; ++i) {
        char c = gen_char();
        cout_spinwait.lock();
        cout << "Thread " << thread_id << ": " << c << endl;
        cout_spinwait.unlock();
    }
}

//MONITOR
mutex mon_mutex;
condition_variable mon_cv;
atomic<bool> ready{true};

void print_random_chars_monitor(int thread_id, int num_chars) {
    for (int i = 0; i < num_chars; ++i) {
        char c = gen_char();
        unique_lock<mutex> lock(mon_mutex);
        while (!ready) {
            mon_cv.wait(lock);
        }
        ready = false;
        cout << "Thread " << thread_id << ": " << c << endl;
        ready = true;
        mon_cv.notify_one();
    }
}
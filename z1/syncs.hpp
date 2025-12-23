#pragma once

void print_random_chars(int thread_id, int num_chars);

void print_random_chars_mutex(int thread_id, int num_chars);

void print_random_chars_sem(int thread_id, int num_chars);

void print_random_chars_barrier(int thread_id, int num_chars);

void print_random_chars_spinlock(int thread_id, int num_chars);

void print_random_chars_spinwait(int thread_id, int num_chars);

void print_random_chars_monitor(int thread_id, int num_chars);
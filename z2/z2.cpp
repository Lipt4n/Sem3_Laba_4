#include <iostream>
#include <vector>
#include <thread>
#include <set>
#include <chrono>
#include <algorithm>

using namespace std;

using Domino = pair<int, int>;

Domino normalize(int a, int b) {
    return {min(a, b), max(a, b)};
}

set<Domino> process_sequential(const vector<Domino>& data) {
    set<Domino> existing;
    for (const auto& d : data) {
        existing.insert(normalize(d.first, d.second));
    }
    return existing;
}

void process_chunk(const vector<Domino>& data, int start, int end, set<Domino>* local_set) {
    for (int i = start; i < end; i++) {
        const auto& d = data[i];
        local_set->insert(normalize(d.first, d.second));
    }
}

set<Domino> process_parallel(const vector<Domino>& data, int num_threads) {
    vector<thread> threads;
    vector<set<Domino>> local_sets(num_threads);
    int chunk_size = data.size() / num_threads;
    int remainder = data.size() % num_threads;

    int start = 0;
    for (int i = 0; i < num_threads; i++) {
        int end = start + chunk_size + (i < remainder ? 1 : 0);
        threads.emplace_back(process_chunk, cref(data), start, end, &local_sets[i]);
        start = end;
    }

    for (auto& t : threads) {
        t.join();
    }

    set<Domino> existing;
    for (const auto& local : local_sets) {
        existing.insert(local.begin(), local.end());
    }
    return existing;
}

int main() {
    int N, T;
    cout << "Введите размер массива данных: "; 
    cin >> N;
    cout << "Введите количество параллельных потоков: "; 
    cin >> T;

    vector<Domino> data(N);
    for (int i = 0; i < N; i++) {
        cout << "Введите кость № " << i+1 << ": ";
        cin >> data[i].first >> data[i].second;
    }

    set<Domino> full_set;
    for (int i = 0; i <= 6; i++) {
        for (int j = i; j <= 6; j++) {
            full_set.insert({i, j});
        }
    }

    // Sequential
    auto start_seq = chrono::steady_clock::now();
    auto existing_seq = process_sequential(data);
    auto end_seq = chrono::steady_clock::now();
    double time_seq = chrono::duration<double>(end_seq - start_seq).count() * 1000;

    // Parallel
    auto start_par = chrono::steady_clock::now();
    auto existing_par = process_parallel(data, T);
    auto end_par = chrono::steady_clock::now();
    double time_par = chrono::duration<double>(end_par - start_par).count() * 1000;

    // Недостающие (из full minus existing, set_difference требует sorted, sets уже sorted)
    vector<Domino> missing;
    set_difference(full_set.begin(), full_set.end(), existing_par.begin(), existing_par.end(), back_inserter(missing));

    // Вывод
    cout << "Sequential time: " << time_seq << " ms\n";
    cout << "Parallel time: " << time_par << " ms\n";
    cout << "Missing dominoes:\n";
    for (const auto& m : missing) {
        cout << "[" << m.first << " " << m.second << "]\n";
    }

    return 0;
}
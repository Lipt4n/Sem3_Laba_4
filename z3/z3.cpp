#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

const int RESOURCE_TYPES = 4;

struct Process {
    vector<int> max;
    vector<int> allocation;
    vector<int> need;
};

void inputData(int& num_processes, int& num_resources, vector<Process>& processes, vector<int>& available) {
    cout << "Введите количество процессов: ";
    cin >> num_processes;

    cout << "Введите количество типов ресурсов: ";
    cin >> num_resources;

    processes.resize(num_processes);
    available.resize(num_resources);

    cout << "\nВведите Available (доступные ресурсы): ";
    for (int j = 0; j < num_resources; ++j) {
        cin >> available[j];
    }

    cout << "\nДля каждого процесса введите:\n";
    cout << "  Max (максимум) | Allocation (уже выделено)\n";
    cout << "  Формат: M1 M2 ... Allocation1 Allocation2 ...\n\n";

    for (int i = 0; i < num_processes; i++) {
        processes[i].max.resize(num_resources);
        processes[i].allocation.resize(num_resources);
        processes[i].need.resize(num_resources);

        cout << "Процесс " << i << ": ";
        for (int j = 0; j < num_resources; j++) {
            cin >> processes[i].max[j];
        }
        for (int j = 0; j < num_resources; j++) {
            cin >> processes[i].allocation[j];
        }

        // Вычисляем need
        for (int j = 0; j < num_resources; j++) {
            processes[i].need[j] = processes[i].max[j] - processes[i].allocation[j];
            if (processes[i].need[j] < 0) {
                cout << "Ошибка: Allocation > Max для процесса " << i << "\n";
                exit(1);
            }
        }
    }
}

bool canAllocate(const vector<int>& need, const vector<int>& available) {
    for (int j = 0; j < RESOURCE_TYPES; j++) {
        if (need[j] > available[j]) {
            return false;
        }
    }
    return true;
}

void executeProcess(vector<int>& available, const vector<int>& allocation) {
    for (int j = 0; j < RESOURCE_TYPES; j++) {
        available[j] += allocation[j];
    }
}

// Основной алгоритм банкира
bool bankersAlgorithm(int num_processes, int num_resources, vector<Process>& processes, vector<int>& available) {
    vector<int> work = available;
    vector<bool> finished(num_processes, false);
    vector<int> safe_sequence;

    int count = 0;
    while (count < num_processes) {
        bool found = false;

        for (int i = 0; i < num_processes; i++) {
            if (!finished[i] && canAllocate(processes[i].need, work)) {
                
                for (int j = 0; j < num_resources; j++) {
                    work[j] += processes[i].allocation[j];
                }
                finished[i] = true;
                safe_sequence.push_back(i);
                found = true;
                count++;
                break;
            }
        }

        if (!found) {
            return false;
        }
    }

    cout << "\nСистема в БЕЗОПАСНОМ состоянии.\n";
    cout << "Безопасная последовательность: ";
    for (int p : safe_sequence) {
        cout << "P" << p << " ";
    }
    cout << "\n";

    return true;
}

int main() {
    int num_processes, num_resources;
    vector<Process> processes;
    vector<int> available;

    inputData(num_processes, num_resources, processes, available);

    cout << "\n=== Алгоритм банкира ===\n";
    if (bankersAlgorithm(num_processes, num_resources, processes, available)) {
        cout << "Система может безопасно выделять ресурсы.\n";
    } else {
        cout << "ОШИБКА: Возможен DEADLOCK!\n";
    }

    return 0;
}
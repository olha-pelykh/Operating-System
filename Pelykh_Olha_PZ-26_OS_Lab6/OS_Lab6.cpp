#include <iostream>
#include <pthread.h>
#include <cmath>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#include <sched.h>
#include <vector>
#include <semaphore.h>
#include <chrono>

using namespace std;

enum SyncType { NO_SYNC, SEMAPHORE, BARIER };

double spended_time = 0;
double sum = 0.0;
sem_t sem;
pthread_barrier_t barrier;
sem_t suspend_sem;

typedef struct {
    double a;
    double b;
    int n;
    pid_t tid;
    SyncType syncType;

} function_data;

double f(double x) {
    return x * sin(x / 2);
}

void* integratefunc(void* args) {
    function_data* data = (function_data*)args;
    double a = data->a;
    double b = data->b;
    int n = data->n;

    pid_t tid = syscall(SYS_gettid);
    data->tid = tid;
    sem_wait(&suspend_sem);

    auto start = std::chrono::high_resolution_clock::now();

    double h = (b - a) / (double)n;
    double tempsum = (f(a) + f(b)) / 2.0;

    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        tempsum += f(x);
    }

    switch (data->syncType) {
    case NO_SYNC:
        sum += tempsum;
        break;
    case BARIER:
        pthread_barrier_wait(&barrier);
        sum += tempsum;
        break;
    case SEMAPHORE:
        sem_wait(&sem);
        sum += tempsum;
        sem_post(&sem);
        break;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    cout << "Thread " << tid << " execution time: "
        << elapsed.count() << " seconds" << endl;
    spended_time += elapsed.count();
    pthread_exit(0);
}

int main() {
    cout << "Enter the number of threads: ";
    int thread_num;
    cin >> thread_num;

    std::cout << "Choose synchronization:\n"
        << "1. No Sync\n"
        << "2. Barier\n"
        << "3. Semaphore\n";
    int syncChoice;
    std::cin >> syncChoice;

    SyncType syncType;
    switch (syncChoice) {
    case 1: syncType = NO_SYNC; break;
    case 2: syncType = BARIER; break;
    case 3: syncType = SEMAPHORE; break;
    default: syncType = NO_SYNC;
    }

    pthread_t tid[thread_num];
    pthread_attr_t attr;
    function_data data[thread_num];

    sem_init(&sem, 0, 1);
    sem_init(&suspend_sem, 0, 0);
    pthread_barrier_init(&barrier, NULL, thread_num);

    pthread_attr_init(&attr);

    double a = 0, b = 100;
    int n = 10000;

    for (int i = 0; i < thread_num; i++) {
        data[i].a = a + i * (b - a) / thread_num;
        data[i].b = a + (i + 1) * (b - a) / thread_num;
        data[i].n = n / thread_num;
        data[i].syncType = syncType;

        if (pthread_create(&tid[i], &attr, integratefunc, (void*)&data[i]) != 0) {
            cerr << "Error creating thread " << i + 1 << endl;
            return 1;
        }
    }

    int choice = 1;
    while (choice) {
        cout << "Choose the action you want to execute:\n0. Resume all threads\n2. Change thread priority by tid\n3. Set thread CPU affinity\n4. Show all threads information\n5. Detach thread\n6. End thread\n";
        cin >> choice;

        switch (choice) {
        case 0:
            for (int i = 0; i < thread_num; i++) {
                sem_post(&suspend_sem);
            }
            break;

        case 2: {
            cout << "Enter the thread ID (tid) for which you want to change the priority: ";
            pid_t target_tid;
            cin >> target_tid;
            cout << "Enter the nice value to set for thread " << target_tid << ": ";
            int nice_value;
            cin >> nice_value;
            if (setpriority(PRIO_PROCESS, target_tid, nice_value) == -1) {
                cerr << "Error setting priority for thread " << target_tid << endl;
            }
            break;
        }
        case 3: {
            cout << "Enter the thread ID (tid) for which you want to set CPU affinity: ";
            pid_t target_tid;
            cin >> target_tid;
            cout << "Enter the CPU core number (0-based index): ";
            int core;
            cin >> core;
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(core, &cpuset);
            if (sched_setaffinity(target_tid, sizeof(cpuset), &cpuset) == -1) {
                cerr << "Error setting CPU affinity for thread " << target_tid << endl;
            }
            break;
        }

        case 4:
            for (int i = 0; i < thread_num; i++) {
                pid_t tid = data[i].tid;
                cout << "Thread ID: " << tid << endl;
                cout << "  Range: [" << data[i].a << ", " << data[i].b << "]\n";
                cout << "  Points: " << data[i].n << "\n";

                int priority = getpriority(PRIO_PROCESS, tid);
                cout << "Priority: " << priority << endl;

                cpu_set_t cpuset;
                CPU_ZERO(&cpuset);
                if (pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) == 0) {
                    cout << "Affinity cores: ";
                    for (int j = 0; j < CPU_SETSIZE; j++) {
                        if (CPU_ISSET(j, &cpuset)) {
                            cout << j << " ";
                        }
                    }
                    cout << endl;
                }
                else {
                    cerr << "Error getting CPU affinity for thread " << tid << endl;
                }
            }
            break;
        case 5: {
            cout << "Enter the thread ID (tid) you want to detach: ";
            pid_t target_tid;
            cin >> target_tid;

            bool found = false;
            for (int i = 0; i < thread_num; i++) {
                if (data[i].tid == target_tid) {
                    if (pthread_detach(tid[i]) == 0) {
                        cout << "Thread " << target_tid << " has been detached." << endl;
                        found = true;
                    }
                    else {
                        cerr << "Error detaching thread " << target_tid << endl;
                    }
                    break;
                }
            }
            if (!found) {
                cerr << "Thread ID " << target_tid << " not found among active threads." << endl;
            }
            break;
        }
        case 6: {
            cout << "Enter the thread ID (tid) you want to terminate: ";
            pid_t target_tid;
            cin >> target_tid;

            bool found = false;
            for (int i = 0; i < thread_num; i++) {
                if (data[i].tid == target_tid) {
                    if (pthread_cancel(tid[i]) == 0) {
                        cout << "Thread " << target_tid << " has been terminated." << endl;
                        found = true;
                    }
                    else {
                        cerr << "Error terminating thread " << target_tid << endl;
                    }
                    break;
                }
            }
            if (!found) {
                cerr << "Thread ID " << target_tid << " not found among active threads." << endl;
            }
            break;
        }
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }

    for (int i = 0; i < thread_num; i++) {
        pthread_join(tid[i], NULL);
    }

    cout << "SPENDED TIME: " << spended_time << endl;
    sem_destroy(&sem);
    sem_destroy(&suspend_sem);
    pthread_barrier_destroy(&barrier);

    cout << "Integral result: " << sum << endl;

    return 0;
}


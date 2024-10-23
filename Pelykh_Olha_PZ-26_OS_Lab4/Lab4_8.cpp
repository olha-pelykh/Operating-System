#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <cmath>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include <sched.h>
#include <algorithm>

struct ProcessInfo {
    pid_t pid;
    std::string name;
    int priority;
    int state; // 0: running, 1: stopped
};

// Табулювання функції
void tabulateFunction(double start, double end, double step) {
    for (double x = start; x <= end; x += step) {
        double result = x / (tan(x) + exp(x)); // f(x) = x / (tan(x) + e^x)
        // Тут можемо зберігати результати у файл, якщо потрібно
    }
}

// Пошук та заміна слова у файлі
void searchAndReplace(const std::string& filename, const std::string& word, const std::string& replacement) {
    std::ifstream file(filename);
    std::string line;
    std::string tempFile = "temp.txt";
    std::ofstream outFile(tempFile);

    while (std::getline(file, line)) {
        size_t pos = line.find(word);
        while (pos != std::string::npos) {
            line.replace(pos, word.length(), replacement);
            pos = line.find(word, pos + replacement.length());
        }
        outFile << line << std::endl;
    }

    file.close();
    outFile.close();
    remove(filename.c_str());
    rename(tempFile.c_str(), filename.c_str());
}

// Виведення інформації про процеси
void printProcessInfo(const std::vector<ProcessInfo>& processes) {
    std::cout << "PID\tName\t\tPriority\tState\n";
    for (const auto& proc : processes) {
        std::cout << proc.pid << "\t" << proc.name << "\t\t" 
                  << proc.priority << "\t\t" 
                  << (proc.state == 0 ? "Running" : "Stopped") << std::endl;
    }
}

// Встановлення прив'язки до ядра
void setAffinity(pid_t pid, const std::vector<int>& cores) {
    cpu_set_t cpuSet;
    CPU_ZERO(&cpuSet);

    for (int core : cores) {
        CPU_SET(core, &cpuSet);
    }

    if (sched_setaffinity(pid, sizeof(cpuSet), &cpuSet) == -1) {
        std::cerr << "Помилка при встановленні прив'язки до ядер\n";
    } else {
        std::cout << "Прив'язка процесу " << pid << " до ядер ";
        for (int core : cores) {
            std::cout << core << " ";
        }
        std::cout << "виконана успішно\n";
    }
}

// Головна функція
int main() {
    std::vector<ProcessInfo> processes;

    while (true) {
        std::cout << "\nМеню:\n";
        std::cout << "1. Створити процес табулювання\n";
        std::cout << "2. Створити процес пошуку і заміни\n";
        std::cout << "3. Призупинити процес\n";
        std::cout << "4. Відновити процес\n";
        std::cout << "5. Змінити пріоритет процесу\n";
        std::cout << "6. Завершити процес\n";
        std::cout << "7. Встановити прив'язку до ядра\n";
        std::cout << "8. Вивести інформацію про процеси\n";
        std::cout << "9. Вийти\n";
        std::cout << "Виберіть дію: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                double start, end, step;
                std::cout << "Введіть початок проміжку: ";
                std::cin >> start;
                std::cout << "Введіть кінець проміжку: ";
                std::cin >> end;
                std::cout << "Введіть крок: ";
                std::cin >> step;

                pid_t pid = fork();
                if (pid == 0) {
                    tabulateFunction(start, end, step);
                    exit(0);
                } else {
                    processes.push_back({pid, "Tabulation Process", 0, 0});
                }
                break;
            }
            case 2: {
                std::string filename, word, replacement;
                std::cout << "Введіть ім'я файлу: ";
                std::cin >> filename;
                std::cout << "Введіть слово для заміни: ";
                std::cin >> word;
                std::cout << "Введіть нове слово: ";
                std::cin >> replacement;
pid_t pid = fork();
                if (pid == 0) {
                    searchAndReplace(filename, word, replacement);
                    exit(0);
                } else {
                    processes.push_back({pid, "Search and Replace", 0, 0});
                }
                break;
            }
            case 3: {
                pid_t pid;
                std::cout << "Введіть PID процесу для призупинення: ";
                std::cin >> pid;
                kill(pid, SIGSTOP);
                for (auto& proc : processes) {
                    if (proc.pid == pid) {
                        proc.state = 1; // Змінюємо стан на "зупинено"
                    }
                }
                break;
            }
            case 4: {
                pid_t pid;
                std::cout << "Введіть PID процесу для відновлення: ";
                std::cin >> pid;
                kill(pid, SIGCONT);
                for (auto& proc : processes) {
                    if (proc.pid == pid) {
                        proc.state = 0; // Змінюємо стан на "запущено"
                    }
                }
                break;
            }
            case 5: {
                pid_t pid;
                int newPriority;
                std::cout << "Введіть PID процесу для зміни пріоритету: ";
                std::cin >> pid;
                std::cout << "Введіть новий пріоритет: ";
                std::cin >> newPriority;
                setpriority(PRIO_PROCESS, pid, newPriority);
                for (auto& proc : processes) {
                    if (proc.pid == pid) {
                        proc.priority = newPriority;
                    }
                }
                break;
            }
            case 6: {
                pid_t pid;
                std::cout << "Введіть PID процесу для завершення: ";
                std::cin >> pid;
                kill(pid, SIGTERM);
                processes.erase(std::remove_if(processes.begin(), processes.end(),
                    [pid](const ProcessInfo& proc) { return proc.pid == pid; }), processes.end());
                break;
            }
           case 7: {
    pid_t pid;
    std::cout << "Введіть PID процесу для встановлення прив'язки до ядер: ";
    std::cin >> pid;

    std::cout << "Введіть номери ядер через пробіл (наприклад, 0 1 2), а потім натисніть Enter: ";
    std::vector<int> cores;
    int core;
    while (std::cin >> core) {
        cores.push_back(core);
        if (std::cin.peek() == '\n') break; // Завершуємо введення на Enter
    }
    setAffinity(pid, cores);
    break;
}

            case 8: {
                printProcessInfo(processes);
                break;
            }
            case 9:
                exit(0);
            default:
                std::cout << "Невірний вибір. Спробуйте ще раз.\n";
        }
    }

    return 0;
}

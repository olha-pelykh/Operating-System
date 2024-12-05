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
#include <ctime>
#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

void createNewFile(const std::string &fileName);
void setReadPermission(const std::string &fileName);
void setWritePermission(const std::string &fileName);
void setOwnership(const std::string &fileName, const std::string &owner, const std::string &group);

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
    }
}

bool checkWritePermission(const std::string &fileName) {
    struct stat fileStat;

    // Отримуємо атрибути файла
    if (stat(fileName.c_str(), &fileStat) == -1) {
        std::cerr << "Failed to retrieve file attributes: " << strerror(errno) << "\n";
        return false;
    }

    // Перевіряємо, чи файл доступний для запису (для власника)
    if (!(fileStat.st_mode & S_IWUSR)) {
        std::cerr << "File is not writable by the owner.\n";
        return false;
    }

    return true;
}

// Функція для табуляції і запису результатів у файл
void tabulateAndWriteToFile(const std::string &fileName, double start, double end, double step) {
    std::ofstream outFile(fileName, std::ios::app); // Відкриваємо файл для допису
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << strerror(errno) << "\n";
        return;
    }

    outFile << "Tabulation Results:\n";
    for (double x = start; x <= end; x += step) {
        double result = x / (tan(x) + exp(x)); // Обчислення функції
        outFile << "x = " << x << ", f(x) = " << result << "\n";
    }
    outFile << "End of Tabulation\n";

    std::cout << "Tabulation results written to " << fileName << "\n";
    outFile.close();
}


// Пошук та заміна слова у файлі
void searchAndReplace(const std::string& filename, const std::string& word, const std::string& replacement) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return;
    }

    std::string tempFile = "/home/vboxuser/Documents/OS_Lab8/OS_Lab8_1/OS_Lab8_LogInfo.txt";
    std::ofstream outFile(tempFile);
    if (!outFile.is_open()) {
        std::cerr << "Failed to create temporary file.\n";
        file.close();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find(word);
        while (pos != std::string::npos) {
            line.replace(pos, word.length(), replacement);
            pos = line.find(word, pos + replacement.length());
        }
        outFile << line << "\n";
    }

    file.close();
    outFile.close();
    if (remove(filename.c_str()) == 0) {
        rename(tempFile.c_str(), filename.c_str());
    } else {
        std::cerr << "Failed to delete the original file.\n";
    }
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

// Функції роботи з файлами
void createNewFile(const std::string &fileName) {
    std::ofstream file(fileName);
    if (file) {
        std::cout << "File created successfully: " << fileName << "\n";
    } else {
        std::cerr << "Failed to create file: " << strerror(errno) << "\n";
    }
}

void setReadPermission(const std::string &fileName) {
    if (chmod(fileName.c_str(), S_IRUSR | S_IRGRP | S_IROTH) == 0) {
        std::cout << "Read-only permissions set successfully.\n";
    } else {
        std::cerr << "Failed to set read-only permissions: " << strerror(errno) << "\n";
    }
}

void setWritePermission(const std::string &fileName) {
    struct stat fileStat;

    // Retrieve the current permissions
    if (stat(fileName.c_str(), &fileStat) == -1) {
        std::cerr << "Failed to retrieve file attributes: " << strerror(errno) << "\n";
        return;
    }

    // Add write permission for the owner while preserving other permissions
    mode_t newMode = fileStat.st_mode | S_IWUSR;

    if (chmod(fileName.c_str(), newMode) == 0) {
        std::cout << "Write permission set for the owner successfully.\n";
    } else {
        std::cerr << "Failed to set write permission: " << strerror(errno) << "\n";
    }
}

void setAdministratorPermission(const std::string &fileName) {
    if (chmod(fileName.c_str(), S_IWUSR | S_IWGRP | S_IWOTH) == 0) {
        std::cout << "Administrator permissions set successfully.\n";
    } else {
        std::cerr << "Failed to set administrator permissions: " << strerror(errno) << "\n";
    }
}

void setOwnership(const std::string &fileName, const std::string &owner, const std::string &group) {
    struct passwd *pwd = getpwnam(owner.c_str());
    struct group *grp = getgrnam(group.c_str());

    if (!pwd) {
        std::cerr << "Invalid owner: " << owner << "\n";
        return;
    }

    if (!grp) {
        std::cerr << "Invalid group: " << group << "\n";
        return;
    }

    if (chown(fileName.c_str(), pwd->pw_uid, grp->gr_gid) == 0) {
        std::cout << "Ownership changed successfully to " << owner << ":" << group << "\n";
    } else {
        std::cerr << "Failed to change ownership: " << strerror(errno) << "\n";
    }
}
void logProcessEvent(const std::string& filename, const std::string& action, pid_t pid) {
    // Check if the file exists and is writable
    struct stat fileStat;
    if (stat(filename.c_str(), &fileStat) != 0) {
        std::cerr << "File does not exist or cannot be accessed: " << strerror(errno) << "\n";
        return;
    }
    if (!(fileStat.st_mode & S_IWUSR)) {
        std::cerr << "File is not writable.\n";
        return;
    }

    // Open the file in append mode
    int fileDescriptor = open(filename.c_str(), O_WRONLY | O_APPEND);
    if (fileDescriptor == -1) {
        std::cerr << "Failed to open file: " << strerror(errno) << "\n";
        return;
    }

    // Prepare the log entry with a timestamp
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char timestamp[20];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localTime);

    std::string logEntry = "PID: " + std::to_string(pid) + "\tAction: " + action + "\tTime: " + timestamp + "\n";

    // Write the log entry to the file
    ssize_t bytesWritten = write(fileDescriptor, logEntry.c_str(), logEntry.size());
    if (bytesWritten == -1) {
        std::cerr << "Failed to write to file: " << strerror(errno) << "\n";
    } //else {
      //std::cout << "Log entry written successfully: " << logEntry;
    //}

    // Close the file
    close(fileDescriptor);
}



int main() {
    std::vector<ProcessInfo> processes;
    std::string logFile = "/home/vboxuser/Documents/OS_Lab8/OS_Lab8_1/OS_Lab4_LogInfo.txt";
    std::string tabFile = "/home/vboxuser/Documents/OS_Lab8/OS_Lab8_1/OS_Lab4_TabInfo.txt";
    createNewFile(tabFile);
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
        std::cout << "9. Змінити налаштування файлу\n";
        std::cout << "10. Вийти\n";
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
                logProcessEvent(logFile, "Process started", getpid());
                pid_t pid = fork();
                if (pid == 0) {
                    tabulateAndWriteToFile(tabFile, start, end, step);
                    //tabulateFunction(start, end, step);
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
                logProcessEvent(logFile, "Process started", getpid());
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
                logProcessEvent(logFile, "Process stopped", getpid());
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
                logProcessEvent(logFile, "Process resumed", pid);
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
                logProcessEvent(logFile, "Process priority changed", pid);
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
                logProcessEvent(logFile, "Process terminated", pid);
                kill(pid, SIGTERM);
                processes.erase(std::remove_if(processes.begin(), processes.end(),
                    [pid](const ProcessInfo& proc) { return proc.pid == pid; }), processes.end());
                break;
            }
           case 7: {
              pid_t pid;
              std::cout << "Введіть PID процесу для встановлення прив'язки до ядер: ";
              std::cin >> pid;
              logProcessEvent(logFile, "Process affinity changed", pid);
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
            logProcessEvent(logFile, "Process info showed", 0);
                printProcessInfo(processes);
                break;
            }
            case 9: {
          std::string fileName = "/home/vboxuser/Documents/OS_Lab8/OS_Lab8_1/OS_Lab4_LogInfo.txt";

          int option;
          std::cout << "\nFile Management Options:\n";
          std::cout << "1. Create a new file\n";
          std::cout << "2. Set read-only permissions\n";
          std::cout << "3. Set write permissions\n";
          std::cout << "4. Set administrator permissions\n";
          std::cout << "5. Change file ownership (user:group)\n";
          std::cout << "Enter your choice: ";
          std::cin >> option;

          switch (option) {
              case 1: { // Create a new file
                  createNewFile(fileName);
                  break;
              }
              case 2: { // Set read-only permissions
                  setReadPermission(fileName);
                  break;
              }
              case 3: { // Set write permissions
                  setWritePermission(fileName);
                  break;
              }
              case 4: { // Set write permissions
                  setAdministratorPermission(fileName);
                  break;
              }
              case 5: { // Change ownership
                  std::string owner, group;
                  std::cout << "Enter new owner: ";
                  std::cin >> owner;
                  std::cout << "Enter new group: ";
                  std::cin >> group;
                  setOwnership(fileName, owner, group);
                  break;
              }
        default: {
            std::cerr << "Invalid option. Please select a valid operation.\n";
            break;
        }
    }
    break;
}
            case 10:
                exit(0);
            default:
                std::cout << "Невірний вибір. Спробуйте ще раз.\n";
        }
    }

    return 0;
}

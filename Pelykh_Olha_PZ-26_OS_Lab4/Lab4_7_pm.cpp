#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: ./process_manager ./tab <start> <end> <step> <N>\n";
        return 1;
    }

    try {
        // Corrected indices for reading the arguments
        double a = std::stod(argv[2]);  // Start
        double b = std::stod(argv[3]);  // End
        double step = std::stod(argv[4]);  // Step
        int N = std::stoi(argv[5]);  // Number of processes

        double variety = b - a;
        double range = variety / N;

        std::vector<pid_t> pids;

        auto start_time = std::chrono::steady_clock::now();

        for (int i = 0; i < N; ++i) {
            double startTab = a + i * range;
            double endTab = a + (i + 1) * range;

            pid_t pid = fork();

            if (pid == 0) {  // Child process
                std::stringstream ss;
                ss << argv[1] << " " << startTab << " " << endTab << " " << step;  // argv[1] is the path to the tabulation program
                std::string command = ss.str();
                std::system(command.c_str());  // Execute the tabulation program
                return 0;
            }
            else if (pid > 0) {  // Parent process
                pids.push_back(pid);
            }
            else {
                std::cerr << "Fork failed\n";
                return 1;
            }
        }

        // Wait for all child processes to complete
        for (pid_t pid : pids) {
            waitpid(pid, nullptr, 0);
        }

        auto stop_time = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> elapsed_time = stop_time - start_time;

        std::cout << "\nExecution time for " << N << " process(-es): " << elapsed_time.count() << " milliseconds" << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Argument out of range: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

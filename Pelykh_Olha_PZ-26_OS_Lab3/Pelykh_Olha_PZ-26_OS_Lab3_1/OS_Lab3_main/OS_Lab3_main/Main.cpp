#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <string>
#include <sstream>
#include <Windows.h>

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "To use this \"exe\" file you need to enter in such way : ProcessesTab.exe Tab.exe <startTab> <endTab> <stepForTab> <N>\n";
        return 1;
    }

    double a = std::stod(argv[1]);
    double b = std::stod(argv[2]);
    double step = std::stod(argv[3]);
    int N = atoi(argv[4]);

    double variety = b - a;
    double range = variety / N;

    std::vector<HANDLE> process_handles;


    auto start_time = std::chrono::steady_clock::now();

    for (int i = 0; i < N; i++) {
        double startTab = a + i * range;
        double endTab = a + (i + 1) * range;

        std::wstring commandLine = L"\"C:\\Users\\User\\Music\\SEM2\\C++\\OS_Lab3_Tab\\x64\\Debug\\OS_Lab3_Tab.exe\" "
            + std::to_wstring(startTab) + L" " + std::to_wstring(endTab) + L" " + std::to_wstring(step);


        STARTUPINFO si = { sizeof(STARTUPINFO) };
        PROCESS_INFORMATION pi;

        if (CreateProcess(NULL, const_cast<LPWSTR>(commandLine.c_str()), NULL, NULL, TRUE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {

            printf("pid=%d, tid=%d\n", pi.dwProcessId, pi.dwThreadId);
            process_handles.push_back(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        else {
            std::cerr << "Failed to create process. Error code: " << GetLastError() << std::endl;
        }
    }

    for (HANDLE handle : process_handles) {
        WaitForSingleObject(handle, INFINITE);
        CloseHandle(handle);
    }

    auto stop_time = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed_time = stop_time - start_time;

    std::cout << "\nExecution time for " << N << " process(-es): " << elapsed_time.count() << " milliseconds" << std::endl;
    return 0;
}



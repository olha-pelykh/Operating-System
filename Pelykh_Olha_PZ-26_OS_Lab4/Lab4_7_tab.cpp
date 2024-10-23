#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>

double TabulateFunction(double x) {
    return 3 + sin(cos(pow(x, 2)) + sin(pow(x, 3)));
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: ./tab <start> <end> <step>\n";
        return 1;
    }

    auto begin = std::chrono::high_resolution_clock::now();

    double start = std::stod(argv[1]);
    double end = std::stod(argv[2]);
    double step = std::stod(argv[3]);

    for (double x = start; x < end; x += step) {
        std::cout << "f(" << x << ") = " << TabulateFunction(x) << std::endl;
    }

    auto ending = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(ending - begin);
    std::cout << "Time needed: " << elapsed.count() << " milliseconds" << std::endl;

    return 0;
}

#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>
#include <string>

double TabulateFunction(double x) {
	return x / (tan(x) + exp(x));
}

int main(int argc, char* argv[]) {
	if (argc < 4) {
		std::cerr << "To use this \"exe\" file you need to enter in such way : Tab.exe <starTab> <endTab> <stepForTab>\n";
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
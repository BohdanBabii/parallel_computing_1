#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>

int main() {
    std::ofstream outfile("memory_bandwidth.csv");

    if (!outfile.is_open()) {
        std::cerr << "Error: Could not create file!" << std::endl;
        return 1;
    }
    outfile << "Array Size (bytes),Bandwidth (GB/s)" << std::endl;

    const int ITERATIONS = 10000;
    double l_scalar = 2.0;

    for (int size = 2; size <= 1024000000; size *= 2) {
        double* l_A = new double[size / sizeof(double)];
        double* l_B = new double[size / sizeof(double)];
        double* l_C = new double[size / sizeof(double)];

        for (int i = 0; i < size / sizeof(double); ++i) {
            l_A[i] = static_cast<double>(std::rand()) / RAND_MAX;
            l_B[i] = static_cast<double>(std::rand()) / RAND_MAX;
            l_C[i] = 0.0;
        }

        auto l_start_time = std::chrono::high_resolution_clock::now();

        for (int iter = 0; iter < ITERATIONS; ++iter) {
            for (int i = 0; i < size / sizeof(double); ++i) {
                l_C[i] = l_A[i] + l_scalar * l_B[i];
            }
        }

        auto l_end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> l_duration = l_end_time - l_start_time;

        double l_data_access_speed = 3.0 * size * ITERATIONS / l_duration.count() / (1024 * 1024 * 1024);

        outfile << size << "," << l_data_access_speed << std::endl;

        delete[] l_A;
        delete[] l_B;
        delete[] l_C;
    }

    outfile.close();

    return 0;
}

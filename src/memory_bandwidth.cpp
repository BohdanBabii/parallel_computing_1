#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#define NUM_SIZES 30
#define NUM_ITERATIONS 10000

const int array_sizes[NUM_SIZES] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824 };

int main() {
    std::ofstream result_file("memory_bandwidth.csv");
    result_file << "Array Size (bytes),Bandwidth (GB/s)" << std::endl;

    for (int i = 0; i < NUM_SIZES; ++i) {
        int array_size = array_sizes[i];
        std::vector<double> a(array_size / sizeof(double), 1.0);
        std::vector<double> b(array_size / sizeof(double), 2.0);
        std::vector<double> c(array_size / sizeof(double), 0.0);

        // Measure time taken for NUM_ITERATIONS of STREAM Triad
        clock_t start_time = clock();
        for (int j = 0; j < NUM_ITERATIONS; ++j) {
            for (int k = 0; k < array_size / sizeof(double); ++k) {
                c[k] = a[k] + 2.0 * b[k];
            }
        }
        clock_t end_time = clock();

        double elapsed_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
        double bandwidth = (2.0 * sizeof(double) * array_size * NUM_ITERATIONS) / (elapsed_time * 1e9);  // Convert to GB/s

        result_file << array_size << "," << bandwidth << std::endl;
    }

    result_file.close();
    return 0;
}

#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <string>

void impl_blocked_k(double *i_A,
                    double *i_b,
                    double *io_c,
                    std::size_t i_k,
                    std::size_t i_m,
                    std::size_t i_blockSize,
                    std::size_t iterations)
{

    for (int i = 0; i < iterations; i++)
    {
        std::size_t l_nBlocks = i_k / i_blockSize;
        for (std::size_t l_bk = 0; l_bk < l_nBlocks; l_bk++)
            for (std::size_t l_m = 0; l_m < i_m; l_m++)
                for (std::size_t l_k = l_bk * i_blockSize; l_k < (l_bk + 1) * i_blockSize; l_k++)
                    io_c[l_m] += i_A[l_m * i_k + l_k] * i_b[l_k];
    }
}

int main()
{
    std::ofstream outfile("matrix_matrix_blocking.csv");

    if (!outfile.is_open())
    {
        std::cerr << "Error: Could not create file!" << std::endl;
        return 1;
    }
    outfile << "Implementation,GFLOPS\n"
            << std::endl;

    std::size_t iterations = 1000;

    for (int size = 32; size <= 8192; size *= 2)
    {
        double *i_A = new double[size * size];
        double *i_B = new double[size * size];
        double *io_C = new double[size * size];

        for (int i = 0; i < size * size; ++i)
        {
            i_A[i] = static_cast<double>(std::rand()) / RAND_MAX;
            i_B[i] = static_cast<double>(std::rand()) / RAND_MAX;
            io_C[i] = 0.0;
        }

        auto l_start_time = std::chrono::high_resolution_clock::now();

        impl_blocked_k(
            i_A,
            i_B,
            io_C,
            size,
            size,
            4,
            iterations);

        auto l_end_time = std::chrono::high_resolution_clock::now();
        double elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(l_end_time - l_start_time).count() / 1e6;

        // Calculate GFLOPS
        double totalFlops = 2.0 * size * size * 4 * iterations;        // Two flops per element (multiply and add)
        double gflops = totalFlops / elapsedTime / 1e9;              // Convert to GFLOPS

        outfile << size << "," << gflops << std::endl;

        delete[] i_A;
        delete[] i_B;
        delete[] io_C;
    }
}
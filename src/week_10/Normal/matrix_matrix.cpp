#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <string>

void matmul_mnk(double *i_A,
                double *i_B,
                double *io_C,
                std::size_t i_m,
                std::size_t i_n,
                std::size_t i_k,
                std::size_t i_count,
                std::size_t iterations)
{
    for (int i = 0; i < iterations; i++)
    {

        switch (i_count)
        {
        case 0:
            // M=N=K
            for (std::size_t l_m = 0; l_m < i_m; l_m++)
                for (std::size_t l_n = 0; l_n < i_n; l_n++)
                    for (std::size_t l_k = 0; l_k < i_k; l_k++)
                        io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
            break;
        case 1:
            // M=K=N
            for (std::size_t l_m = 0; l_m < i_m; l_m++)
                for (std::size_t l_k = 0; l_k < i_k; l_k++)
                    for (std::size_t l_n = 0; l_n < i_n; l_n++)
                        io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
            break;
        case 2:
            // K=M=N
            for (std::size_t l_k = 0; l_k < i_k; l_k++)
                for (std::size_t l_m = 0; l_m < i_m; l_m++)
                    for (std::size_t l_n = 0; l_n < i_n; l_n++)
                        io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
            break;
        case 3:
            // K=N=M
            for (std::size_t l_k = 0; l_k < i_k; l_k++)
                for (std::size_t l_n = 0; l_n < i_n; l_n++)
                    for (std::size_t l_m = 0; l_m < i_m; l_m++)
                        io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
            break;
        case 4:
            // N=K=M
            for (std::size_t l_n = 0; l_n < i_n; l_n++)
                for (std::size_t l_k = 0; l_k < i_k; l_k++)
                    for (std::size_t l_m = 0; l_m < i_m; l_m++)
                        io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
            break;
        default:
            // N=M=K
            for (std::size_t l_n = 0; l_n < i_n; l_n++)
                for (std::size_t l_m = 0; l_m < i_m; l_m++)
                    for (std::size_t l_k = 0; l_k < i_k; l_k++)
                        io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
        }
    }
}

int main()
{
    std::ofstream outfile("performance.csv");

    if (!outfile.is_open())
    {
        std::cerr << "Error: Could not create file!" << std::endl;
        return 1;
    }
    outfile << "Implementation,M=N=K,GFLOPS\n"
            << std::endl;

    std::size_t iterations = 50;
    std::string variation[6] = {"M=N=K", "M=K=N", "K=M=N", "K=N=M", "N=K=M", "N=M=K"};

    for (int size = 2; size <= 1024; size *= 2)
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

        for (int j = 0; j < 6; j++)
        {
            auto l_start_time = std::chrono::high_resolution_clock::now();

            matmul_mnk(
                i_A,
                i_B,
                io_C,
                size,
                size,
                size,
                j,
                iterations);

            auto l_end_time = std::chrono::high_resolution_clock::now();
            double elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(l_end_time - l_start_time).count() / 1e6;

            // Calculate GFLOPS
            double totalFlops = 2.0 * size * size * size * iterations;   // Two flops per element (multiply and add) - 3 size for M, N, K
            double gflops = totalFlops / elapsedTime / 1e9; // Convert to GFLOPS

            outfile << size << "," << variation[j] << "," << gflops << std::endl;
        }

        delete[] i_A;
        delete[] i_B;
        delete[] io_C;
    }
}
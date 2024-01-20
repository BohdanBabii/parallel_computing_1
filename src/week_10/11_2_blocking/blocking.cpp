#define _USE_MATH_DEFINES

#include <mpi.h>

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <fstream>


void impl_blocked_k(double* i_A,
                    double* i_b,
                    double* io_c,
                    std::size_t i_k,
                    std::size_t i_m,
                    std::size_t i_blockSize){

    std::size_t l_nBlocks = i_k / i_blockSize;
    for( std::size_t l_bk = 0; l_bk < l_nBlocks; l_bk++ )
        for( std::size_t l_m = 0; l_m < i_m; l_m++ )
        for( std::size_t l_k = l_bk*i_blockSize; l_k < (l_bk+1)*i_blockSize; l_k++)
            io_c[l_m] += i_A[l_m*i_k + l_k] * i_b[l_k];
}
void impl_not_blocked(  double* i_A,
                        double* i_b,
                        double* io_c,
                        std::size_t i_k,
                        std::size_t i_m){

    for( std::size_t l_m = 0; l_m < i_m; l_m++ )
      for( std::size_t l_k = 0; l_k < i_k; l_k++)
        io_c[l_m] += i_A[l_m*i_k + l_k] * i_b[l_k];
}

void getTime(double* l_A, double* l_B, double* l_C, int size, int type, std::ofstream& outfile, int i_blockSize) {
    std::string sz;

    auto l_start_time = std::chrono::high_resolution_clock::now();
    switch (type) {
        case 1:
            impl_blocked_k(l_A, l_B, l_C, size, size, i_blockSize);
            sz = "blocked";
            break;
        case 2:
            impl_not_blocked(l_A, l_B, l_C, size, size);
            sz = "free";
            break;
        default:
            break;
    }

    auto l_end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> l_duration = l_end_time - l_start_time;
    double gflops = 2.0 * size * size * size / (1.0e9 * l_duration.count()); 

    outfile << sz << "," << size << "," << gflops << std::endl;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int l_comm_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &l_comm_rank);

    std::ofstream outfile("performance.csv");

    if (!outfile.is_open()) {
        std::cerr << "Error: Could not create file!" << std::endl;
        return 1;
    }

    outfile << "Implementation,"
            << "M=K,"
            << "GFLOPS" << std::endl;

    for (int size = 32; size <= 8192; size *= 2) {
        double* l_A = new double[size * size];
        double* l_B = new double[size];
        double* l_C = new double[size];

        for (int i = 0; i < size * size; ++i) {
            l_A[i] = static_cast<double>(std::rand()) / RAND_MAX;
        }
        for (int i = 0; i < size; ++i) {
            l_B[i] = static_cast<double>(std::rand()) / RAND_MAX;
            l_C[i] = 0.0;
        }
        getTime(l_A, l_B, l_C, size, 1, outfile, 4);
        getTime(l_A, l_B, l_C, size, 2, outfile, 4);
        
        delete[] l_A;
        delete[] l_B;
        delete[] l_C;
    }
    outfile.close();
    MPI_Finalize();
    return 0;
}

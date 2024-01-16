#define _USE_MATH_DEFINES

#include <mpi.h>

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>

void matmul_mnk(double* i_A,
                double* i_B,
                double* io_C,
                std::size_t i_m,
                std::size_t i_n,
                std::size_t i_k) {
    for (std::size_t l_m = 0; l_m < i_m; l_m++)
        for (std::size_t l_n = 0; l_n < i_n; l_n++)
            for (std::size_t l_k = 0; l_k < i_k; l_k++)
                io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
}

void matmul_mkn(double* i_A,
                double* i_B,
                double* io_C,
                std::size_t i_m,
                std::size_t i_n,
                std::size_t i_k) {
    for (std::size_t l_m = 0; l_m < i_m; l_m++)
        for (std::size_t l_k = 0; l_k < i_k; l_k++)
            for (std::size_t l_n = 0; l_n < i_n; l_n++)
                io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
}

void matmul_nkm(double* i_A,
                double* i_B,
                double* io_C,
                std::size_t i_m,
                std::size_t i_n,
                std::size_t i_k) {
    for (std::size_t l_n = 0; l_n < i_n; l_n++)
        for (std::size_t l_k = 0; l_k < i_k; l_k++)
            for (std::size_t l_m = 0; l_m < i_m; l_m++)
                io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
}

void matmul_nmk(double* i_A,
                double* i_B,
                double* io_C,
                std::size_t i_m,
                std::size_t i_n,
                std::size_t i_k) {
    for (std::size_t l_n = 0; l_n < i_n; l_n++)
        for (std::size_t l_m = 0; l_m < i_m; l_m++)
            for (std::size_t l_k = 0; l_k < i_k; l_k++)
                io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
}

void matmul_knm(double* i_A,
                double* i_B,
                double* io_C,
                std::size_t i_m,
                std::size_t i_n,
                std::size_t i_k) {
    for (std::size_t l_k = 0; l_k < i_k; l_k++)
        for (std::size_t l_n = 0; l_n < i_n; l_n++)
            for (std::size_t l_m = 0; l_m < i_m; l_m++)
                io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
}

void matmul_kmn(double* i_A,
                double* i_B,
                double* io_C,
                std::size_t i_m,
                std::size_t i_n,
                std::size_t i_k) {
    for (std::size_t l_k = 0; l_k < i_k; l_k++)
        for (std::size_t l_m = 0; l_m < i_m; l_m++)
            for (std::size_t l_n = 0; l_n < i_n; l_n++)
                io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
}
void getTime(double* i_A, double* i_B, int type) {
    String::size_type sz;
    const int ITERATIONS = 1000;

    auto l_start_time = std::chrono::high_resolution_clock::now();

    case 1:
        matmul_mnk(l_A, l_B, l_C, size, size, size);
        size_type sz = "MNK";
        break;
    case 2:
        matmul_mkn(l_A, l_B, l_C, size, size, size);
        size_type sz = "MKN";
        break;
    case 3:
        matmul_nkm(l_A, l_B, l_C, size, size, size);
        size_type sz = "NKM";
        break;
    case 4:
        matmul_nmk(l_A, l_B, l_C, size, size, size);
        size_type sz = "NMK";
        break;
    case 5:
        matmul_knm(l_A, l_B, l_C, size, size, size);
        size_type sz = "KNM";
        break;
    case 6:
        matmul_kmn(l_A, l_B, l_C, size, size, size);
        size_type sz = "KMN";
        break;

        std::chrono::duration<double> l_duration = l_end_time - l_start_time;

        double l_data_access_speed = 3.0 * size * ITERATIONS / l_duration.count() / (1024 * 1024 * 1024);

        outfile << sz << size << l_data_access_speed << std::endl;

        delete[] l_A;
        delete[] l_B;
        delete[] l_C;
}
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int l_comm_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &l_comm_rank);

    std::cout << 'Implementation' << 'M=N=K' << 'GFLOPS' << std::endl;

    for (int size = 2; size <= 1024; size *= 2) {
        double* l_A = new double[size * size];
        double* l_B = new double[size * size];
        double* l_C = new double[size * size];

        for (int i = 0; i < size / sizeof(double); ++i) {
            l_A[i] = static_cast<double>(std::rand()) / RAND_MAX;
            l_B[i] = static_cast<double>(std::rand()) / RAND_MAX;
            l_C[i] = 0.0;
        }
    }

    MPI_Finalize();
    return 0;
}
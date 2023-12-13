#define _USE_MATH_DEFINES

#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <random>

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int l_comm_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &l_comm_rank);

  int l_comm_size;
  MPI_Comm_size(MPI_COMM_WORLD, &l_comm_size);

  // Initialize this once
  std::random_device rd;
  std::mt19937_64 gen(rd());

  // Definition of the number generator
  std::uniform_real_distribution<> dis(0.0, 1.0);

  int elements = 50000000;
  int iterations_per_process = elements / l_comm_size;
  double x_coord = 0.0;
  double y_coord = 0.0;
  int l_c = 0;

  double start_time;

  // Start Time Measurement
  if ( l_comm_rank == 0 )
  {
    start_time = MPI_Wtime();
  }
  
  for (int n=0; n < iterations_per_process; n++)
  {
    x_coord = dis(gen);
    y_coord = dis(gen);
    if ( pow(x_coord, 2) + pow(y_coord, 2) < 1 )
    {
        l_c++;
    }
  }

  if( l_comm_rank == 0 ) {
    int received_data;

    for ( std::size_t l_da = 1; l_da < l_comm_size; l_da++ )
    {
        MPI_Recv(
            &received_data,     // Pointer to Data
            1,                  // Count of Data Items
            MPI_INT,            // Data Type
            l_da,               // Sender's Rank
            0,                  // Message Tag
            MPI_COMM_WORLD,     // Communicator
            MPI_STATUS_IGNORE   // Status Variable
        );
        l_c += received_data;
    }

    double final_result = (static_cast<double>(l_c) * 4) / elements;
    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;

    std::cout << "Elapsed time: " << elapsed_time << " seconds" << std::endl;
    std::cout << "The error is: " << M_PI - final_result << "." << std::endl;
  }
  else {
    MPI_Send(
        &l_c,          // Pointer to Data
        1,             // Count of Data Items
        MPI_INT,       // Data Type
        0,             // Receiver's Rank
        0,             // Message Tag
        MPI_COMM_WORLD // Communicator
    );
  }

  MPI_Finalize();
  return 0;
}
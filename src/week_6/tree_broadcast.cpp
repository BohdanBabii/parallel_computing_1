#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

void tree_broadcast(int size, int rank, int comm_size) {
  int root = 0;

  double* data_array = new double[size];

  if (rank == root) {
    std::srand(std::time(0));
    for (int i = 0; i < size; ++i) {
      data_array[i] = static_cast<double>(std::rand()) / RAND_MAX;
    }
  }

  for (int span = 1; span < comm_size; span *= 2) {
    int partner = rank ^ span;  

    if (partner < comm_size) {
      if (rank < partner) {
        MPI_Send(data_array, size, MPI_DOUBLE, partner, 0, MPI_COMM_WORLD);
      } else {
        MPI_Recv(data_array, size, MPI_DOUBLE, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);
  }

  std::cout << "Rank " << rank << " received the array:" << std::endl;
  for (int i = 0; i < size; ++i) {
    std::cout << "Element " << i << ": " << data_array[i] << std::endl;
  }
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int rank, comm_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  int n = 10; 
  int array_size = (argc > 1) ? std::atoi(argv[1]) : n; 

  double start_time, end_time;


  MPI_Barrier(MPI_COMM_WORLD);
  start_time = MPI_Wtime();
  tree_broadcast(array_size, rank, comm_size);
  end_time = MPI_Wtime();
  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) {
    std::cout << "Custom tree_broadcast time: " << end_time - start_time << " seconds." << std::endl;
  }

  MPI_Barrier(MPI_COMM_WORLD);


  double* mpi_data_array = new double[array_size];
  MPI_Barrier(MPI_COMM_WORLD);
  start_time = MPI_Wtime();
  MPI_Bcast(mpi_data_array, array_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  end_time = MPI_Wtime();
  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) {
    std::cout << "MPI_Bcast time: " << end_time - start_time << " seconds." << std::endl;
  }

  MPI_Finalize();

  return 0;
}

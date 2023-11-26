#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

void my_custom_reduction() {
  int l_rank;
  int l_comm_size;

  MPI_Comm_rank(MPI_COMM_WORLD, &l_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &l_comm_size);

  MPI_Request send_request, recv_request;
  MPI_Status send_status, recv_status;

  std::srand(std::time(0) + l_rank);

  if ( l_rank == 0 )
  {
    double data_array[l_comm_size - 1];
    int message_tag = 777;

    // puts the element at the sender ranks position - 1
    for ( int i = 1; i < l_comm_size; i++)
    {
      int receiver_rank = i;
      MPI_Irecv(&data_array[i - 1], 1, MPI_DOUBLE, receiver_rank, message_tag, MPI_COMM_WORLD, &recv_request);

      int flag = 0;
      while(flag == 0) 
      {
        MPI_Test(&recv_request, &flag, &recv_status);
      }
    }

    double sum_data = 0.0;

    // printing out every value received and the summed up value
    std::cout << "Rank " << l_rank << " has received the array:" << std::endl;
    for ( int i = 0; i < l_comm_size - 1; i++)
    {
      sum_data += data_array[i];
      std::cout << "Element " << i << ": " << data_array[i] << std::endl;
    }
    std::cout << "The sum of the array is: " << sum_data << std::endl;
  }
  else 
  {
    // every rank except rank 0 sends a random value

    double send_data = static_cast<double>(std::rand()) / RAND_MAX;
    MPI_Isend(&send_data, 1, MPI_DOUBLE, 0, 777, MPI_COMM_WORLD, &send_request);

    int flag = 0;
    while(flag == 0) {
      MPI_Test(&send_request, &flag, &send_status);
    }

    // for debugging:
    // std::cout << "Rank " << l_rank << " has send the value: " << send_data << std::endl;
  }
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  my_custom_reduction();

  MPI_Finalize();

  return 0;
}
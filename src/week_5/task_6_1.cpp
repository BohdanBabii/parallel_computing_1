#include <mpi.h>
#include <iostream>
#include <cassert>

int main(int argc, char** argv) {
  int l_ret = MPI_Init(&argc, &argv);
  assert(l_ret == MPI_SUCCESS);

  int l_rank;
  int l_comm_size;

  l_ret = MPI_Comm_rank(MPI_COMM_WORLD, &l_rank);
  assert(l_ret == MPI_SUCCESS);

  l_ret = MPI_Comm_size(MPI_COMM_WORLD, &l_comm_size);
  assert(l_ret == MPI_SUCCESS);

  if ( l_comm_size < 2 )
  {
    std::cerr << "There were not enough processes." << std::endl;
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  if ( l_rank == 0 )
  {
    // send to next process
    int init_value = 42;
    int receiver_rank = l_rank + 1;
    int message_tag = 0;
    l_ret = MPI_Send(&init_value, 1, MPI_INT, receiver_rank, message_tag, MPI_COMM_WORLD);
    assert(l_ret == MPI_SUCCESS);

    std::cout << "Process with rank: " << l_rank << " has send the initial value: " << init_value << " to Process with rank: " << receiver_rank << std::endl;

    // receive from last process
    int received_value;
    int sender_rank = l_comm_size - 1;
    MPI_Status status;
    l_ret = MPI_Recv(&received_value, 1, MPI_INT, sender_rank, message_tag, MPI_COMM_WORLD, &status);
    assert(l_ret == MPI_SUCCESS);

    std::cout << "Process with rank: " << l_rank << " has received the value: " << received_value << " from Process with rank: " << sender_rank << std::endl;
  }
  else
  {
    // receive
    int received_value;
    int sender_rank = l_rank - 1;
    int message_tag = 0;
    MPI_Status status;
    l_ret = MPI_Recv(&received_value, 1, MPI_INT, sender_rank, message_tag, MPI_COMM_WORLD, &status);
    assert(l_ret == MPI_SUCCESS);

    std::cout << "Process with rank: " << l_rank << " has received the value: " << received_value << " from Process with rank: " << sender_rank << std::endl;

    if ( l_rank == l_comm_size - 1 )
    {
        // send process 0
        int receiver_rank = 0;
        l_ret = MPI_Send(&received_value, 1, MPI_INT, receiver_rank, message_tag, MPI_COMM_WORLD);
        assert(l_ret == MPI_SUCCESS);
        std::cout << "Process with rank: " << l_rank << " has send the initial value: " << received_value << " to Process with rank: " << receiver_rank << std::endl;
    }
    else
    {
        // sends back to rank 0 and appends its rank -> value + rank
        int receiver_rank = l_rank + 1;
        l_ret = MPI_Send(&received_value, 1, MPI_INT, receiver_rank, message_tag, MPI_COMM_WORLD);
        assert(l_ret == MPI_SUCCESS);
        std::cout << "Process with rank: " << l_rank << " has send the initial value: " << received_value << " to Process with rank: " << receiver_rank << std::endl;
    }
  }

  l_ret = MPI_Finalize();
  assert(l_ret == MPI_SUCCESS);

  return 0;
}

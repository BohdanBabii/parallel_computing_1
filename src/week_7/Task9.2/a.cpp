#include <mpi.h>
#include <iostream>
#include <cassert>

// PUT Data from Proc1 
int main(int argc, char** argv) {
  int l_ret;

  l_ret = MPI_Init(&argc, &argv);
  assert( l_ret == MPI_SUCCESS);

  int l_rank;
  int l_comm_size;

  l_ret = MPI_Comm_rank(MPI_COMM_WORLD, &l_rank);
  assert( l_ret == MPI_SUCCESS );
  l_ret = MPI_Comm_size(MPI_COMM_WORLD, &l_comm_size);
  assert( l_ret == MPI_SUCCESS );

  MPI_Win l_win;

  int l_data;

  if ( l_rank == 0 )
  {
    l_data = 100;
    std::cout << "rank " << l_rank << " with data: " << l_data << " before PUT." << std::endl;
  }
  else
  {
    l_data = 1;
    std::cout << "rank " << l_rank << " with data: " << l_data << " before PUT." << std::endl;
  }

  if( l_rank == 0 ) {
    MPI_Win_create( &l_data,         // base
                    sizeof(int),     // size
                    sizeof(int),     // disp_unit
                    MPI_INFO_NULL,   // info
                    MPI_COMM_WORLD,  // comm
                    &l_win );        // win
  }
  else {
    MPI_Win_create( NULL,           // base
                    0,              // size
                    sizeof(int),    // disp_unit
                    MPI_INFO_NULL,  // info
                    MPI_COMM_WORLD, // comm
                    &l_win );       // win
  }

  // communicate within fences
  MPI_Win_fence( MPI_MODE_NOPRECEDE, // assert
                 l_win );            // win

  if( l_rank == 1 ) {
    MPI_Put( &l_data,   // origin_addr
             1,         // origin_count
             MPI_INT,   // origin_datatype
             0,         // target_rank
             0,         // target_disp
             1,         // target_count
             MPI_INT,   // target_datatype
             l_win );   // win
  }

  MPI_Win_fence( 0,       // assert
                 l_win ); // win

  // print and shutdown
  if ( l_rank == 0 )
  {
    std::cout << "rank " << l_rank << " with data: " << l_data << " after PUT." << std::endl;
  }
  else
  {
    std::cout << "rank " << l_rank << " with data: " << l_data << " after PUT." << std::endl;
  }

  MPI_Win_free( &l_win );

  l_ret = MPI_Finalize();
  assert( l_ret == MPI_SUCCESS );

  return 0;
}
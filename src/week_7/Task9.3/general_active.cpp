#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <cassert>

int main(int argc, char** argv) {
  int l_ret;

  MPI_Init(&argc, &argv);

  int l_comm_rank;
  int l_comm_size;

  MPI_Comm_rank(MPI_COMM_WORLD, &l_comm_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &l_comm_size);

  // Check initialization
  std::cout << "Hi! I am rank " << l_comm_rank
            << " out of " << l_comm_size << " ranks."
            << std::endl;

  // Initialize data
  float l_buf[l_comm_size];
  float l_single_buf = 0.0;

  if ( l_comm_rank == 0 )
  {
    for( std::size_t l_da = 0; l_da < l_comm_size; l_da++ ) 
    {
        l_buf[l_da] = l_comm_rank;
    }
  }
  else
  {
    l_single_buf = l_comm_rank * 10.0;
  }

  // Create Win
  MPI_Win l_win;

  if( l_comm_rank == 0 ) 
  {
    MPI_Win_create( l_buf,                       // base
                    l_comm_size*sizeof(float), // size
                    sizeof(float),             // disp_unit
                    MPI_INFO_NULL,             // info
                    MPI_COMM_WORLD,            // comm
                    &l_win );                  // win
  }
  else 
  {
    MPI_Win_create( NULL,           // base
                    0,              // size
                    sizeof(float),  // disp_unit
                    MPI_INFO_NULL,  // info
                    MPI_COMM_WORLD, // comm
                    &l_win );       // win
  }

  // Print initial data on rank 0
  if( l_comm_rank == 0 ) {
    for( std::size_t l_da = 0; l_da < l_comm_size; l_da++ ) {
      std::cout << "BEFORE: rank " << l_comm_rank << ": "
                << l_da << " / " << std::fixed << std::setprecision(1) << l_buf[l_da] << std::endl;
    }
  }

  // Set up groups
  MPI_Group l_comm_group;
  MPI_Comm_group( MPI_COMM_WORLD,  // comm
                  &l_comm_group ); // group


  MPI_Group l_group_incl;
  int l_ranks_target[1] = {0};         // where the data should go
  int l_ranks_origin[l_comm_size - 1]; // where the data comes from

  for ( std::size_t l_da = 0; l_da < l_comm_size - 1; l_da++ )
  {
    l_ranks_origin[l_da] = l_da + 1;
  }

  if( l_comm_rank == 0 ) {
    MPI_Group_incl( l_comm_group,    // group
                    l_comm_size - 1, // comm_size - 1
                    l_ranks_origin,  // ranks - where data comes from
                    &l_group_incl ); // newgroup
  }
  else 
  {
    MPI_Group_incl( l_comm_group,    // group
                    1,               // 1
                    l_ranks_target,  // ranks - where data goes to
                    &l_group_incl ); // newgroup
  }

  // Communicate
  if( l_comm_rank == 0 ) {
    // Start of Communication
    std::cout << "rank " << l_comm_rank << ": "
              << "calling MPI_Win_post, MPI_Win_Wait"
              << std::endl;

    MPI_Win_post( l_group_incl, // group
                  0,            // assert
                  l_win );      // win

    MPI_Win_wait( l_win ); // win

    // End of Communication
    std::cout << "rank " << l_comm_rank << ": "
              << "finished MPI_Win_post, MPI_Win_Wait"
              << std::endl;
  }
  else 
  {
    // Start of Communication
    std::cout << "rank " << l_comm_rank << ": "
              << "calling MPI_Win_start, MPI_Put, MPI_Win_complete"
              << std::endl;

    MPI_Win_start( l_group_incl, // group
                   0,            // assert
                   l_win );      // win

    MPI_Aint l_target_disp = l_comm_rank; // places data at the number of their rank in l_buf
    MPI_Put( &l_single_buf,     // origin_addr - Data to be send
             1,               // origin_count - How many items to send
             MPI_FLOAT,       // origin_datatype
             0,               // target_rank - Where the data should go
             l_target_disp,   // target_disp - Where the data should be placed
             1,               // target_count
             MPI_FLOAT,       // target_datatype
             l_win );         // win

    MPI_Win_complete( l_win ); // win

    // End of Communication
    std::cout << "rank " << l_comm_rank << ": "
              << "finished MPI_Win_start, MPI_Put, MPI_Win_complete"
              << std::endl;
  }

  // Free data
  MPI_Win_free( &l_win );
  MPI_Group_free( &l_comm_group );
  MPI_Group_free( &l_group_incl );

  // Print data on rank 0 after communicating
  if( l_comm_rank == 0 ) {
    for( std::size_t l_da = 0; l_da < l_comm_size; l_da++ ) {
      std::cout << "AFTER: rank " << l_comm_rank << ": "
                << l_da << " / " << std::fixed << std::setprecision(1) << l_buf[l_da] << std::endl;
    }
  }

  MPI_Finalize();

  return 0;
}
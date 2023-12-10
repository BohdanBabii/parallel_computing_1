#include <mpi.h>
#include <cstdlib>
#include <iomanip>
#include <iostream>

int main( int    i_argc,
          char * i_argv[] ) {
  // mpi startup
  MPI_Init( &i_argc,
            &i_argv );

  int l_comm_size = 0;
  MPI_Comm_size( MPI_COMM_WORLD,
                 &l_comm_size );

  int l_comm_rank = -1;
  MPI_Comm_rank( MPI_COMM_WORLD,
                 &l_comm_rank );

  std::cout << "Hi! I am rank " << l_comm_rank
            << " out of " << l_comm_size << " ranks."
            << std::endl;

  // Initialize Data
  float l_buf[l_comm_size];
  float l_single_buf = 0.0;

  if ( l_comm_rank == 0 )
  {
    for ( std::size_t l_da = 0; l_da < l_comm_size; l_da++ )
    {
      l_buf[l_da] = l_comm_rank;
    }
  }
  else
  {
    l_single_buf = l_comm_rank * 10;
  }

  // Create window, init on rank 0, print on the others
  MPI_Win l_win;
  if( l_comm_rank == 0 ) {
    MPI_Win_create( l_buf,          // base
                    l_comm_size*sizeof(float), // size
                    sizeof(float),   // disp_unit
                    MPI_INFO_NULL,   // info
                    MPI_COMM_WORLD,  // comm
                    &l_win );        // win
  }
  else {
    MPI_Win_create( NULL,           // base
                    0,              // size
                    sizeof(float),  // disp_unit
                    MPI_INFO_NULL,  // info
                    MPI_COMM_WORLD, // comm
                    &l_win );       // win
  }

  // Print initial values of rank 0
  if( l_comm_rank == 0 ) 
  {
    for( std::size_t l_da = 0; l_da < l_comm_size; l_da++ ) 
    {
      std::cout << "BEFORE: rank " << l_comm_rank << ": "
                << l_da << " / " << std::fixed << std::setprecision(1) << l_buf[l_da] << std::endl;
    }
  }

  // Creating fence for synchronisation with rank 0
  MPI_Win_fence( MPI_MODE_NOPRECEDE, // assert
                 l_win );            // win

  if( l_comm_rank != 0 ) {
    MPI_Win_lock( MPI_LOCK_SHARED, // lock_type
                  0,               // rank
                  0,               // assert
                  l_win );         // win

    MPI_Aint l_target_disp = l_comm_rank; // places data at the number of their rank in buf
    MPI_Put( &l_single_buf,               // origin_addr - Data to be send
             1,                           // origin_count - How many items to send
             MPI_FLOAT,                   // origin_datatype
             0,                           // target_rank - Where the data should go
             l_target_disp,               // target_disp - Where the data should be placed
             1,                           // target_count 
             MPI_FLOAT,                   // target_datatype
             l_win );                     // win

    MPI_Win_unlock( 0,       // rank
                    l_win ); // win
  }

  MPI_Win_fence( 0,       // assert
                 l_win ); // win

  // Print values of rank 0 after PUT
  if( l_comm_rank == 0 ) {
    for( std::size_t l_da = 0; l_da < l_comm_size; l_da++ ) {
      std::cout << "AFTER: rank " << l_comm_rank << ": "
                << l_da << " / " << std::fixed << std::setprecision(1) << l_buf[l_da] << std::endl;
    }
  }

  MPI_Win_free( &l_win );

  MPI_Finalize();

  return EXIT_SUCCESS;
}
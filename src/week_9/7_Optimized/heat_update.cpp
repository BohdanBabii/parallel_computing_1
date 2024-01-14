#include <mpi.h>
#include "heat.h"

// Include header files if necessary
void start_halo_exchange(Field *temperature, ParallelData *parallel)
{

    // This function should initiate the halo exchange to communicate boundary data between neighboring processes.

    // Width for accessing and navigating through the temperature field
    int width = temperature->ny + 2;
    int index = 0;

    // (up <-> down)
    // Communication 1: Send data to the upper neighbor and receive from the lower neighbor
    // This exchanges the ghost cells in the top row of the local temperature field

    MPI_Isend(
        &(temperature->data[idx(1, 0, width)]), // Pointer to Data
        1,                                      // Count of Data Items - Whole Row
        parallel->rowtype,                      // Data Type
        parallel->nup,                          // Receiver's Rank - Neighbor on Top
        0,                                      // Message Tag - IGNORE
        parallel->comm,                         // Communicator
        &(parallel->requests[index++])          // Request Object
    );

    MPI_Irecv(
        &(temperature->data[idx(temperature->nx + 1, 0, width)]), // Pointer to Data
        1,                                                        // Count of Data Items - Whole Row
        parallel->rowtype,                                        // Data Type
        parallel->ndown,                                          // Sender's Rank - Neighbor below
        0,                                                        // Message Tag - IGNORE
        parallel->comm,                                           // Communicator
        &(parallel->requests[index++])                            // Request Object
    );

    // (down <-> up)
    // Communication 2: Send data to the lower neighbor and receive from the upper neighbor
    // This exchanges the ghost cells in the bottom row of the local temperature field

    MPI_Isend(
        &(temperature->data[idx(temperature->nx, 0, width)]), // Pointer to Data
        1,                                                    // Count of Data Items
        parallel->rowtype,                                    // Data Type
        parallel->ndown,                                      // Receiver's Rank - Neighbor below
        0,                                                    // Message Tag - IGNORE
        parallel->comm,                                       // Communicator
        &(parallel->requests[index++])                        // Request Object
    );

    MPI_Irecv(
        &(temperature->data[idx(0, 0, width)]), // Pointer to Data - First Spot
        1,                                      // Count of Data Items - Start at one Cell
        parallel->rowtype,                      // Data Type
        parallel->nup,                          // Sender's Rank - Neighbor on top
        0,                                      // Message Tag - IGNORE
        parallel->comm,                         // Communicator
        &(parallel->requests[index++])          // Request Object
    );

    // (left <-> right)
    // Communication 3: Send data to the left neighbor and receive from the right neighbor
    // This exchanges the ghost cells in the leftmost column of the local temperature field

    MPI_Isend(
        &(temperature->data[idx(0, 1, width)]), // Pointer to Data
        1,                                      // Count of Data Items
        parallel->columntype,                   // Data Type
        parallel->nleft,                        // Receiver's Rank - Left Neighbor
        0,                                      // Message Tag - IGNORE
        parallel->comm,                         // Communicator
        &(parallel->requests[index++])          // Request Object
    );

    MPI_Irecv(
        &(temperature->data[idx(0, temperature->ny + 1, width)]), // Pointer to Data
        1,                                                        // Count of Data Items
        parallel->columntype,                                     // Data Type
        parallel->nright,                                         // Sender's Rank - Right Neighbor
        0,                                                        // Message Tag - IGNORE
        parallel->comm,                                           // Communicator
        &(parallel->requests[index++])                            // Request Object
    );

    // (right <-> left)
    // Communication 4: Send data to the right neighbor and receive from the left neighbor
    // This exchanges the ghost cells in the rightmost column of the local temperature field

    MPI_Isend(
        &(temperature->data[idx(0, temperature->ny, width)]), // Pointer to Data
        1,                                                    // Count of Data Items - The one Cell
        parallel->columntype,                                 // Data Type
        parallel->nright,                                     // Receiver's Rank - Right Neighbor
        0,                                                    // Message Tag - IGNORE
        parallel->comm,                                       // Communicator
        &(parallel->requests[index++])                        // Request Object
    );

    MPI_Irecv(
        &(temperature->data[idx(0, 0, width)]), // Pointer to Data - First Spot
        1,                                      // Count of Data Items - One Cell
        parallel->columntype,                   // Data Type
        parallel->nleft,                        // Sender's Rank - Left Neighbor
        0,                                      // Message Tag - IGNORE
        parallel->comm,                         // Communicator
        &(parallel->requests[index++])          // Request Object
    );
}

void complete_halo_exchange(ParallelData *parallel)
{
    // Wait for the completion of non-blocking communication requests related to halo exchange
    MPI_Waitall(8, parallel->requests, MPI_STATUS_IGNORE);
}

void update_interior_temperature(Field *curr, Field *prev, double a, double dt)
{

    // This function should update the interior temperature field based on the five-point stencil.

    // Indices for center, up, down, left, right
    // These indices are used for accessing neighboring grid points during the update.
    int i, j;
    int ic, iu, id, il, ir; // Indices for center, up, down, left, right

    // Width of the grid (number of columns)
    // The width is used to calculate the indices and navigate through the temperature field.
    int width;
    width = curr->ny + 2;

    // Parameters for the update
    // a: Thermal diffusivity
    // dt: Time step size

    // Determine the temperature field at the next time step
    // As fixed boundary conditions are applied, the outermost grid points are not updated.

    // Loop over the interior grid points for the update
    // Update the temperature using the five-point stencil

    double a_dt = a * dt;
    double term_1 = ((a_dt) / (prev->dx * prev->dx));
    double term_2 = ((a_dt) / (prev->dy * prev->dy));

    for (i = 1; i < curr->nx + 1; i++)
    {
        for (j = 1; j < curr->ny + 1; j++)
        {
            ic = idx(i, j, width);
            iu = idx(i - 1, j, width);
            id = idx(i + 1, j, width);
            ir = idx(i, j + 1, width);
            il = idx(i, j - 1, width);

            curr->data[ic] = prev->data[ic] + term_1 * (prev->data[ir] - 2 * prev->data[ic] + prev->data[il]) + term_2 * (prev->data[iu] - 2 * prev->data[ic] + prev->data[id]);
        }
    }
}

void update_boundary_temperature(Field *curr, Field *prev, double a, double dt)
{
    int i, j;
    int ic, iu, id, il, ir; // Indices for center, up, down, left, right
    int width;
    width = curr->ny + 2;

    double a_dt = a * dt;
    double term_1 = ((a_dt) / (prev->dx * prev->dx));
    double term_2 = ((a_dt) / (prev->dy * prev->dy));

    // Update the left and right borders
    i = 1;
    for (j = 1; j < curr->ny + 1; j++)
    {
        ic = idx(i, j, width);
        iu = idx(i - 1, j, width);
        id = idx(i + 1, j, width);
        ir = idx(i, j + 1, width);
        il = idx(i, j - 1, width);

        // Apply the five-point stencil to update the temperature at the left and right borders.
        curr->data[ic] = prev->data[ic] + term_1 * (prev->data[ir] - 2 * prev->data[ic] + prev->data[il]) + term_2 * (prev->data[iu] - 2 * prev->data[ic] + prev->data[id]);
    }

    i = curr->nx;
    for (j = 1; j < curr->ny + 1; j++)
    {
        // Update indicies using idx
        ic = idx(i, j, width);
        iu = idx(i - 1, j, width);
        id = idx(i + 1, j, width);
        ir = idx(i, j + 1, width);
        il = idx(i, j - 1, width);

        // Apply the five-point stencil to update the temperature at the left and right borders.
        curr->data[ic] = prev->data[ic] + term_1 * (prev->data[ir] - 2 * prev->data[ic] + prev->data[il]) + term_2 * (prev->data[iu] - 2 * prev->data[ic] + prev->data[id]);
    }

    // Update the upper and lower borders
    j = 1;
    for (i = 1; i < curr->nx + 1; i++)
    {
        // Update indicies using idx
        ic = idx(i, j, width);
        iu = idx(i - 1, j, width);
        id = idx(i + 1, j, width);
        ir = idx(i, j + 1, width);
        il = idx(i, j - 1, width);

        // Apply the five-point stencil to update the temperature at the upper and lower borders.
        curr->data[ic] = prev->data[ic] + term_1 * (prev->data[ir] - 2 * prev->data[ic] + prev->data[il]) + term_2 * (prev->data[iu] - 2 * prev->data[ic] + prev->data[id]);
    }

    // Update the lower and upper borders
    j = curr->ny;
    for (i = 1; i < curr->nx + 1; i++)
    {
        // Update indicies using idx
        ic = idx(i, j, width);
        iu = idx(i - 1, j, width);
        id = idx(i + 1, j, width);
        ir = idx(i, j + 1, width);
        il = idx(i, j - 1, width);

        // Apply the five-point stencil to update the temperature at the upper and lower borders.
        curr->data[ic] = prev->data[ic] + term_1 * (prev->data[ir] - 2 * prev->data[ic] + prev->data[il]) + term_2 * (prev->data[iu] - 2 * prev->data[ic] + prev->data[id]);
    }
}
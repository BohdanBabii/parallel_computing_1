#include <mpi.h>

#include "heat.h"

// Include header files if necessary

void start_halo_exchange(Field *temperature, ParallelData *parallel) {
    MPI_Isend(
        &(temperature->data[idx(1, 0, width)]),  // Pointer to Data
        1,                                       // Count of Data Items - Whole Row
        parallel->rowtype,                       // Data Type
        parallel->nup,                           // Receiver's Rank - Neighbor on Top
        0,                                       // Message Tag - IGNORE
        parallel->comm,                          // Communicator
        &(parallel->requests[index++])           // Request Object
    );

    MPI_Irecv(
        &(temperature->data[idx(temperature->nx + 1, 0, width)]),  // Pointer to Data
        1,                                                         // Count of Data Items - Whole Row
        parallel->rowtype,                                         // Data Type
        parallel->ndown,                                           // Sender's Rank - Neighbor below
        0,                                                         // Message Tag - IGNORE
        parallel->comm,                                            // Communicator
        &(parallel->requests[index++])                             // Request Object
    );
}

void complete_halo_exchange(ParallelData *parallel) {
    // Wait for the completion of non-blocking communication requests related to halo exchange
    MPI_Waitall(8, parallel->requests, MPI_STATUS_IGNORE);
}

void update_interior_temperature(Field *curr, Field *prev, double a, double dt) {
    int i, j;
    int i_c, i_u, i_d, i_r, i_l;

    int width = curr->ny + 2;

    double dy_2 = prev->dy * prev->dy;
    double dx_2 = prev->dx * prev->dx;

    for (i = 1; i < curr->nx + 1; i++) {
        for (j = 1; j < curr->ny + 1; j++) {
            i_c = idx(i, j, width);
            i_u = idx(i - 1, j, width);
            i_d = idx(i + 1, j, width);
            i_l = idx(i, j - 1, width);
            i_r = idx(i, j + 1, width);

            curr->data[i_c] = prev->data[i_c] + ((a * dt) / (dx_2)) * (prev->data[i_r] - 2 * prev->data[i_c] + prev->data[i_l]) + ((a * dt) / (dy_2)) * (prev->data[i_u] - 2 * prev->data[i_c] + prev->data[i_d]);
        }
    }
}

void update_boundary_temperature(Field *curr, Field *prev, double a, double dt) {
    int i, j;
    int i_c, i_u, i_d, i_l, i_r;
    int width = curr->ny + 2;
    double dx_2, dy_2;

    dx_2 = prev->dx * prev->dx;
    dy_2 = prev->dy * prev->dy;

    // Left-Right
    i = 1;
    for (j = 1; j < curr->ny + 1; j++) {
        i_c = idx(i, j, width);
        i_u = idx(i + 1, j, width);
        i_d = idx(i - 1, j, width);
        i_r = idx(i, j + 1, width);
        i_l = idx(i, j - 1, width);

        curr->data[i_c] = prev->data[i_c] + ((a * dt) / (dx_2)) * (prev->data[i_r] - 2 * prev->data[i_c] + prev->data[i_l]) + ((a * dt) / (dy_2)) * (prev->data[i_u] - 2 * prev->data[i_c] + prev->data[i_d]);
    }

    // Right-Left
    i = curr->nx;
    for (j = 1; j < curr->ny + 1; j++) {
        i_c = idx(i, j, width);
        i_u = idx(i + 1, j, width);
        i_d = idx(i - 1, j, width);
        i_r = idx(i, j + 1, width);
        i_l = idx(i, j - 1, width);

        curr->data[i_c] = prev->data[i_c] + ((a * dt) / (dx_2)) * (prev->data[i_r] - 2 * prev->data[i_c] + prev->data[i_l]) + ((a * dt) / (dy_2)) * (prev->data[i_u] - 2 * prev->data[i_c] + prev->data[i_d]);
    }

    // Upper-Lower
    j = 1;
    for (i = 1; i < curr->nx + 1; i++) {
        i_c = idx(i, j, width);
        i_u = idx(i + 1, j, width);
        i_d = idx(i - 1, j, width);
        i_r = idx(i, j + 1, width);
        i_l = idx(i, j - 1, width);

        curr->data[i_c] = prev->data[i_c] + ((a * dt) / (dx_2)) * (prev->data[i_r] - 2 * prev->data[i_c] + prev->data[i_l]) + ((a * dt) / (dy_2)) * (prev->data[i_u] - 2 * prev->data[i_c] + prev->data[i_d]);
    }

    // Lower-Upper
    j = curr->ny;
    for (i = 1; i < curr->nx + 1; i++) {
        i_c = idx(i, j, width);
        i_u = idx(i + 1, j, width);
        i_d = idx(i - 1, j, width);
        i_r = idx(i, j + 1, width);
        i_l = idx(i, j - 1, width);

        curr->data[i_c] = prev->data[i_c] + ((a * dt) / (dx_2)) * (prev->data[i_r] - 2 * prev->data[i_c] + prev->data[i_l]) + ((a * dt) / (dy_2)) * (prev->data[i_u] - 2 * prev->data[i_c] + prev->data[i_d]);
    }
}
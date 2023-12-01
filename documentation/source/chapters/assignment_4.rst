.. _ch:Task_4_1:

Report Week 6
=============

.. _ch:Contribution_4:

Contribution
------------

Bohdan Babii: Documentation, Part 7.1, 8

Denis Novikau: Part 7.1, 8

Lucas Obitz: Part 7.1, 8

.. _ch:part_4_1:

.. admonition:: Task

  #. Initialize an array of double values with a user-defined size, N, specified through the Terminal.
  #. Write a C/C++ function named "tree_broadcast" that uses a tree approach (like Fig. 7.1.1) for broadcasting, employing **blocking** communication.
  #. Compare the runtime of your "tree_broadcast" function with the standard "MPI_Bcast" function.
  #. Submit your work using a batch script. Include the script, the job output, and the implementation itself in the submission.

tree_broadcast
--------------

The **tree_broadcast** function implements a parallelized tree-based broadcast operation in an MPI environment. It distributes a double array from a designated root process to all other processes using a binary tree structure. The root generates random data, and each process communicates with its partner in the tree, sending or receiving data. Barrier synchronization ensures ordered execution. After completion, each process prints its rank and the received array. 

**tree_broadcast:**

.. code-block:: c++
    :linenos:

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


runtime comparison
------------------

In the given MPI program, the **MPI_Bcast** function outperforms the custom **tree_broadcast** implementation. The reported runtimes are:

.. math::

    \text{{MPI\_Bcast time: }} 9.2929 \times 10^{-5} \text{{ seconds.}} \\
    \text{{Custom tree\_broadcast time: }} 5.80006 \times 10^{-4} \text{{ seconds.}}

In this run, MPI_Bcast is approximately 6 times faster than tree_broadcast. The efficiency of the standard MPI broadcast operation is attributed to its optimized implementation, which handles broadcast operations more effectively than the custom tree-based approach.

SLURM script
------------

.. literalinclude:: ../../../src/week_6/tree_broadcast.sh
    :linenos:

SLURM output
------------

.. literalinclude:: ../../../src/week_6/tree_broadcast_job.out
    :linenos:

SLURM error
-----------

.. literalinclude:: ../../../src/week_6/tree_broadcast_job.err
    :linenos:
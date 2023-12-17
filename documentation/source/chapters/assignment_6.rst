.. _ch:Task_6_1:

Report Week 8
=============

.. _ch:Contribution_6:

Contribution
------------

Bohdan Babii: Documentation

Denis Novikau: Part 10.1

Lucas Obitz: Part 10.1

.. _ch:part_6_2:


Example Workloads
-----------------


.. admonition:: Task

   #. Use the `std::mt19937_64 <https://cplusplus.com/reference/random/mt19937_64/>`__ random number generator and `std::uniform_real_distribution <https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution>`_ to generate a sequence of random numbers in :math:`[0,1)`.
   #. Parallelize the random number generation using MPI.
      Ensure that each process has its own independent instance of the random number generator.
   #. Implement the described Monte Carlo method for the computation of :math:`\pi`.
      Calculate the error and required time-to-solution for 50,000,000 random samples.
   #. Parallelize your implementation using your desired MPI communication.

Random Number generator
-----------------------
This C++ code uses the MPI library to distribute the computation of estimating the value of Pi across multiple processes. It initializes MPI, determines the rank and size of the communication world, and sets up a random number generator. The number of elements to be processed is set to 50,000,000, and this is divided by the number of processes to determine how many iterations each process should perform. The x and y coordinates are initialized to 0.0, and a counter variable l_c is also initialized.

.. literalinclude:: ../../../src/week_8/pi.cpp
    :linenos:

SBATCH script
-------------

.. literalinclude:: ../../../src/week_8/script.sh
    :linenos:

Output file
-----------

.. literalinclude:: ../../../src/week_8/myjob.out
    :linenos:

Error file
----------

.. literalinclude:: ../../../src/week_8/myjob.err
    :linenos:
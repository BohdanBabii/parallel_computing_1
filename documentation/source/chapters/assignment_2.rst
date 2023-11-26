.. _ch:Task_2_1:

Report Week 4
=============

.. _ch:Contribution_2:

Contribution
------------

Bohdan Babii: Documentation, Part 1

Denis Novikau: Part 1, 2

Lucas Obitz: Part 1, 2

.. _ch:part_2_1:

.. admonition:: Task

  #. Research and provide an overview of two different MPI implementations for example comparing performance,.. (e.g., Intel MPI and Open MPI).
  #. Linear Arrangement:
  
    - Assume a linear arrangement of processes within the communicator, where each process is a neighbor to the one on its left and right, except for the first process that has a neighbor only on its right side, and the last process that has a neighbor only on its left side. For example, process 3 is neighbor with 2 and 4. But process 0 is just neighbor with process 1.
    - Write an MPI program that prints a message for each process, indicating its rank and its neighbors according to this linear scheme.
    - For process 4 the output has to be something like:  Hello from process rank 4 to my right neighbor (rank 5) and my left neighbor (rank 3) and so on. 

Part 1
------

#. **Intel MPI:**

    * Overview:
        
        * Developed by Intel Corporation.
        * Commercially available and part of the Intel Cluster Toolkit.

    * Features:
        
        * Optimized for Intel architectures.
        * Advanced debugging tools.
        * Integration with Intel software stack.

    * Performance:
        
        * Tailored for high performance on Intel-based clusters.
        * Performance varies based on hardware and workload.

* **Open MPI:**

    * Overview:
        
        * Open-source MPI implementation.
        * Designed for portability across architectures and operating systems.

    * Features:
        
        * Portability and scalability.
        * Active community support.
        * Versatile for various computing environments.

    * Performance:
        
        * Good performance on diverse platforms.
        * Influenced by hardware and configuration.

#. **Comparison:**

    * Licensing:
        
        * Intel MPI is commercial.
        * Open MPI is open source.

    * Optimization:
        
        * Intel MPI optimized for Intel architecture.
        * Open MPI offers flexibility but may lack specific optimizations.

    * Support:
        
        * Intel MPI has official support.
        * Open MPI relies on community support.

Part 2
------

**task_6_1.cpp**

The provided C++ MPI program demonstrates a basic circular data exchange among multiple processes. Using MPI_Send and MPI_Recv, the processes form a ring structure where process 0 initiates by sending a value to the next process. The value is then passed along until the last process sends it back to process 0. Each process prints messages indicating sent and received values along with ranks. This example showcases fundamental MPI communication functions for distributed computing, providing a concise illustration of message-passing concepts.

.. literalinclude:: ../../../src/week_5/task_6_1.cpp
    :linenos:

|

**result.log**


.. literalinclude:: ../../../src/week_5/results.log
    :linenos:

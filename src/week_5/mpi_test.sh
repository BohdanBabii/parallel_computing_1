#!/bin/bash
#SBATCH --job-name=mpi_test
#SBATCH --output=mpi_test.out
#SBATCH --error=mpi_test.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=2
#SBATCH --ntasks=2
#SBATCH --mem=10G
#SBATCH --time=00:01:00
#SBATCH --cpus-per-task=72

module load mpi/openmpi/4.1.2-gcc-10.2.0 

# Compile the MPI program
mpicxx -o exe main.cpp

# Execute the MPI program
mpirun -np 10 ./exe


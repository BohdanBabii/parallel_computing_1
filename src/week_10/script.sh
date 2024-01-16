#!/bin/bash
#SBATCH --job-name=myjob
#SBATCH --output=myjob.out
#SBATCH --error=myjob.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=1:00:00
#SBATCH --cpus-per-task=72

# Load any necessary modules (if needed)
module load mpi/openmpi/4.1.2-gcc-10.2.0
mpicxx -o matrix_multiplication.o matrix_multiplication.cpp

# Enter your executable commands here
# Execute the compiled program
mpirun -np ./matrix_multiplication.o

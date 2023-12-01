#!/bin/bash
#SBATCH --job-name=my_custom_reduction_job
#SBATCH --output=my_custom_reduction_job.out
#SBATCH --error=my_custom_reduction_job.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=1:30:00
#SBATCH --cpus-per-task=72

# Load any necessary modules (if needed)
module load mpi/openmpi/4.1.2-gcc-10.2.0

# Enter your executable commands here
mpicxx -o my_custom_reduction.o my_custom_reduction.cpp

# Enter your compiled program here
mpirun -np 20 ./my_custom_reduction.o

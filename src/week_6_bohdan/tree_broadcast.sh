#!/bin/bash
#SBATCH --job-name=tree_broadcast_job
#SBATCH --output=tree_broadcast_job.out
#SBATCH --error=tree_broadcast_job.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=1:30:00
#SBATCH --cpus-per-task=72

# Load any necessary modules (if needed)
module load mpi/openmpi/4.1.2-gcc-10.2.0

# Enter your executable commands here
mpicxx -o exe tree_broadcast.cpp

# Enter your compiled program here
mpirun -np 20 ./exe 10

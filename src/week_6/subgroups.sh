#!/bin/bash
#SBATCH --job-name=mysubgroup
#SBATCH --output=mysubgroup.out
#SBATCH --error=mysubgroup.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=1:30:00
#SBATCH --cpus-per-task=72

# Load any necessary modules
module load mpi/openmpi/4.1.2-gcc-10.2.0

# Enter your executable commands here
mpicxx -o subgroups.o subgroups.cpp

mpirun -np 16 ./subgroups.o

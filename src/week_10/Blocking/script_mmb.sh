#!/bin/bash
#SBATCH --job-name=matrix_matrix_blocking_test
#SBATCH --output=matrix_matrix_blocking_out.out
#SBATCH --error=matrix_matrix_blocking_err.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=72
#SBATCH --time=00:30:00

# Load Modules
module load compiler/gcc/9.3.0

# Compile the C++ program
g++  -march=native -O2 -o matrix_matrix_blocking.o matrix_matrix_blocking.cpp

# Run program
./matrix_matrix_blocking.o


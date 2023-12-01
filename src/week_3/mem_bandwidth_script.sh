#!/bin/bash
#SBATCH --job-name=memory_bandwidth_test
#SBATCH --output=mem_bandwidth_out.out
#SBATCH --error=mem_bandwidth_err.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --mem=183G
#SBATCH --time=1:30:00
#SBATCH --cpus-per-task=72

module load tools/python/3.8
module load libs/libxc/5.1.7-gcc-7.3.0

# Compile the C++ program
g++ -O3 -std=c++11 bandwidth.cpp -o memory_bandwidth.o

# Execute the C++ program
./memory_bandwidth.o

# Run the Python program
python csvreader.py
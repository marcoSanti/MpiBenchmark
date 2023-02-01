#!/bin/sh
#SBATCH -p broadwell
#SBATCH -N 40
#SBATCH -o /Users/marco/Desktop/MpiBenchmark/tests/arm64-Darwin/benchmark_log.log
#SBATCH -e /Users/marco/Desktop/MpiBenchmark/tests/arm64-Darwin/benchmark_err.err
#SBATCH --ntasks-per-node=1
srun --mpi=pmix ./mpi_bench

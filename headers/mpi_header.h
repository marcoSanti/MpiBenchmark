/**
 * @file mpi_header.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <general_headers.h>
#include <mpi.h>


/**
 * @brief 
 * 
 * @param tp 
 * @param sourceData 
 * @param processRank 
 * @param minWinSize 
 * @param maxWinSize 
 * @param winSizeStep 
 */
void runBenchmark(testType tp, FILE *sourceData, int processRank, int minWinSize, int maxWinSize, int winSizeStep);

benchmarkSubResult* _runBenchmark(testType testType, FILE* sourceData, double fileSize, int windowSize);


/**
 * @brief this function return whether the calling process has rank equal to 0, ie. it is the master
 * 
 * @return 1 it the process is master. 0 otherwise
 */
int is_mpi_master();

benchmarkResult* initBench(testType tp, long long fileSize);

void addBenchResToBenchmark(benchmarkResult *benchmark, benchmarkSubResult *subTest);

void freeBenchmarkMem(benchmarkResult *benchmark);
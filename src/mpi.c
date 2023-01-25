#include <mpi_header.h>
#include <utils.h>

/**
 * vedere mpi scatter e broadcast. per ora fare un benchmark su come andare a misurare bene i tempi di invio: invio e ricevo da altro lato e divido per due il tempo
 * poi vare un invio da un processo a N parti
 * mettere una barrier tra send e recive e prendere il tempo dopo la barrier.
 *
 * per il broadcast prendo tutto il file e lo invio a tutti quanti. poi sto a misurare con una barrier quanto tempo ci ha messo a inviare il file a tutti quanti.
 *
 *
 */

void runBenchmark(testType tp, FILE *sourceData, int processRank, int minWinSize, int maxWinSize, int winSizeStep)
{
    benchmarkResult *benchmark = initBench(tp, __TEMP_FILE_SIZE__);
    for (int winSize = minWinSize; winSize <= maxWinSize; winSize = winSize + winSizeStep)
    {
        benchmarkSubResult *subResult = _runBenchmark(tp, sourceData, __TEMP_FILE_SIZE__, winSize);
        if (processRank)
        {
            addBenchResToBenchmark(benchmark, subResult);
            log_print(benchmark_info, "Completed iteration ");
            printf("%d of %d for benchmark: %s\n", (int)(winSize / winSizeStep), (int)(maxWinSize / winSizeStep), testTypeToString(tp));
        }
    }
    if (processRank)
    {
        log_print(benchmark_info, "Executed cached send/recv benchmark\n");
        print_benchmark_result(benchmark);
        generateBenchmarkJSON(benchmark);
        log_print(info, "benchmark saved\n");
    }
    free(benchmark);
}

benchmarkSubResult *_runBenchmark(testType testType, FILE *sourceData, double fileSize, int windowSize)
{
    int rank, p;

    benchmarkSubResult *currentBenchmark = (benchmarkSubResult *)malloc(sizeof(benchmarkSubResult));

    currentBenchmark->windowSize = windowSize;

    MPI_CHECK(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
    MPI_CHECK(MPI_Comm_size(MPI_COMM_WORLD, &p));

    char *buffer = malloc(windowSize);

    switch (testType)
    {
    case Buffered:
        if (rank == 0)
        {
            fseek(sourceData, 0, SEEK_SET);

            // start timer
            currentBenchmark->testBeginTime = MPI_Wtime();

            // fill buffer with first block of data and then not repeat again
            fread(buffer, windowSize, 1, sourceData);

            for (double i = 0; i < fileSize; i = i + windowSize)
            {
                MPI_CHECK(MPI_Send(buffer, 1, MPI_BYTE, 1, __MPI_SEND_DATA__, MPI_COMM_WORLD));
            }

            // send termination message to reciver
            MPI_CHECK(MPI_Send(buffer, 1, MPI_BYTE, 1, __MPI_TERMINATE_CONNECTION__, MPI_COMM_WORLD));

        }
        else
        {
            int continue_reciving = 1;
            MPI_Status status;
            while (continue_reciving)
            {
                MPI_CHECK(MPI_Recv(buffer, 1, MPI_BYTE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status));
                if (status.MPI_TAG == 1) // terminate execution
                    continue_reciving = 0;
            }
        }

        // barrier to wait for the reciver to have recived all data
        MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));
        if(rank==0){
            currentBenchmark->testEndTime = MPI_Wtime();
            //save the elapsed time of the benchmark
            currentBenchmark->testBandWidth = fileSize / (currentBenchmark->testEndTime - currentBenchmark->testBeginTime);
        }
        

        break;

    default:
        break;
    }

    return currentBenchmark;
}

int is_mpi_master()
{
    int rank;
    MPI_CHECK(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
    return rank == 0;
}

benchmarkResult *initBench(testType tp, long long fileSize)
{
    benchmarkResult *currentBenchmark = (benchmarkResult *)malloc(sizeof(benchmarkResult));
    currentBenchmark->benchmark_type = tp;
    currentBenchmark->fileTransferSize = fileSize;
    currentBenchmark->TestTimeStamp = (unsigned long)time(NULL);
    currentBenchmark->benchmarkCount = 0;
    currentBenchmark->listOfBenchmarks = NULL;

    return currentBenchmark;
}

void addBenchResToBenchmark(benchmarkResult *benchmark, benchmarkSubResult *subTest)
{
    if (benchmark->benchmarkCount == 0)
        benchmark->listOfBenchmarks = (benchmarkSubResult **)malloc(sizeof(benchmarkSubResult *));
    else
        benchmark->listOfBenchmarks = (benchmarkSubResult **)realloc(benchmark->listOfBenchmarks, ((benchmark->benchmarkCount) + 1) * sizeof(benchmarkResult *));

    (benchmark->benchmarkCount)++;

    (benchmark->listOfBenchmarks)[(benchmark->benchmarkCount) - 1] = subTest;
    return;
}

void freeBenchmarkMem(benchmarkResult *benchmark)
{
    int benchCount = benchmark->benchmarkCount;
    for (int i = 0; i < benchCount; i++)
        free(benchmark->listOfBenchmarks[i]);
    free(benchmark->listOfBenchmarks);
    free(benchmark);
}
/**
 * @file main.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-01-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <general_headers.h>
#include <generate_random_data.h>
#include <utils.h>
#include <mpi_header.h>

int main(int argc, char *argv[])
{
    MPI_CHECK(MPI_Init(&argc, &argv));

    FILE *sampleData;
    int processRank = is_mpi_master();
    double start_bench_time, end_bench_time; // to measure global execution time
    char *sysHostname = (char *)malloc(100);
    gethostname(sysHostname, 100);

    if (processRank)
    {

        struct utsname systemInformations;
        uname(&systemInformations);

        start_bench_time = MPI_Wtime();

        // remove at end the temp file
        atexit(remove_sample_data);

        printf("\n\nMpi benchmark version %s\nCreated by %s\n%s\n\n", __MPI_BENCH_VERSION__, __MPI_BENCH_AUTHOR__, __MPI_BENCH_COMPANY__);

        char textBuffer[500];
        log_print(system_info, "System informations for master node: ");
        printf("%s:\n", sysHostname);

        sprintf(textBuffer, "System architecture: %s\n", systemInformations.machine);
        log_print(system_info, textBuffer);

        sprintf(textBuffer, "System OS: %s\n", systemInformations.sysname);
        log_print(system_info, textBuffer);

        sprintf(textBuffer, "System Kernel version: %s\n", systemInformations.version);
        log_print(system_info, textBuffer);

        int cpusAvail = sysconf(_SC_NPROCESSORS_ONLN);
        sprintf(textBuffer, "Online CPUs: %d\n", cpusAvail);
        log_print(system_info, textBuffer);

        double availMemGB = (sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE)) / 1073741824; // memory available in GB
        sprintf(textBuffer, "Available memory: %.2f GB\n", availMemGB);
        log_print(system_info, textBuffer);

        printf("\n");
        sampleData = generate_file(__TEMP_FILE_SIZE__);
        generateSystemInfoJSON(cpusAvail, availMemGB, systemInformations.machine, systemInformations.sysname, systemInformations.version);
        log_print(info, "Begin benchmark execution\n");
    }

    MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));

    // getting node names
    if (processRank)
    {
        log_print(system_info, "Master process running on ");
        printf("%s\n", sysHostname);
    }
    else
    {
        log_print(system_info, "Slave process running on ");
        printf("%s\n", sysHostname);
    }
    free(sysHostname);

    MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));

    // chaching baselie benchmark
    runBenchmark(cachedBaseline, sampleData ,processRank, 1000, 10000, 1000);
    MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));

    // blocking benchmark
    runBenchmark(blocking, sampleData ,processRank, 1000, 10000, 1000);
    MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));

    // imediate send imediate recive
    runBenchmark(Immediate, sampleData ,processRank, 1000, 10000, 1000);
    MPI_CHECK(MPI_Barrier(MPI_COMM_WORLD));


    MPI_CHECK(MPI_Finalize());

    if (processRank)
    {
        end_bench_time = MPI_Wtime();
        log_print(info, "Benchmark took ");
        printf("%d seconds to complete\n", (int)(end_bench_time - start_bench_time));
        fclose(sampleData);
    }
}
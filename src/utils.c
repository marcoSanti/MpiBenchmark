#include <general_headers.h>
#include <utils.h>

void log_print(log_msg_type type, char *message)
{
    printf("[");
    switch (type)
    {
    case info:
        printf(C_CYAN);
        printf("   info    ");
        break;
    case warning:
        printf(C_YELLOW);
        printf("  warning  ");
        break;
    case error:
        printf(C_RED);
        printf("   error   ");
        break;
    case benchmark_info:
        printf(C_GREEN);
        printf(" benchmark ");
        break;
    case system_info:
        printf(C_MAGENTA);
        printf("  system   ");
        break;
    default:
        break;
    }
    printf(C_DEFAULT);
    printf("] ");
    printf("%s", message);
}

char *testTypeToString(testType testType)
{
    char *buff = (char *)malloc(50);
    switch (testType)
    {
    case cachedBaseline:
        memcpy(buff, "cached_send_recv\0", sizeof("cached_send_recv\0"));
        break;

    case blocking:
        memcpy(buff, "send_recv\0", sizeof("send_recv\0"));
        break;

    case Immediate:
        memcpy(buff, "isend_irecv\0", sizeof("isend_irecv\0"));
        break;
    default:
        break;
    };
    return buff;
}


void print_benchmark_result(benchmarkResult *benchmark)
{
    int benchCount = benchmark->benchmarkCount;
    printf(__JSON_TEST_OUT_FORMAT_OPEN__,
           testTypeToString(benchmark->benchmark_type),
           benchmark->TestTimeStamp,
           benchmark->fileTransferSize);

    for (int i = 0; i < benchCount; i++)
    {
        printf(___JSON_SUB_TEST_FILE_FORMAT__,
               (benchmark->listOfBenchmarks[i])->windowSize,
               (benchmark->listOfBenchmarks[i])->testBeginTime,
               (benchmark->listOfBenchmarks[i])->testEndTime,
               (benchmark->listOfBenchmarks[i])->testBandWidth / __BANDWIDTH_UNIT_CONV_FACTOR__,
               __BANDWIDTH_MEASURE_UNIT__);
        if (i + 1 < benchCount)
            printf(",\n");
        else
            printf("\n");
    }

    printf(__JSON_TEST_OUT_FORMAT_CLOSE__);
}

void generateBenchmarkJSON(benchmarkResult *benchmark)
{
    int subTestCount = benchmark->benchmarkCount;
    int filepathlen = strlen(__TEST_OUT_DIR__) + 100; // len of path + 100 char for string "/test-<type>-<timestamp>.json\0"
    char *fileNameBuffer = malloc(filepathlen);
    memcpy(fileNameBuffer, __TEST_OUT_DIR__, sizeof(__TEST_OUT_DIR__));

    char testNameString[100];
    sprintf(testNameString, "/TEST-%s-%lu.json", testTypeToString(benchmark->benchmark_type), benchmark->TestTimeStamp);

    strcat(fileNameBuffer, testNameString);

    log_print(info, "generating test info file at: ");
    printf("%s\n", fileNameBuffer);

    FILE *out = fopen(fileNameBuffer, "w");
    char outFileBuffer[1024];
    fprintf(out,
            __JSON_TEST_OUT_FORMAT_OPEN__,
            testTypeToString(benchmark->benchmark_type),
            benchmark->TestTimeStamp,
            benchmark->fileTransferSize);

    for (int i = 0; i < subTestCount; i++)
    {
        fprintf(out,
                ___JSON_SUB_TEST_FILE_FORMAT__,
                (benchmark->listOfBenchmarks[i])->windowSize,
                (benchmark->listOfBenchmarks[i])->testBeginTime,
                (benchmark->listOfBenchmarks[i])->testEndTime,
                (benchmark->listOfBenchmarks[i])->testBandWidth / __BANDWIDTH_UNIT_CONV_FACTOR__,
                __BANDWIDTH_MEASURE_UNIT__);
        if (i + 1 < subTestCount)
            fprintf(out, ",\n");
        else
            fprintf(out, "\n");
    }

    fprintf(out, __JSON_TEST_OUT_FORMAT_CLOSE__);
    fclose(out);
    free(fileNameBuffer);
}

void generateSystemInfoJSON(int cpus, float memoryGB, char *sysArch, char *sysOs, char *sysKernel)
{

    int filepathlen = strlen(__TEST_OUT_DIR__) + 15; // len of path + 15 chat for string "/sysinfo.json\0"
    char *fileNameBuffer = malloc(filepathlen);
    memcpy(fileNameBuffer, __TEST_OUT_DIR__, sizeof(__TEST_OUT_DIR__));
    strcat(fileNameBuffer, "/sysinfo.json\0");
    log_print(info, "generating system info file at: ");
    printf("%s\n", fileNameBuffer);

    FILE *out = fopen(fileNameBuffer, "w");
    fprintf(out,
            __JSON_SYS_INFO_FORMAT__,
            (unsigned long)time(NULL),
            __BUILD_DIR__,
            __TEST_OUT_DIR__,
            sysArch,
            sysOs,
            sysKernel,
            cpus,
            memoryGB);
    fclose(out);
    free(fileNameBuffer);
}
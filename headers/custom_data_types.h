
// data types for mpi
typedef enum _benchmark_test_type
{
    cachedBaseline,
    blocking,
    Immediate,
    oneSided
} testType;

// struct to store benchmark result

typedef struct _benchmarkSubResult{
    long long int windowSize;
    double testBeginTime;
    double testEndTime;
    double testBandWidth;
} benchmarkSubResult;

typedef struct _benchmarkResult
{
    testType benchmark_type;
    long long int fileTransferSize;
    unsigned long TestTimeStamp;
    int benchmarkCount;
    benchmarkSubResult **listOfBenchmarks;
} benchmarkResult;



typedef enum
{
    benchmark_info,
    info,
    warning,
    error,
    system_info
} log_msg_type;
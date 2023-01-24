#include <general_headers.h>

void log_print(log_msg_type type, char* message);

void print_benchmark_result(benchmarkResult *benchmark);

void generateBenchmarkJSON(benchmarkResult *benchmark);

void generateSystemInfoJSON(int cpus, float memoryGB, char* sysArch, char* sysOs, char* sysKernel);

char* testTypeToString(testType testType);


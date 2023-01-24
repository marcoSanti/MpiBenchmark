#define __MPI_BENCH_VERSION__ "0.1"
#define __MPI_BENCH_AUTHOR__ "Marco Edoardo Santimaria (marco.santimaria@edu.unito.it)"
#define __MPI_BENCH_COMPANY__ "Universita degli studi di Torino"

// definition of bytes multiples
#define KBYTES *1024
#define MBYTES *1024 KBYTES
#define GBYTES *1024 MBYTES


//default parameters
#define __TEMP_FILE_NAME__ "./mpi_bench_temp_src"
#define __WINDOW__SIZE__ 200
#define __TEMP_FILE_SIZE__ 1 GBYTES

#define __MPI_TERMINATE_CONNECTION__ 1
#define __MPI_SEND_DATA__ 2


//theese variables control the conversion factor from bytes/ seconds to whathever is specified here
#define __BANDWIDTH_MEASURE_UNIT__ "Mbps"
#define __BANDWIDTH_UNIT_CONV_FACTOR__ (1e6)*8


// color scheme for printing to log console
#define C_DEFAULT "\033[39m"
#define C_BLACK "\033[30m"
#define C_RED "\033[31m"
#define C_GREEN "\033[32m"
#define C_YELLOW "\033[33m"
#define C_BLUE "\033[34m"
#define C_MAGENTA "\033[35m"
#define C_CYAN "\033[36m"
#define C_GRAY "\033[37m"
#define C_WHITE "\033[97m"
#define BG_C_DEFAULT "\033[49m"
#define BG_C_BLACK "\033[40m"
#define BG_C_RED "\033[41m"
#define BG_C_GREEN "\033[42m"
#define BG_C_YELLOW "\033[43m"
#define BG_C_BLUE "\033[44m"
#define BG_C_MAGENTA "\033[45m"
#define BG_C_CYAN "\033[46m"
#define BG_C_GRAY "\033[100m"
#define BG_C_WHITE "\033[107m"


//error checking macro for mpi;
#define MPI_CHECK(stmt)                                          \
do {                                                             \
   int mpi_errno = (stmt);                                       \
   if (MPI_SUCCESS != mpi_errno) {                               \
       fprintf(stderr, "[%s:%d] MPI call failed with %d: %s\n",  \
        __FILE__, __LINE__,mpi_errno, strerror(mpi_errno));      \
       exit(EXIT_FAILURE);                                       \
   }                                                             \
   assert(MPI_SUCCESS == mpi_errno);                             \
} while (0)


// FILE JSON OUT FORMAT
#define __JSON_TEST_OUT_FORMAT_OPEN__ "\
{\n\
    \"test_type\": \"%s\",\n\
    \"test_timestamp\": %lu,\n\
    \"file_transfer_size\": %llu,\n\
    \"test_results\": [\n"

#define __JSON_TEST_OUT_FORMAT_CLOSE__ "\
    ]\n\
}\n"

#define ___JSON_SUB_TEST_FILE_FORMAT__ "\
        {\n\
            \"transfer_window_size\": %llu,\n\
            \"test_begin_clock\":%lf,\n\
            \"test_end_clock\":%lf,\n\
            \"test_bandwidth\": %lf,\n\
            \"bandwidth_unit\": \"%s\"\n\
        }"


#define __JSON_SYS_INFO_FORMAT__ "\
{\n\
    \"test_timestamp\":%lu ,\n\
    \"sys_info\":{\n\
        \"build_directory\": \"%s\",\n\
        \"test_output_dir\": \"%s\",\n\
        \"system_architecture\": \"%s\",\n\
        \"system_os\": \"%s\",\n\
        \"system_kernel\": \"%s\",\n\
        \"available_cpu\": \"%d\",\n\
        \"available_mem_GB\": \"%lf\"\n\
    }\n\
}\n"


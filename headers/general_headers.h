/**
 * @file geenral_headers.h
 * @author Marco Edoardo Santimaria
 * @brief General header files required by all code
 * @version 0.1
 * @date 2023-01-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __INCLUDE_GENERALS__
#define __INCLUDE_GENERALS__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <assert.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/utsname.h>

#include <constants.h>
#include <custom_data_types.h>


//inclusion of build dependant header file (contains build folder and test output foldeer)
#if __has_include("local_config.h")
    #include <local_config.h>
#endif

//inclusion of custom configuration file. this file tells the benchmark test file and other eventual parameters if they differ from default behaviour
#if __has_include("custom_config.h")
    #include <custom_config.h>
#endif




#endif


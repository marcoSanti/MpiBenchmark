#!/bin/bash

CC=$(which mpicc)
DESTARCH=$(uname -m)
DESTOS=$(uname -s)
BUILDDIR=$(pwd)"/build/$DESTARCH-$DESTOS"
HEADERSFOLDER=$(pwd)"/headers"
TESTRESULTDIR=$(pwd)"/tests/$DESTARCH-$DESTOS"
KERNELVERSION=$(uname -r)

generate_configuration (){
    #echo some build variables to the configuration header file...
    echo "Enter size for test file followed by measure units of test file: (KBYTES, MBYTES, GBYTES):"
    read -r CONFIG_TEST_FILE_SIZE CONFIG_TEST_UNIT_MEASURE
    echo "Enter windo size in bytes"
    read CONFIG_WIN_SIZE
    echo "#undef __TEMP_FILE_SIZE__" > $HEADERSFOLDER/custom_config.h
    echo "#undef __WINDOW__SIZE__" > $HEADERSFOLDER/custom_config.h
    echo "#define __TEMP_FILE_SIZE__ $CONFIG_TEST_FILE_SIZE $CONFIG_TEST_UNIT_MEASURE" >> $HEADERSFOLDER/custom_config.h
    echo "#define __WINDOW__SIZE__ $CONFIG_WIN_SIZE" >> $HEADERSFOLDER/custom_config.h
    echo "Configuration saved into $HEADERSFOLDER/custom_config.h"
}

build_benchmark (){

    echo "#define __BUILD_DIR__ \"$BUILDDIR\"" > $HEADERSFOLDER/local_config.h
    echo "#define __TEST_OUT_DIR__ \"$TESTRESULTDIR\"" >> $HEADERSFOLDER/local_config.h

    COMPILER=$CC" -I "$(pwd)"/headers"
    echo "compiler is "$CC
    echo "current architecture is" ${DESTARCH}
    echo "current build folder is set to" ${BUILDDIR}
    mkdir -p $TESTRESULTDIR
    mkdir -p $BUILDDIR
    $COMPILER -c ./src/utils.c -o ${BUILDDIR}/utils.o
    $COMPILER -c ./src/generate_random_data.c -o ${BUILDDIR}/generate_random_data.o
    $COMPILER -c ./src/mpi.c -o ${BUILDDIR}/mpi.o
    $COMPILER -c ./src/main.c -o ${BUILDDIR}/main.o
    $COMPILER ${BUILDDIR}/utils.o ${BUILDDIR}/generate_random_data.o  ${BUILDDIR}/mpi.o ${BUILDDIR}/main.o -o $BUILDDIR/mpi_bench
}

 
clean_build_folder (){
    echo "Removing "$(pwd)"/build/"
    rm -rf ./build
    echo "Removing headers/local_config.h"
    rm headers/local_config.h
    echo "Removing headers/custom_config.h"
    rm headers/custom_config.h
}

run_benchmark (){
    echo "executing "$BUILDDIR"/mpi_bench"
    echo
    echo
    mpirun -np 2 $BUILDDIR/mpi_bench
}

build_run (){
    build_benchmark
    run_benchmark
}

remove_tests (){
    echo "Removing test result folder"
    rm -rf ./tests
}

help_func (){
    echo "Usage of the file: \n\t./build.sh [ | build | genconf | clean | run | buildrun | cleantest | cleanall ]"
    echo
    echo "build = compiles the benchmark"
    echo
    echo "makeconf = generate the configuration file for the benchmark. if not called, when building the benchmark, the default transfer size file will be 1 Gbyte"
    echo
    echo "clean = deletes all builded version of the benchmark"
    echo
    echo "run = executes the benchmark"
    echo
    echo "buildrun = generate the configuration, compiles and executes the benchmark."
    echo
    echo "cleantest = removes all previous test saved files"
    echo
    echo "cleanall = removes compiled files as well as prevous run test results"
}

if [[ $1 == "build" ]]
then
    
    build_benchmark
    
elif [[ $1 == "makeconf" ]]
then
    
    generate_configuration
    
elif [[ $1 == "clean" ]]
then
    
    clean_build_folder
    
elif [[ $1 == "run" ]]
then
    run_benchmark
    
elif [[ -z "$1" ]] || [[ "$1" == "buildrun" ]]
then

    build_benchmark
    run_benchmark

elif [[ $1 == "cleantest" ]]
then

    remove_tests

elif [[ $1 == "cleanall" ]]
then

    remove_tests
    clean_build_folder

else
    echo "Uknown option $1"
    help_func
fi


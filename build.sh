#!/bin/bash

#CONFIG VARIABLES
TARGETCLUSTER="broadwell"
NODECOUNT=10
MINWINSIZE=1000
MAXWINSIZE=10000
WINSIZESTEP=1000
#END OF CONFIG VARIABLES


CC="mpicc"
DESTARCH=$(uname -m)
DESTOS=$(uname -s)
BUILDDIR=$(pwd)"/build/$DESTARCH-$DESTOS"
HEADERSFOLDER="$(pwd)/headers/"
TESTRESULTDIR="$(pwd)/tests/$DESTARCH-$DESTOS"
KERNELVERSION=$(uname -r)


echo $HEADERSFOLDER

build_benchmark (){

    echo "#define __BUILD_DIR__ \"$BUILDDIR\"" > $HEADERSFOLDER/local_config.h
    echo "#define __TEST_OUT_DIR__ \"$TESTRESULTDIR\"" >> $HEADERSFOLDER/local_config.h
    echo "#define __NODE__COUNT__ $NODECOUNT" >> $HEADERSFOLDER/local_config.h
    echo "#define __MIN_WIN_SIZE__ $MINWINSIZE" >> $HEADERSFOLDER/local_config.h
    echo "#define __MAX_WIN_SIZE__ $MAXWINSIZE" >> $HEADERSFOLDER/local_config.h
    echo "#define __WIN_SIZE_STEP__ $WINSIZESTEP" >> $HEADERSFOLDER/local_config.h

    COMPILER="$CC -I$HEADERSFOLDER"

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
    mpirun -np $NODECOUNT $BUILDDIR/mpi_bench
}

build_run (){
    build_benchmark
    run_benchmark
}

remove_tests (){
    echo "Removing test result folder"
    rm -rf ./tests
}

slurm_run(){

   # srun -p $TARGETCLUSTER -n 1 --pty ./build.sh build

    echo "#!/bin/sh" > sbatch.sh
    echo "#SBATCH -p $TARGETCLUSTER" >> sbatch.sh
    echo "#SBATCH -N $NODECOUNT" >> sbatch.sh
    echo "#SBATCH -o $TESTRESULTDIR/benchmark_log.log" >> sbatch.sh
    echo "#SBATCH -e $TESTRESULTDIR/benchmark_err.err" >> sbatch.sh
    echo "#SBATCH --ntasks-per-node=1" >> sbatch.sh
    echo "srun --mpi=pmix $BUILDDIR/mpi_bench" >> sbatch.sh
    sbatch sbatch.sh
}

help_func (){
    echo ""
}

if [[ $1 == "build" ]]
then

    build_benchmark

elif [[ $1 == "clear" ]]
then

    clean_build_folder

elif [[ $1 == "run" ]]
then
    run_benchmark

elif [[ -z "$1" ]] || [[ "$1" == "buildrun" ]]
then

    build_benchmark
    run_benchmark

elif [[ $1 == "clean" ]]
then
    remove_tests
    clean_build_folder

elif [[ $1 == "slurm_run" ]]
then

    slurm_run

else
    echo "Uknown option $1"
    help_func
fi

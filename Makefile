#CONFIG VARIABLES
TARGETCLUSTER=broadwell
NODECOUNT=40
MINWINSIZE=1000
MAXWINSIZE=10000
WINSIZESTEP=1000
#END OF CONFIG VARIABLES

CC=$(shell which mpicc) 
CCINC=-I${HEADERSFOLDER}
DESTARCH=$(shell uname -m)
DESTOS=$(shell uname -s)
BUILDDIR=$(shell pwd)/build/${DESTARCH}-${DESTOS}
HEADERSFOLDER=$(shell pwd)/headers
TESTRESULTDIR=$(shell pwd)/tests/${DESTARCH}-${DESTOS}
KERNELVERSION=$(shell uname -r)



build: ${HEADERSFOLDER}/local_config.h ${BUILDDIR}/utils.o ${BUILDDIR}/generate_random_data.o ${BUILDDIR}/mpi.o ${BUILDDIR}/main.o mpi_bench.sh
	${CC} ${CCINC} ${BUILDDIR}/main.o ${BUILDDIR}/utils.o ${BUILDDIR}/generate_random_data.o  ${BUILDDIR}/mpi.o -o ${BUILDDIR}/mpi_bench
	cp ${BUILDDIR}/mpi_bench ./


mpi_bench.sh:
	echo "#!/bin/sh" > mpi_bench.sh
	echo "#SBATCH -p ${TARGETCLUSTER}" >> mpi_bench.sh
	echo "#SBATCH -N ${NODECOUNT}" >> mpi_bench.sh
	echo "#SBATCH -o ${TESTRESULTDIR}/benchmark_log.log" >> mpi_bench.sh
	echo "#SBATCH -e ${TESTRESULTDIR}/benchmark_err.err" >> mpi_bench.sh
	echo "#SBATCH --ntasks-per-node=1" >> mpi_bench.sh
	echo "srun --mpi=pmix ./mpi_bench" >> mpi_bench.sh


${BUILDDIR}/main.o:
	${CC} ${CCINC} -c ./src/main.c -o ${BUILDDIR}/main.o

${BUILDDIR}/mpi.o:
	${CC} ${CCINC} -c ./src/mpi.c -o ${BUILDDIR}/mpi.o

${BUILDDIR}/generate_random_data.o:
	${CC} ${CCINC} -c ./src/generate_random_data.c -o ${BUILDDIR}/generate_random_data.o

${BUILDDIR}/utils.o:
	${CC} ${CCINC} -c ./src/utils.c -o ${BUILDDIR}/utils.o

${HEADERSFOLDER}/local_config.h:
	echo "compiler is ${CC}"
	echo "current architecture is" ${DESTARCH}
	echo "current build folder is set to" ${BUILDDIR}
	mkdir -p ${TESTRESULTDIR}
	mkdir -p ${BUILDDIR}
	echo "#define __BUILD_DIR__ \"${BUILDDIR}\"" > ${HEADERSFOLDER}/local_config.h
	echo "#define __TEST_OUT_DIR__ \"${TESTRESULTDIR}\"" >> ${HEADERSFOLDER}/local_config.h
	echo "#define __NODE__COUNT__ ${NODECOUNT}" >> ${HEADERSFOLDER}/local_config.h
	echo "#define __MIN_WIN_SIZE__ ${MINWINSIZE}" >> ${HEADERSFOLDER}/local_config.h
	echo "#define __MAX_WIN_SIZE__ ${MAXWINSIZE}" >> ${HEADERSFOLDER}/local_config.h
	echo "#define __WIN_SIZE_STEP__ ${WINSIZESTEP}" >> ${HEADERSFOLDER}/local_config.h


clear:
	rm -f mpi_bench_temp_src
	rm -f mpi_bench.sh
	rm -f ${HEADERSFOLDER}/local_config.h
	rm -f mpi_bench
	rm -rf ./build
	rm -rf ./tests


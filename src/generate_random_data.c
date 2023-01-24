/**
 * @file generate_random_data.c
 * @author Marco Edoardo Santimaria
 * @brief this file contains the function required to generate the file to be sent with mpi
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <generate_random_data.h>
#include <general_headers.h>
#include <utils.h>

extern int errno;

//TODO: ottimizzare la scrittura e fare per blocchi di N bytes
//TODO: riunonimare questo file che non è un gran che al momento
FILE* generate_file(double bytes){

    log_print(info, "Started to generate test file\n");

    srand(getpid());

    char* buffer = malloc(bytes);

    int out = open(__TEMP_FILE_NAME__, O_CREAT | O_WRONLY | O_TRUNC, 0666);

    if(out < 1){
        log_print(error, "Error while opening file: ");
        printf("%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int rand_src = open("/dev/random",O_RDONLY);

    read(rand_src, buffer, bytes);
    if( write(out, buffer, bytes) < 1){
        log_print(error, "Error unable to write to file...: ");
        printf("%s\n",  strerror(errno));
        exit(EXIT_FAILURE);
    }
    

    close(rand_src);
    close(out);
    log_print(info, "Test file generated\n");
    FILE* fp;
    if((fp =fopen(__TEMP_FILE_NAME__, "r")) == NULL ){
        log_print(error, "Error opening file: ");
        printf("%s\n", strerror(errno));
    }

    return fp;
}

void remove_sample_data(){
    remove(__TEMP_FILE_NAME__ );
    log_print(info, "Removed test file\n");
}

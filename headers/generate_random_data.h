

#include <general_headers.h>

/**
 * @brief thif function generates a random file with <size> random bytes. The file is called whatever is configured into the __TEMP_FILE_NAME__ constant
 * 
 * @param bytes size of the temporary file
 * @return FILE* file pointer of the written file 
 */
FILE* generate_file(double bytes);

/**
 * @brief This method deletes the temporary file
 * 
 */
void remove_sample_data();

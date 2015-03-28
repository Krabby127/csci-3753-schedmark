#include <stdlib.h>
#include <stdio.h>
#include "primes.h"



int main(int argc, char* argv[]){

    // check argument count
    if (argc < 2 || argc > 3){
        fprintf(stderr, "ERROR: Expected 1 or two inputs.\n");
        fprintf(stderr,
                "Usage: %s <number of primes> <optional output file>\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    // parse first argument
    size_t num_primes = 0;
    if (sscanf(argv[1], "%zu", &num_primes) != 1){
        fprintf(stderr, "Expected integer argument.\n");
    }

    // calculate primes
    if (argc == 3){

        // save results to file
        if (primes_file(num_primes, argv[2])){
            return EXIT_FAILURE;
        }

    } else {

        // print results to screen
        printf("Calculating %zu primes.\n\n", num_primes);
        unsigned int *prime_nums = malloc(num_primes*sizeof(unsigned int));
        if (prime_nums == NULL){
            fprintf(stderr, "ERROR: Failed to allocate memory.\n");
            return EXIT_FAILURE;
        }
        primes_verbose(num_primes, prime_nums);
        free(prime_nums);

    }



    return EXIT_SUCCESS;

}

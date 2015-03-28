#include <stdlib.h>
#include <stdio.h>
#include "primes.h"



int main(int argc, char* argv[]){

    // check argument count
    if (argc != 2){
        fprintf(stderr, "ERROR: Expected exactly 1 input.\n");
        fprintf(stderr, "Usage: %s <number of primes>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // parse argument
    size_t num_primes = 0;
    if (sscanf(argv[1], "%zu", &num_primes) != 1){
        fprintf(stderr, "Expected integer argument.\n");
    }

    // calculate primes
    printf("Calculating %zu primes.\n\n", num_primes);
    unsigned int *prime_nums = malloc(num_primes*sizeof(unsigned int));
    primes_verbose(num_primes, prime_nums);
    free(prime_nums);

}

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "primes.h"




int main(int argc, char* argv[])
{
    bool verbose = false;

    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "hv")) != -1){
        switch (c){
            case 'h':
                fprintf(stdout, 
"NAME\n"
"       prime - computes prime numbers\n"
"\n"
"SYNOPSIS\n"
"       %s [-h] [-v] primes [output]\n"
"\n"
"DESCRIPTION\n"
"       Takes the number of primes to generate and uses the rather\n"
"       non-optimized algorithm found at\n"
"           http://docs.cython.org/src/tutorial/cython_tutorial.html\n"
"\n"
"OPTIONS\n"
"       -h      Display this message.\n"
"\n"
"       -v      Enable verbose output.  Prints each prime to the terminal\n"
"               instead of just the last.  Does not apply if an output file\n"
"               is given.\n"
"\n"
"       If the output argument is given the entire list of prime numbers\n"
"       generated will be written as 4 byte unsigned integers to the\n"
"       specified file.\n"
"\n"
"updated: Friday, April 03, 2015\n",
                        argv[0]);
                return EXIT_SUCCESS;
                break;
            case 'v':
                verbose = true;
                break;
        }
    }

    // make sure at least one argument is given.
    if (optind >= argc){
        fprintf(stderr, "Expected number of primes.\n");
        return EXIT_FAILURE;
    }

    // parse first argument
    size_t num_primes = 0;
    if (sscanf(argv[optind++], "%zu", &num_primes) != 1){
        fprintf(stderr, "Expected integer argument for primes.\n");
    }

    // check for output file
    if (optind < argc){

        // save results to file
        if (primes_file(num_primes, argv[optind++])){
            return EXIT_FAILURE;
        } else {
            return EXIT_SUCCESS;
        }
    }

    // compute primes
    unsigned int *prime_nums = malloc(num_primes*sizeof(unsigned int));
    if (prime_nums == NULL){
        fprintf(stderr, "ERROR: Failed to allocate memory.\n");
        return EXIT_FAILURE;
    }

    // print results to screen if verbose
    if (verbose){
        primes_verbose(num_primes, prime_nums);
    } else {
        primes(num_primes, prime_nums);
    }
    free(prime_nums);

    return EXIT_SUCCESS;

}

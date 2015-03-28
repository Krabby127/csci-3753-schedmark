#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>







// algorithm taken from:
//      http://docs.cython.org/src/tutorial/cython_tutorial.html
void _primes(size_t num, unsigned int prime_nums[], bool verbose){

    unsigned int n = 2;
    size_t k = 0;
    size_t i = 0;

    if (verbose){
        printf("+----------+--------------+\n");
        printf("| #        | Prime        |\n");
        printf("+----------+--------------+\n");
    }

    for (k = 0, n = 2; k < num; ++n){

        // check against previous primes
        for (i = 0; i < k && n % prime_nums[i] != 0; ++i);

        // if prime add to list of primes
        if (i == k){
            prime_nums[k] = n;
            ++k;
            if (verbose){
                printf("| %8zu | %12d |\n", k, n);
            }
        }
    }


    if (verbose){
        printf("+----------+--------------+\n");
    }

}





// computes the first num primes and returns them in the prime_nums array
// NOTE: This function can take a long time to complete the calculation
void primes(size_t num, unsigned int prime_nums[]){
    return _primes(num, prime_nums, false);
}



// same as primes but prints each one to the terminal
void primes_verbose(size_t num, unsigned int prime_nums[]){
    return _primes(num, prime_nums, true);
}



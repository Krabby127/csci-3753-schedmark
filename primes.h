#include <stdlib.h>

#ifndef PRIMES_H
#define PRIMES_H





// computes the first n primes and returns them in the prime_nums array
// NOTE: This function can take a long time to complete the calculation
void primes(size_t num, unsigned int prime_nums[]);

// same as primes but prints each one to the terminal
void primes_verbose(size_t num, unsigned int prime_nums[]);


#endif

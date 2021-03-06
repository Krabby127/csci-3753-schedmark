/* A simple prime library used for benchmarking.
 *
 * Copyright (C) 2015  Michael R. Shannon
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */




#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


// needed for "open" and "write"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// needed for "close"
#include <unistd.h>



// algorithm taken from:
//      http://docs.cython.org/src/tutorial/cython_tutorial.html
void _primes(size_t num, unsigned int prime_nums[], bool verbose)
{

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
void primes(size_t num, unsigned int prime_nums[])
{
    return _primes(num, prime_nums, false);
}




// same as primes but prints each one to the terminal
void primes_verbose(size_t num, unsigned int prime_nums[])
{
    return _primes(num, prime_nums, true);
}


// same as primes but writes output to a file, while avoiding caching
// NOTE: The only point of this function is for mixed IO benchmarking
int primes_file(size_t num, const char *filename)
{

    // error code
    int err = 0;

    // open file for writing
    // NOTE: flags taken from provided "rw" program
    int fid = open(filename,
         O_CREAT // create file if does not exist
         | O_WRONLY // open for write only
         | O_TRUNC // erase file if it exists
         | O_SYNC // perform synchronous I/O
         , S_IRUSR // user has read permission
         | S_IWUSR // user has write permission
         | S_IRGRP // group has read permission
         | S_IWGRP // group has write permission
         | S_IROTH); // others have read permission
    if (fid < 0){
        perror("Failed to open output file");
        return -1;
    }

    // allocate temporary memory
    unsigned int *prime_nums = malloc(num*sizeof(unsigned int));
    if (prime_nums == NULL){
        fprintf(stderr, "ERROR: Failed to allocate memory.\n");
        err = -1;
        goto fail;
    }

    // initialize variables for prime computation
    unsigned int n = 2;
    size_t k = 0;
    size_t i = 0;
    size_t w = 0;

    for (k = 0, n = 2; k < num; ++n){

        // check against previous primes
        for (i = 0; i < k && n % prime_nums[i] != 0; ++i);

        // if prime add to list of primes
        if (i == k){
            prime_nums[k] = n;
            ++k;

        }

        // write to file every 128 primes to file
        if (k - w ==128){
            write(fid, &(prime_nums[w]), 128*sizeof(unsigned int));
            w += 128;
        }
    }

    // finish writing primes to file
    write(fid, &(prime_nums[w]), (k - w)*sizeof(unsigned int));


fail:

    if (close(fid)){
        perror("Failed to close input file");
    }
    free(prime_nums);
    return err;
}

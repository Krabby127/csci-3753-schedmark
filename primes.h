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

#ifndef PRIMES_H
#define PRIMES_H




// computes the first n primes and returns them in the prime_nums array
// NOTE: This function can take a long time to complete the calculation
void primes(size_t num, unsigned int prime_nums[]);

// same as primes but prints each one to the terminal
void primes_verbose(size_t num, unsigned int prime_nums[]);

// same as primes but writes output to a file, while avoiding caching
// NOTE: The only point of this function is for mixed IO benchmarking
int primes_file(size_t num, const char *filename);



#endif

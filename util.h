#include <stdio.h>
#include <math.h>
#include <gmp.h>

/*
    Check if @arg b is a primitive root of @arg a.
    @see Primitive modulo root.

    @returns boolean. (int: 1 True, int: 0 False)
*/
int checkIfPrimitiveRoot(size_t a, size_t b);

/*
    Check if @argn n is a prime number. 
    @see Prime numbers

    @return boolean. (int: 1 True, int: 0 False)
*/
int checkIfPrime(size_t n);

/*
    Euclidean algorithm for greatest common divider.
*/
size_t gcd(size_t a, size_t b);

void large_prime_generator(mpz_t prime, int digits);
void lambda_function(mpz_t lambda, mpz_t p, mpz_t q);
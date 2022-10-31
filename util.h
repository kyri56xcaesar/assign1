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
    Function to get the previous prime of a number.
    Decrement by 1 and check primality.
*/
long long int getPrevPrime(long long int p);

/*
    Euclidean algorithm for greatest common divider.
*/
size_t gcd(size_t a, size_t b);


/*
    Function to calculate the lambda number
*/
void lambda_euler_function(mpz_t lambda, mpz_t p, mpz_t q);

/*
    Function to choose a d prime number key from lambda. 
    This implementation cuts lambda in half and finds the closest prime to that half.
*/
void forge_d_key(mpz_t d, mpz_t lambda);

/*
    Simple array printing to stdout.
*/
void printArray(long long int array[], int size);
/*
    Quick sort implementation. @used for checking duplicates in a primitive roots array 
    @at method checkIfPrimitiveRoot.
*/
void quickSort(long long int array[], int low, int high);
/*
    helper function of quicksort.
*/
int partition(long long int array[], int low, int hight);
/*
    helpert function of quicksort
*/
void swap(long long int *a, long long int *b);




/*
    UNUSED functions. To be in the future!
*/
void large_prime_generator(mpz_t prime, int digits);
void lambda_function(mpz_t lambda, mpz_t p, mpz_t q);
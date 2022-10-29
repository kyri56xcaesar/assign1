#include "util.h"
#include <assert.h>
#include <gmp.h>


int checkIfPrime(size_t n);

int main(void)
{
    // Test to check if a number is indeed a primitive root.
    
    printf("PRIMITIVE ROOT TEST.\n");
    printf("-------------------------\n\n\n");
    printf("\tConfirming that 10 is a primitive root of 23...\n");

    assert(checkIfPrimitiveRoot(23, 10));

    printf("\tSuccess...\n\n\n");



    // Test to check if a number is indeed a prime.
    int n[] = {3,2,7,5,11,31,23,29};
    int i;
    int size = sizeof(n)/sizeof(n[0]);

    printf("PRIME numbers TEST\n");
    printf("-------------------------\n\n\n");
    printf("\tConfirming that array{1,2,7,5,11,31,23,29} contains prime numbers...\n");
    for (i = 0; i < size; i++)
    {
        assert(checkIfPrime(n[i])== 1);
    }
    printf("\tSuccess...\n\n\n");


    // Test to check if a number is the greatest common dividor of two numbers.


    int a = 10;
    int b = 16;

    printf("GCD TEST.\n");
    printf("-------------------------\n\n\n");
    printf("\tConfirming GCD of 10, 16 to be 2...\n");
    assert(gcd(a, b) == 2);

    printf("\tSuccess.\n\n");






    printf("LARGE PRIME TEST\n");
    printf("-------------------------\n\n\n\t");

    mpz_t prime;
    mpz_init(prime);
    mpz_set_ui(prime, 0);
    
    printf("\n\tLarge odd 100 digit number:\n\t");
    large_prime_generator(prime, 100);
    
    mpz_out_str(stdout, 10, prime);

    printf("\n\t");
    mpz_t prime2;
    mpz_init(prime2);
    mpz_set_ui(prime2, 0);

    printf("\n\tLarge odd 80 digit number:\n\t");
    large_prime_generator(prime2, 80);
    
    mpz_out_str(stdout, 10, prime2);


    mpz_clear(prime);
    mpz_clear(prime2);
    //
    printf("\n\n\n\nTEST if GCD is working properly with gmp...\n");
    printf("-------------------------\n\n\n\t");


    mpz_t a1;
    mpz_init(a1);
    mpz_set_ui(a1, 11);
    mpz_t b1;
    mpz_init(b1);
    mpz_set_ui(b1, 23);


    mpz_t gcd;
    mpz_init(gcd);
    mpz_gcd(gcd, a1, b1);
    
    printf("\n\tNumber a:\n\t");
    mpz_out_str(stdout, 10, a1);
    printf("\n\n\tNumber b:\n\t");
    mpz_out_str(stdout, 10, b1);
    printf("\n\tGCD: \t");
    mpz_out_str(stdout, 10, gcd);
    printf("\n\tgcd == 1 ?: %d\n\t",mpz_cmp_ui(gcd, 1) == 0);

   
    mpz_clear(gcd);

    printf("\n\nTEST if JACOBI is working properly with gmp...\n");
    printf("-------------------------\n\n\n\t");

    int jacobi = mpz_jacobi(a1, b1);
    mpz_t x, exp;
    mpz_init(x);
    mpz_init(exp);

    mpz_sub_ui(exp, b1, 1);
    mpz_cdiv_q_ui(exp, exp, 2);
    mpz_powm(x, a1, exp, b1);
    


    printf("\n\tNumber a:\t");
    mpz_out_str(stdout, 10, a1);
    printf("\n\n\tNumber b:\t");
    mpz_out_str(stdout, 10, b1);
    printf("\n\n\texp: \t");
    mpz_out_str(stdout, 10, exp);
    printf("\n\n\tx: \t");
    mpz_out_str(stdout, 10, x);
    printf("\n\n\tJACOBI: %d \n\t", jacobi);
    printf("\n\tx == jacobi \t :?: %d\n\t", mpz_cmp_ui(x, jacobi) == 0);

    




    mpz_clear(a1);
    mpz_clear(b1);
    mpz_clear(x);
    mpz_clear(exp);


    printf("\n\n\n-------------------\nTESTS FINISHED.\n-------------------\n\n");



}


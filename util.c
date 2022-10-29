#include <stdio.h>
#include <math.h>
#include <gmp.h>
#include <assert.h>

/*
    Check if @arg b is a primitive root of @arg a.
    @see Primitive modulo root.

    @returns boolean. (int: 1 True, int: 0 False)
*/
int checkIfPrimitiveRoot(long long int a, long long int b)
{
    int i;

    for (i = 1; i <= a; i++)
    {
        int x = fmod(pow(b, i), a);
        if (x != (int)fmod(x, a))
        {
            return 0;
        }
    }

    return 1;
}

/*
    Check if @argn n is a prime number. 
    @see Prime numbers

    This method uses 6k+1 optimization method.
    @see https://en.wikipedia.org/wiki/Primality_test

    @return boolean. (int: 1 True, int: 0 False)
*/
int checkIfPrime(size_t n)
{

    if (n <= 3)
    {   
        return n > 1? 1:0;
    }
        
    if (n % 2 == 0 || n % 3 == 0)
    {
        return 0;
    }

    int i;

    for (i = 5; i * i < n; i+=6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
    }

    return 1;
}

/*
    Euclidean algorithm for greatest common divider.
*/
size_t gcd(size_t a, size_t b)
{
    size_t t;
    while (b != 0)
    {
        t = b;
        b = fmod(a, b);
        a = t;
    }

    return a;
}

void large_prime_generator(mpz_t prime, int digits)
{

    unsigned long int x = 10;
    unsigned long int two = 2;
    unsigned long int zero = 0;


    gmp_randstate_t st;
    gmp_randinit_default(st);

    mpz_t rnd;
    mpz_init(rnd);

    
    mpz_t ten;
    mpz_init(ten);
    mpz_set_ui(ten, x);

    mpz_t mod;
    mpz_init(mod);


    fflush(stdout);
    long int i;
    for (i = 0; i < digits; i++)
    {
        // generate a random number and 
        // keep generating until number is odd.
        int r = 0;
        while(r == 0)
        {
           mpz_urandomm(rnd, st, ten);
           //printf("\n\tRand number is: ");
           //printf("\t%ld\n",mpz_out_str(stdout, 10, rnd));
           mpz_mod_ui(mod, rnd, two);
           r = mpz_cmp_ui(mod, zero);
           //printf("\n\tOdd? : %d\n",r);
            
        }

        mpz_mul_ui(prime, prime, 10);
        mpz_add(prime, prime, rnd);

        //printf("\n\tPrime so far: ");
        //mpz_out_str(stdout, 10, prime);

        //printf("\n\n");
    }

    mpz_clear(rnd);
    gmp_randclear(st);
    mpz_clear(ten);
    mpz_clear(mod);

   
}

int validate_primality(mpz_t num_b)
{
    // must pick a random number a
    // test gcd(a,b) = 1
    // test J(a,b) = a^[(b-1)/2]mod(b)

    // Setup random state /initialize
    gmp_randstate_t st;
    gmp_randinit_default(st);

    // random number a : [1, b-1]
    mpz_t rnd_a;
    mpz_init(rnd_a);
    mpz_set_ui(rnd_a, 1);

    while(mpz_cmp_ui(rnd_a, 0) == 0)
    {
        mpz_urandomm(rnd_a, st, num_b);
    }

    assert(mpz_cmp(num_b, rnd_a) > 0);

    int conditionD = 0;
    int conditionJ = 0;

    mpz_t gcd;
    mpz_init(gcd);

    mpz_gcd(gcd, rnd_a, num_b);
    conditionD = mpz_cmp_ui(gcd, 1) == 0;

    int jacobi = mpz_jacobi(rnd_a, num_b);
    mpz_t x, exp;
    mpz_init(x);
    mpz_init(exp);
    mpz_sub_ui(exp, num_b, 1);
    mpz_cdiv_q_ui(exp, exp, 2);
    mpz_powm(x, rnd_a, exp, num_b);
    conditionJ = mpz_cmp_ui(x, jacobi) == 0;



    mpz_clear(gcd);
    mpz_clear(x);
    mpz_clear(exp);
    mpz_clear(rnd_a);

    return conditionD && conditionJ;


}

void lambda_function(mpz_t l, mpz_t p, mpz_t q)
{
    // λ(n) = lcm(p - 1,q - 1)
    // lcm(a,b)=|ab|/gcd(a,b)
   
    // λ(n) = |(p-1)(q-1)| / gcd(a,b)

    // setup p - 1
    mpz_t p_1;
    mpz_init(p_1);
    mpz_sub_ui(p_1, p, 1);

    // setup q - 1
    mpz_t q_1;
    mpz_init(q_1);
    mpz_sub_ui(q_1, q, 1);
    
    // setup |(p-1)(q-1)|
    mpz_t abs_pq;
    mpz_init(abs_pq);
    mpz_mul(abs_pq, p_1, q_1);

    // setupd gcd(p-1, q-1)
    mpz_t gcd;
    mpz_init(gcd);
    mpz_gcd(gcd, p_1, q_1);


    mpz_cdiv_q(l, abs_pq, gcd);

    mpz_clear(gcd);
    mpz_clear(abs_pq);
    mpz_clear(p_1);
    mpz_clear(q_1);

}

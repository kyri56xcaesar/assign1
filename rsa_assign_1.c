#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>


/*
Key generation:
    
    1. Give from the command line 2 numbers. Let's name them p and q.
    2. Calculate if these 2 numbers are primes or not.
    3. If they are, compute n where n = p * q.
    4. Calculate lambda(n) where lambda(n) = (p - 1) * (q - 1). This is Euler’s totient
        function, described in the original RSA paper "A Method for Obtaining Digital
        Signatures and Public-Key Cryptosystems”. You may find out that other
        implementations use different totient functions. However, for this implementation,
        we are going to use Euler’s.
    5. Choose a prime e where (e % lambda(n) != 0) AND (gcd(e, lambda) == 1)
        where gcd() is the Greatest Common Denominator.
    6. Choose d where d is the modular inverse of (e, lambda).
    7. The public key consists of n and d, in this order.
    8. The private key consists of n and e, in this order.



Options:
    -i path Path to the input file
    -o path Path to the output file
    -k path Path to the key file
    -g Perform RSA key-pair generation
    -d Decrypt input and store results to output
    -e Encrypt input and store results to output
    -h This help message


*/

char *in;
char *out;
char *k;

void key_generation();

void HELP();

int main(int argv, char *argc[])
{
    if (argv < 2 || in == NULL || out == NULL || k ==NULL)
    {
        HELP();

        exit(0);
    }
  
    int i = 0;

    for (i = 1; i < argv; i++)
    {
        if (argc[i][0] == '-' && argc[i][1] == 'h')
        {
            HELP();

            exit(0);
        }
    }

    for (i = 1; i < argv - 1; i++)
    {
        if (argc[i][0] == '-')
        {

            if (argc[i][1] == 'i')
            {
                in = argc[i + 1];
            }

            if (argc[i][1] == 'o')
            {
                out = argc[i + 1];
            }

            if (argc[i][1] == 'k')
            {
                k = argc[i + 1];
            }

            if (argc[i][1] == 'g')
            {
                // perform key pair generation
                key_generation();
            }

            if (argc[i][1] == 'd')
            {
                // decrypt
            }


            if (argc[i][1] == 'e')
            {
                // encrypt.
            }

        }
           
    }

      

    return 0;
}

void key_generation()
{
    // 2 Large primes
    mpz_t p;
    mpz_t q;
    mpz_init(p);
    mpz_init(q);

    // n = p * q
    mpz_t n;
    mpz_init(n);

    // Multiplication: n = p * q
    mpz_mul(n, p, q);


    // Need to calculate lambda(n)

    // Need to choose a prime "e" where e%lambda(n) != 0 && (gcd(e, lambda) == 1)
    // Choose d : modular inverse of(e, lambda)

    // public key: (n, d)
    // private key: (n, e)


    mpz_clear(p);
    mpz_clear(q);
    
}




void HELP()
    {
        fprintf(stdout, "Options:\n\
         \t-i path Path to the input file\n\
         \t-o path Path to the outpout file\n\
         \t-k path Path to the key file\n\
         \t-g Perform RSA key-pair generation\n\
         \t-d Decrypt input and store results to output\n\
         \t-e Encrypt input and store results to output\n\
         \t-h This hellp message.\n");
    }    
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "util.h"


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

mpz_t n;
mpz_t d;
mpz_t e;

char *in;
char *out;
char *k;

void key_generation();
void encrypt();
void decrypt();

void HELP();

int main(int argv, char *argc[])
{
    if (argv < 2)
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

    for (i = 1; i < argv; i++)
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
                printf("Preparing keys...\n\n\n");
                key_generation();
                printf("Keys ready.\n");
            }

            if (argc[i][1] == 'd' && (in != NULL || out != NULL || k != NULL))
            {
                decrypt();
            }


            if (argc[i][1] == 'e' && (in != NULL || out != NULL || k != NULL))
            {
                encrypt();
            }

        }
           
    }

      

    mpz_clear(n);
    mpz_clear(d);
    mpz_clear(e);

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
    mpz_init(n);

    large_prime_generator(p, 20);
    large_prime_generator(q, 15);

    // Multiplication: n = p * q
    mpz_mul(n, p, q);

    // Need to calculate lambda(n)
    // λ(n) = lcm(p - 1,q - 1)
    // lcm(a,b)=|ab|/gcd(a,b)
    mpz_t lambda;
    mpz_init(lambda);
    lambda_function(lambda, p, q);

    // Need to choose a prime "e" where e%lambda(n) != 0 && (gcd(e, lambda) == 1)
    // 1 < e < lambda

    // @todo GENERATE e value.

    mpz_init(e);
    mpz_set_ui(e, 65537);

    // Calculate d : modular inverse of(e, lambda)


    mpz_init(d);
    mpz_powm_ui(d, e, -1, lambda);
    // public key: (n, d)
    // private key: (n, e)


    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(lambda);


    FILE *fp1;
    FILE *fp2;
    fp1 = fopen("public.key", "w");
    fp2 = fopen("private.key", "w");

    fprintf(fp1, "(");
    mpz_out_str(fp1, 16, n);
    fprintf(fp1,",");
    mpz_out_str(fp1, 16, d);
    fprintf(fp1,")");
    fflush(fp1);
    
    fprintf(fp2, "(");
    mpz_out_str(fp2, 16, n);
    fprintf(fp2,",");
    mpz_out_str(fp2, 16, e);
    fprintf(fp2,")");
    fflush(fp2);
    
    fclose(fp1);
    fclose(fp2);
    
}

void encrypt()
{

}

void decrypt()
{

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
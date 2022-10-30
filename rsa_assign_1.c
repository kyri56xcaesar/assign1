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

void destruct();
void key_generation();
void encryption();
void decryption();
char* encrypt(char *plaintext, unsigned long int puKey, unsigned long int n);
char* decrypt(char *cipher, unsigned long int prKey, unsigned long int n);

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
                if (k[1] == 'r')
                    decryption();
                else
                {
                    printf("Must obtain a private key in order to decrypt!.");

                    destruct();
                }
                

            }


            if (argc[i][1] == 'e' && (in != NULL || out != NULL || k != NULL))
            {
                if (k[1] == 'u')
                    encryption();
                else
                {
                    printf("Must obtain a public key in order to decrypt!.");

                    destruct();
                }
            }

        }
           
    }

      

    mpz_clear(n);
    mpz_clear(d);
    mpz_clear(e);

    return 0;
}

void destruct()
{
    mpz_clear(n);
    mpz_clear(d);
    mpz_clear(e);

    exit(1);
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


    // mpz_set_ui(p, 3);
    // mpz_set_ui(q, 5);
    //large_prime_generator(p, 20);
    //large_prime_generator(q, 15);

    // prompt user for primes:
    printf("Enter a prime number p: ");
    mpz_inp_str(p, stdin, 10);
    printf("\nEnter a prime number q: ");
    mpz_inp_str(q, stdin, 10);

    // Multiplication: n = p * q
    mpz_mul(n, p, q);


    // Calculate lambda euler func.
    mpz_t lambda;
    mpz_init(lambda);
    lambda_euler_function(lambda, p, q);


    // Choose an e. Prime and larger than lambda.
    mpz_init(e);   
    forge_e_iteratively(e, lambda);

  
    // Calculate d : modular inverse of(e, lambda)
    mpz_init(d);
    mpz_invert(d, e, lambda);


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

void encryption()
{
    FILE* fk;

    fk = fopen(k, "r");


    // Fetch the keys.
    char buffer[500];
    char n[200];
    char e[200];
    char ch;
    int i = 0;
    int j = 0;
    int k = 0;
    int n_flag1 = 1;
    int n_flag2 = 0;


    while ((ch = fgetc(fk)) != EOF)
    {
        if (ch != EOF)
        {
            buffer[i++] = ch;
        
          
            if (ch == ',')
            {
                n_flag1 = 0;
                n_flag2 = 1;
            }
            else if (ch == ')')
            {
                n_flag1 = 0;
                n_flag2 = 0;
            }
            if (n_flag1 && ch != '(')
            {
                n[j++] = ch;
            }   
            if (n_flag2)
            {
                e[k++] = ch;
            } 
        }
    }

    // print keys
    printf("K:\n%s\nN:\n%s\nE:\n%s\n", buffer, n, e);
    fclose(fk);


    // Fetch the plain_text from input.
    FILE* fin;

    fin = fopen(in, "r");
    char plaintext[200];
    char c;
    i = 0;

    while ((c = fgetc(fin)) != EOF)
    {
        if (c != EOF)
        {
            plaintext[i++] = c;
        }
    }

    // print plaintext
    printf("PLAINTEXT:\n%s", plaintext);
    fclose(fin);


    // encrypt.
    //char* cipher = encrypt();


    FILE* fout;
    fout = fopen(out, "w");
    
    // print cipher
    //printf("CIPHER:\n%s", cipher);

    //fprintf(fout, "%s", cipher);

    //free(cipher);

    fflush(stdout);
    fclose(fout);
}

char* encrypt(char *plaintext, unsigned long int puKey, unsigned long int n)
{
    int size = sizeof(plaintext)/sizeof(plaintext[0]);
    int i;

    char cipher[size];

    for (i = 0; i < size; i ++)
    {
        cipher[i] = plaintext[i]^puKey % n;
    }

    return cipher;
}
char* decrypt(char *cipher, unsigned long int prKey, unsigned long int n)
{
    int size = sizeof(cipher)/sizeof(cipher[0]);
    int i;

    char plaintext[size];

    for (i = 0; i < size; i ++)
    {
        plaintext[i] = cipher[i]^prKey % n;
    }

    return cipher;
}
void decryption()
{FILE* fk;

    fk = fopen(k, "r");


    // Fetch the keys.
    char buffer[512];
    char n[256];
    char d[256];
    char ch;
    int i = 0;
    int j = 0;
    int k = 0;
    int n_flag1 = 1;
    int n_flag2 = 0;


    while ((ch = fgetc(fk)) != EOF)
    {
        if (ch != EOF)
        {
            buffer[i++] = ch;
        
          
            if (ch == ',')
            {
                n_flag1 = 0;
                n_flag2 = 1;
            }
            else if (ch == ')')
            {
                n_flag1 = 0;
                n_flag2 = 0;
            }
            if (n_flag1 && ch != '(')
            {
                n[j++] = ch;
            }   
            if (n_flag2)
            {
                d[k++] = ch;
            } 
        }
    }

    // print keys
    printf("K:\n%s\nN:\n%s\nE:\n%s\n", buffer, n, d);
    fclose(fk);


    // Fetch the plain_text from input.
    FILE* fin;

    fin = fopen(in, "r");
    char cipher[200];
    char c;
    i = 0;

    while ((c = fgetc(fin)) != EOF)
    {
        if (c != EOF)
        {
            cipher[i++] = c;
        }
    }

    // print plaintext
    printf("PLAINTEXT:\n%s", cipher);
    fclose(fin);


    // encrypt.
    //char* plaintext = decrypt();


    FILE* fout;
    fout = fopen(out, "w");
    
    // print cipher
    //printf("CIPHER:\n%s", plaintext);

    //fprintf(fout, "%s", plaintext);

    //free(plaintext);

    fflush(stdout);
    fclose(fout);
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
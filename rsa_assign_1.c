#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "util.h"
#include <inttypes.h>


#define MAX_BUFFER 512
#define MAX_CIPHER 4096

long long int cipher_byte_size=0;

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

void print_cipher(size_t *cihper, int size);
void write_cipher(size_t *cipher, int size, char *out);
void destruct();
void key_generation();
void encryption();
void decryption();
size_t* encrypt(size_t* cipher, char *plaintext, size_t size, mpz_t puKey, mpz_t n);
char* decrypt(size_t *cipher, size_t size, mpz_t prKey, mpz_t n);

/*
    Helper function. Prints -h menu
*/
void HELP();

/*
    Configure arguments.
 */
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
                    printf("Must obtain a private key in order to decrypt!.\n");

                    destruct();
                }
                

            }


            if (argc[i][1] == 'e' && (in != NULL || out != NULL || k != NULL))
            {
                if (k[1] == 'u')
                    encryption();
                else
                {
                    printf("Must obtain a public key in order to ecnrypt!.\n");

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

/*
    Exit  function. Frees memory before exiting with error.
*/
void destruct()
{
    mpz_clear(n);
    mpz_clear(d);
    mpz_clear(e);

    exit(1);
}


/*
    Function that will generate all the necessary keys and values for RSA encryption.

    Called upon -g
*/
void key_generation()
{
    // 2 Large primes
    mpz_t p;
    mpz_t q;
    mpz_init(p);
    mpz_init(q);

    // n = p * q
    mpz_init(n);

    //large_prime_generator(p, 20);
    //large_prime_generator(q, 15);

    // prompt user for primes:
    int cs =0;
    while(cs == 0)
    {
        printf("Enter a prime number p: ");
        mpz_inp_str(p, stdin, 10);
    
        cs = mpz_probab_prime_p(p, 20);
        if (cs ==0)
        {
            printf("\nNot a prime. Pick again.\n");
        }
    }
    cs = 0;
    while(cs == 0)
    {
     
        printf("\nEnter a prime number q: ");
        mpz_inp_str(q, stdin, 10);   
        cs = mpz_probab_prime_p(q, 20);
        if (cs == 0)
        {
            printf("\nNot a prime. Pick again");
        }
    }
    printf("\n");

    // Multiplication: n = p * q
    mpz_mul(n, p, q);

    printf("\nn: ");
    mpz_out_str(stdout, 10, n);
    printf("\n");


    // Calculate lambda euler func.
    mpz_t lambda;
    mpz_init(lambda);
    lambda_euler_function(lambda, p, q);

    printf("\nλ(n)= :");
    mpz_out_str(stdout, 10, lambda);


    // Choose an e. Prime and larger than lambda.
    mpz_init(d);   
    forge_d_iteratively(d, lambda);
    printf("\nd: ");
    mpz_out_str(stdout, 10, d);
    printf("\n");
  
    // Calculate d : modular inverse of(e, lambda)
    mpz_init(e);
    mpz_invert(e, d, lambda);
    printf("\ne: ");
    mpz_out_str(stdout, 10, e);
    printf("\n");

    // public key: (n, d)
    // private key: (n, e)


    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(lambda);


    FILE *fp1;
    FILE *fp2;
    fp1 = fopen("public.key", "w");
    fp2 = fopen("private.key", "w");
    if (fp1 == NULL|| fp2 == NULL)
    {
        printf("Error opening a file. Program will now terminate...\n");

        exit(1);
    }

    fprintf(fp1, "(");
    mpz_out_str(fp1, 10, n);
    fprintf(fp1,",");
    mpz_out_str(fp1, 10, e);
    fprintf(fp1,")");
    fflush(fp1);
    
    fprintf(fp2, "(");
    mpz_out_str(fp2, 10, n);
    fprintf(fp2,",");
    mpz_out_str(fp2, 10, d);
    fprintf(fp2,")");
    fflush(fp2);
    
    fclose(fp1);
    fclose(fp2);
    
}


/*
    Function to configure encryption mechanism of a plain text.
*/
void encryption()
{
    FILE* fk;

    fk = fopen(k, "r");
    if (fk == NULL)
    {
        printf("Error opening file. Program will exit...\n");

        exit(1);
    }


    // Fetch the keys.
    char buffer[MAX_BUFFER];
    char *n;
    char *e;
    char ch;
    int i = 0;
    int j = 0;
    int k = 0;


    while ((ch = fgetc(fk)) != EOF)
    {
        if (ch != EOF)
        {
            buffer[i++] = ch;

            if (ch == ',')
            {
                n = (char*)malloc(sizeof(char)*i);

                int c = 0;
                for (j = 0; j < i - 1; j++)
                {
                    if (buffer[j] != '(' && buffer[j] != ',')
                    {
                        n[c++] = buffer[j];
                    }
                }

            }
            else if (ch == ')')
            {
                e = (char*)malloc(sizeof(char)*(i-j));
                
                int b = 0;
                for (k = 0; k < i - j; k++)
                {
                    if (buffer[j + k] != ',' && buffer[j + k] != ')')
                    {
                        e[b++] = buffer[j + k];
                    }
                }

                break;
            }
        }
    }

    char *keys = (char*) malloc(sizeof(char)*i);
    j=0;
    for (j = 0; j < i; j++)
    {
        keys[j] = buffer[j];
    }


    // print keys
    printf("keys: %s\nn: %s\ne: %s\n", keys, n, e);
    fclose(fk);

    free(keys); 
    remove(buffer);


    // Fetch the plain_text from input.
    FILE* fin;

    fin = fopen(in, "r");
    if (fin == NULL)
    {
        printf("Error opening file. Program will exit...\n");

        exit(1);
    }


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
    i--;

    int size = i;

    // print plaintext
    printf("PLAINTEXT: %s", plaintext);
    fclose(fin);

    // create mpz_t numbers;
    mpz_t ekey;
    mpz_t nkey;

    mpz_init(ekey);
    mpz_init(nkey);

    mpz_set_str(ekey, e, 10);
    mpz_set_str(nkey, n, 10);

    size_t* cipher = (size_t*)malloc(sizeof(size_t)*size);
    printf("size:%d %ld\n", size, sizeof(size_t)*size);
    // encrypt.
    cipher = encrypt(cipher, plaintext, size, ekey, nkey);


    //printf("%ld\n", sizeof(cipher[0])+sizeof(cipher[1])+sizeof(cipher[2])+sizeof(cipher[3]));

   
    
    // print cipher
    printf("CIPHER: ");
    print_cipher(cipher, size);

    // write cipher to file
    //fprintf(fout, "%s", cipher);
    //write_cipher(cipher, size, out);


    mpz_clear(nkey);
    mpz_clear(ekey);
    free(cipher);
    free(e);
    free(n);
    fflush(stdout);
 
}

/*
    Helper function in order to print a size_t cipher in stdout.
*/
void print_cipher(size_t *cipher, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("%"PRIu64" ", (uint64_t)cipher[i]);
    }

    printf("\n");
}

/*
    Helper function in order to write a size_t in a file.
*/
void write_cipher(size_t *cipher, int size, char *file)
{
    FILE *fp;

    fp = fopen(file, "wb");
    if (fp == NULL)
    {
        printf("Error opening file. Program will exit...\n");

        exit(1);
    }

    int i;
    cipher_byte_size = size * 8;

    // zero pad

    
    // store as bytes
    //fwrite(cipher, sizeof(size_t), size, fp);
    
    for (i = 0; i < size; i++)
    {
        //fwrite(cipher, sizeof(size_t), size, fp);
        fprintf(fp, "%lu\n", (uint64_t)cipher[i]);
    }
}

size_t* encrypt(size_t* cipher, char *plaintext, int size, mpz_t puKey, mpz_t n)
{
    
    int i; 
    mpz_t ch;
    mpz_init(ch);
    mpz_t powm;
    mpz_init(powm);


 
    

    int dummy;

    for (i = 0; i < size; i ++)
    {
        mpz_set_ui(ch, plaintext[i]);
        mpz_powm(powm, ch, puKey, n);
        
        //cipher[i] = (uint64_t)mpz_get_ui(powm);
        mpz_export(cipher, NULL, 1, 8, 0, 0, powm);

        printf("Prompt to wait. continue?\n");
        scanf("%d", &dummy);

    }

    mpz_clear(ch);
    mpz_clear(powm);

    return cipher;
}

char* decrypt(size_t *cipher, size_t size, mpz_t prKey, mpz_t n)
{
    if (cipher == NULL)
    {
        return NULL;
    }

    int i;

    char* plaintext = (char*)malloc(sizeof(char) *size/8);
   
    mpz_t ch;
    mpz_init(ch);
    mpz_t powm;
    mpz_init(powm);


    for (i = 0; i < size; i ++)
    {
        mpz_set_ui(ch, cipher[i]);
        mpz_powm(powm, ch, prKey, n);
        plaintext[i] = mpz_get_ui(powm);
    }

    mpz_clear(ch);
    mpz_clear(powm);

    return plaintext;
}
void decryption()
{
    FILE* fk;

    fk = fopen(k, "r");
    if (fk == NULL)
    {
        printf("Error opening file. Program will exit...\n");

        exit(1);
    }


    // Fetch the keys.
    char buffer[MAX_BUFFER];
    char *n;
    char *d;
    char ch;
    int i = 0;
    int j = 0;
    int k = 0;


    while ((ch = fgetc(fk)) != EOF)
    {
        if (ch != EOF)
        {
            buffer[i++] = ch;

            if (ch == ',')
            {
                n = (char*)malloc(sizeof(char)*i);

                int c = 0;
                for (j = 0; j < i - 1; j++)
                {
                    if (buffer[j] != '(' && buffer[j] != ',')
                    {
                        n[c++] = buffer[j];
                    }
                }

            }
            else if (ch == ')')
            {
                d = (char*)malloc(sizeof(char)*(i-j));
                
                int b = 0;
                for (k = 0; k < i - j; k++)
                {
                    if (buffer[j + k] != ',' && buffer[j + k] != ')')
                    {
                        d[b++] = buffer[j + k];
                    }
                }

                break;
            }
        }
    }

    char *keys = (char*) malloc(sizeof(char)*i);
    j=0;
    for (j = 0; j < i; j++)
    {
        keys[j] = buffer[j];
    }
    // print keys
    printf("keys: %s\nn: %s\ne: %s\n", keys, n, d);
    fclose(fk);

    free(keys); 
    remove(buffer);

    // Fetch the cipher from input.
    FILE* fin;

    fin = fopen(in, "r");
    if (fin == NULL)
    {
        printf("Error opening file. Program will exit...\n");

        exit(1);
    }

    size_t cipher[cipher_byte_size];
    fseek(fin, 0, SEEK_SET);

    fread(cipher, sizeof(size_t), cipher_byte_size/8, fin);
    
    fclose(fin);


    printf("CIPHER: ");
    print_cipher(cipher, i);

    // create mpz_t numbers;
    mpz_t dkey;
    mpz_t nkey;

    mpz_init(dkey);
    mpz_init(nkey);

    mpz_set_str(dkey, d, 10);
    mpz_set_str(nkey, n, 10);

    // encrypt.
    char* plaintext = decrypt(cipher, i, dkey, nkey);


    FILE* fout;
    fout = fopen(out, "w");
    if (fout == NULL)
    {
        printf("Error opening file. Program will exit...\n");

        exit(1);
    }
    
    // print cipher
    printf("PLAINTEXT:%s\n", plaintext);

    fprintf(fout, "%s", plaintext);


    mpz_clear(nkey);
    mpz_clear(dkey);
    free(plaintext);
    free(d);
    free(n);
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
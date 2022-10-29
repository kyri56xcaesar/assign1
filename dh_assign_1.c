#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"

/*
 * Prime numbers p and g (g previous prime from p)
 *
 * a : secret integer a<p
 * b : secret integer b<p
 * 
 * A = g^a mod p
 * B = g^b mod p
 * 
 * s = B^a mod p
 * s = A^b mod p
 


    Options:
     -o path Path to outpout file
     -p number Prime number
     -g number Primitive Root for previous prime number
     -a number Private key A
     -b number Private key B
     -h This hellp message.
*/

long long int p = 0;
long long int g = 0;;
long long int a,b;
char *output;

long long int A;
long long int B;
long long int KEY;


long long int createPublicKey(long long int key);
long long int discoverSecret(long long int pKey, long long int sKey);

void HELP();
void printData();
void print_Data(FILE *fp, char *filename);

int main(int argv, char* argc[])
{

    if (argv != 11)
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

            if (argc[i][1] == 'o')
            {
                output = argc[i + 1];
            }

            if (argc[i][1] == 'g')
            {
                g = atof(argc[i + 1]);
            }

            if (argc[i][1] == 'p')
            {
                p = atof(argc[i + 1]);

                if (!checkIfPrimitiveRoot(p, g))
                {
                    printf("False input. P is not a primitive root of G\n");

                    exit(1);
                }
            }


            if (argc[i][1] == 'a')
            {
                a = atof(argc[i + 1]);
                if (a >= p)
                {
                    printf("Invalid secret integer.\nMust be less than the prime root p.\n");
                    exit(1);
                }
            }


            if (argc[i][1] == 'b')
            {
                b = atof(argc[i + 1]);
                if (b >= p)
                {
                    printf("Invalid secret integer.\nMust be less than the prime root p.\n");
                    exit(1);
                }
            }
        }
    }

    //printData();

    A = createPublicKey(a);
    B = createPublicKey(b);
    KEY = discoverSecret(A, b);

    // check
    if (discoverSecret(B, a) != discoverSecret(A, b))
    {
        printf("Error...\n");
        //exit(1);
    }


    FILE *fp;
    if ((fp = fopen(output, "w")) == NULL)
    {
        fprintf(stdout, "Error opening file.\n");

        exit(1);
    }


    fprintf(fp, "<%lld>,<%lld>,<%lld>", A, B, KEY);

    fflush(fp);

    print_Data(fp, output);

    

    fclose(fp);


    return 0;
}



long long int discoverSecret(long long int pKey, long long int sKey)
{
    return fmod(pow(pKey, sKey), p);
}
long long int createPublicKey(long long int sKey)
{
    return fmod(pow(g, sKey), p);
}


void HELP(){
    fprintf(stdout, "Options:\n\
     \t-o path Path to outpout file\n\
     \t-p number Prime number\n\
     \t-g number Primitive Root for previous prime number\n\
     \t-a number Private key A\n\
     \t-b number Private key B\n\
     \t-h This hellp message.\n");
}

void printData()
{
    fprintf(stdout, "o: %s\np:%lld\ng:%lld\na:%lld\nb:%lld\n", output, p, g, a, b);
}

void print_Data(FILE *fp, char *filename)
{
    printf("\n");

    if ((fp = fopen(filename, "r")) == NULL)
    {
        fprintf(stdout, "Error opening file.\n");

        exit(1);
    }
    rewind(fp);


    char ch;
    
    char buffer[100];
    int i=0;
    
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch != EOF)
        {
            buffer[i++] = ch;
        }
    }
    
    printf("%s\n", buffer);

    printf("\n");

    fclose(fp);

}
#include <stdio.h>
#include <stdlib.h>

char *in, out, k;
double g, d, e;

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
                g = atof(argc[i + 1]);
            }

            if (argc[i][1] == 'd')
            {
                d = atof(argc[i + 1]);
            }


            if (argc[i][1] == 'e')
            {
                e = atof(argc[i + 1]);
            }

        }
           
    }

      

    return 0;
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
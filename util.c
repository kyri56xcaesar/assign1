#include <stdio.h>
#include <math.h>
#include <gmp.h>
#include <assert.h>
#include <stdlib.h>


//QUICK SORT

// function to print array elements
void printArray(long long int array[], int size) {
  for (int i = 0; i < size; ++i) {
    printf("%lld ", array[i]);
  }
  printf("\n");
}



// function to swap elements
void swap(long long int *a, long long int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

// function to find the partition position
int partition(long long int array[], int low, int high) {
  
  // select the rightmost element as pivot
  long long int pivot = array[high];
  
  // pointer for greater element
  int i = (low - 1);

    int j;
  // traverse each element of the array
  // compare them with the pivot
  for (j = low; j < high; j++) {
    if (array[j] <= pivot) {
        
      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;
      
      // swap element at i with element at j
      swap(&array[i], &array[j]);
    }
  }

  // swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);
  
  // return the partition point
  return (i + 1);
}

void quickSort(long long int array[], int low, int high) {
  if (low < high) {
    
    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on right of pivot
    int pi = partition(array, low, high);
    
    // recursive call on the left of pivot
    quickSort(array, low, pi - 1);
    
    // recursive call on the right of pivot
    quickSort(array, pi + 1, high);
  }
}



/*
    Check if @arg b is a primitive root of @arg a.
    @see Primitive modulo root.

    A number x is a primitive root of a number y if : x^1 mody , x^2 mod y,... , x^(y-1)mod y are distinct.

    @returns boolean. (int: 1 True, int: 0 False)
*/
int checkIfPrimitiveRoot(long long int a, long long int b)
{
    int i;

    long long int *prev = (long long int*)malloc(sizeof(long long int) * a-1);

    // populate with prevs of prime
    for (i = 0; i < a-1; i++)
    {   
        prev[i] = fmod(pow(b, i), a);

    }
    //printf("%d\n",i);

    // check for equality. establish distinction
    // can sort and iterate once O(nlogn) + O(n)
    //printArray(prev, a-1);
    quickSort(prev, 0, a-1);
    //printArray(prev, a-1);

    for (i = 0; i < a-1; i++)
    {
        if (prev[i] == prev[i+1])
        {
            return 0;
        }
    }

    free(prev);
 
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
    get previous prime number by linearly decreasing by 1 and checking...
*/

long long int getPrevPrime(long long int p)
{
    long long int i;

    for (i = p-1; i > 0; i--)
    {
        if (checkIfPrime(i))
        {
            return i;
        }
    }

    return 0;
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


/*
    Lambda EULER FUNCTION used in this implementation.
*/
void lambda_euler_function(mpz_t l, mpz_t p, mpz_t q)
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
    mpz_mul(l, p_1, q_1);


    mpz_clear(p_1);
    mpz_clear(q_1);

}


/*
    Function to get a d key out of lambda. MUST BE prime
    In this simple case its cutting in half the lambda number and its finding the closest prime
    to that half.

    WORKS with mpz_t numbers.
*/
void forge_d_key(mpz_t d, mpz_t lambda)
{
    // Searching for a large number d < lambda which is relatively prime to lambda
    // tmp value to hold d
    mpz_t tmp;
    mpz_init(tmp);
    
    // Prerquisites are d % lambda != 0 && gcd(d, lambda)==1 
    // d mod lambda
    mpz_t mod;
    mpz_init(mod);

    // gcd (d, lambda)
    mpz_t gcd;
    mpz_init(gcd);



    // iterator index. Iterate max lambda times!
    unsigned long int i;

    // Recieve a prime double of lambda.
    mpz_t op;
    mpz_init(op);


    mpz_cdiv_q_ui(op, lambda, 7);
  

    
    for (i = 1;  mpz_cmp_ui(lambda, i) > 0 ;i++)
    {
        // get a larger prime
        mpz_nextprime(tmp, op);

        // evaluate mod
        mpz_mod(mod, tmp, lambda);
   
        // evaluate gcd
        mpz_gcd(gcd, tmp, lambda);
   
        // check conditions
        int mod_is_not_zero = mpz_cmp_ui(mod, 0);
        int gcd_is_one = mpz_cmp_ui(gcd, 1);

        // if conditions are met set e and return
        if (mod_is_not_zero != 0 &&  gcd_is_one== 0)
        {

            mpz_set(d, tmp);
       
            break;
        }
        // If conditions are not met
        // set operator to be current tmp in order to get a next prime
        mpz_set(op, tmp);
    

    }
    // Clear data.
    mpz_clear(tmp);
    mpz_clear(mod);
    mpz_clear(gcd);
    mpz_clear(op);
}

/*
    @UNUSED... TO BE COMPLETE

    Create a large prime by concatenating random odd numbers.
    Validate primality with primality propabilistic methods.
*/
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


/*
    @UNUSED --> TO BE COMPLETE

    an effort to create a primality propabilistic validation method.
*/
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

/*
    Lambda function of 2 mpz_t numbers as expressed below.
    This is not used yet.
*/
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





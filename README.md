gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0

**Makefile included**
>Use "make" to compile the programms.


    :dh_assign_1
    :rsa_assign_1
    :unit_testing

>Use "make clean" to delete everything useless and fresh start.


After compilation: 

-- ./dh_assign_1 -h for further assistance
-- ./rsa_assign_1 -h for further assistance
-- ./unit_testing for some specific cases: BUGGY. *NEEDS ATTENTION*



----> NOTE that everything is connected via files. Printing to console has been commented. Yet available to be uncommented.
---- FOR RSA TOOL. YOU MUST CREATE AN INPUT FILE WITH YOUR PLAINTEXT.
---- input.txt => "ITS ALL GREEK TO ME" may be provided as demo.


** DEPENDENCIES **
util.h has been featured to include specific all in all functions to assist the development of dh and rsa encryption algos.

*WARNING* its not all complete. Some steps and functions may not be used, experimental cases or to be attended for further development and improvements.
For example:
    Primes in this tool are meant to be given by the user or fixed at. There are some functions in util.h that are for random generation of large primes. (They don't work propery so far[almost] but are to be seen in the future!)



-------------- **DIFFIE-HELLMAN** --------------

Arguments as numbers and path are given by user, validated by the program.
P must be a prime and G a Primitive Root of P.
private keys A and B are whatever.

output file is: <Public key A>,<Public key B>,<shared secret key>

------------------- **RSA** --------------------

RSA encryption Implementation. USES GMP library to accomodate large numbers and their arithmetic.

Functions of GMP are used to encrypt and decrypt with mathematical precision.

input/ouput are saved in files with the corresponding values 8 bytes cipher per 1 byte plaintext.
In this case specifically mpz_export and mpz_import are used to accomodate this feature.


> d and q  keys (prime numbers) are prompted in the command prompt until they are indeed primes.

> e key is somewhat inaccurately chosed as the next prime of the (q-1)(d-1) product. 
**TO BE UPDATED**


For key generation, paths must not be provided.
Before encryption or decryption options. Input output and key paths must be provided. 



@ENCRYPTION DECRYPTION work properly as expected.


**minor bug**

-- FIXED







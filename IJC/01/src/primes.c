/*
 * ======================================================== *
 * Name:        primes.c                                    *
 * Author:      RADIM SAFAR - FIT VUT                       *
 * Email:       xsafar27(at)stud.fit.vutbr.cz               *
 * Last modif:  03/22/2022                                  *
 * ======================================================== *
 */

#include "../headers/bitset.h"
#include "../headers/error.h"
#include "../headers/eratosthenes.h"
#include <stdio.h>
#include <time.h>

// Number where the algorithm stops
#define AMOUNT 300000000

int main(void)
{
    clock_t start = clock();

    bitset_create(bitset, AMOUNT);
    eratosthenes(bitset);

    int count = 0;
    int i;

    // Looks for the 10th prime from the end
    for(i = AMOUNT - 1; count < 10; i--){
        ul bit = bitset_getbit(bitset, i);
        if(bit == 0){
            count++;
        }
    }

    // Prints last 10 primes in ascending order
    count = 0;
    for(;count < 10; i++){
        ul bit = bitset_getbit(bitset, i);
        if(bit == 0){
            count ++;
            printf("%d\n", i);
        }
    }

    // Prints time it took the program to run
    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);
}
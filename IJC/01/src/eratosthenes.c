/*
 * ======================================================== *
 * Name:        eratosthenes.c                              *
 * Author:      RADIM SAFAR - FIT VUT                       *
 * Email:       xsafar27(at)stud.fit.vutbr.cz               *
 * Last modif:  03/22/2022                                  *
 * ======================================================== *
 */

#include <math.h>
#include "../headers/bitset.h"
#include "../headers/error.h"
#include "../headers/eratosthenes.h"

void eratosthenes(bitset_t bitset)
{
    ul size = bitset_size(bitset);
    double sq_size = sqrt(size);

    bitset_setbit(bitset, 0, 1);
    bitset_setbit(bitset, 1, 1);

    for(ul i = 2; i < size; i += 2){
        bitset_setbit(bitset, i, 1);
    }

    for(ul i = 3; i < sq_size; i++){
        if ((bitset_getbit(bitset, i)) != 1){
            for (ul j = i*i; j < size; j += 2 * i){
                bitset_setbit(bitset, j, 1);
            }
        }
    }
}
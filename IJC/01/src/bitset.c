/*
 * ======================================================== *
 * Name:        bitset.c                                    *
 * Author:      RADIM SAFAR - FIT VUT                       *
 * Email:       xsafar27(at)stud.fit.vutbr.cz               *
 * Last modif:  03/16/2022                                  *
 * ======================================================== *
 */

#include "../headers/bitset.h"

#ifdef USE_INLINE

extern inline void bitset_free(bitset_t jmeno_pole);

extern inline ul bitset_size(bitset_t jmeno_pole);

extern inline void bitset_setbit(bitset_t jmeno_pole, bitset_index_t index, int value);
    
extern inline ul bitset_getbit(bitset_t jmeno_pole, bitset_index_t index);

#endif // end USE_INLINE
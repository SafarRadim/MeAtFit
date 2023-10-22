/*
 * ======================================================== *
 * Name:       htab_structs.h                               *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/18/2022                                   *
 * ======================================================== *
 */


#ifndef HTAB_STRUCTS
#define HTAB_STRUCTS

#include "htab.h"

typedef struct htab_item {
    htab_pair_t *pair;
    struct htab_item *next;
} htab_item;

typedef struct htab {
    int size;
    int arr_size;
    htab_item **arr_ptr;
} htab_t;

#endif // HTAB_STRUCTS

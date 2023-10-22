/*
 * ======================================================== *
 * Name:       htab_init.c                                  *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/18/2022                                   *
 * ======================================================== *
 */

#include <stdlib.h>
#include <stdio.h>

#include "htab.h"
#include "htab_structs.h"

htab_t *htab_init(size_t n)
{
    htab_t *htab = (htab_t*) malloc(sizeof(htab_t));
    htab_item **items = (htab_item**) malloc(n * sizeof(htab_item));

    if(items == NULL){
        fprintf(stderr, "Failed to malloc items in htab_init.c");
        return NULL;
    }

    if(htab == NULL){
        fprintf(stderr, "Failed to malloc htab in htab_init.c");
        return NULL;
    }

    htab->arr_ptr = items;
    
    if (htab == NULL){
        return NULL;
    }

    for(size_t i = 0; i < n; i++){
        htab->arr_ptr[i] = NULL;    // easier foreach if all is default null
    }

    htab->arr_size = n;
    htab->size = 0;

    return htab;
}
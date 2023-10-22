/*
 * ======================================================== *
 * Name:       htab_resize.c                                *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/19/2022                                   *
 * ======================================================== *
 */
#include <stdio.h>

#include "htab.h"
#include "htab_structs.h"

void htab_resize(htab_t *t, size_t newn)
{
    htab_t *newTab = htab_init(newn);
    if(newTab == NULL){
        return;
    }
    
    size_t htabSize_old = htab_bucket_count(t);
    htab_item *currentPtr;

    for(size_t i = 0; i < htabSize_old; i++){
        currentPtr = t->arr_ptr[i];
        while(currentPtr){
            htab_pair_t *check = htab_lookup_add(newTab, currentPtr->pair->key);
            if(check == NULL){
                return;
            }
            currentPtr = currentPtr->next;
        }
    }

    t = newTab;
}
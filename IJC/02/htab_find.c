/*
 * ======================================================== *
 * Name:       htab_find.c                                  *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/19/2022                                   *
 * ======================================================== *
 */

#include "htab.h"
#include "htab_structs.h"

htab_pair_t *htab_find(htab_t *t, htab_key_t key)
{
    size_t index = htab_hash_function(key) % htab_bucket_count(t);

    htab_item *currentPtr = t->arr_ptr[index];
    while(currentPtr){
        if(currentPtr->pair->key == key){
            return currentPtr->pair;
        }
        currentPtr = currentPtr->next;
    }
    return NULL;
}
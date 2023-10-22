/*
 * ======================================================== *
 * Name:       htab_clear.c                                 *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/19/2022                                   *
 * ======================================================== *
 */

#include <stdlib.h>

#include "htab.h"
#include "htab_structs.h"

void htab_clear(htab_t *t)
{
    for(int i = 0; i < t->arr_size; i++){
        if (t->arr_ptr[i] == NULL){
            continue;
        }

        htab_item *current_ptr = t->arr_ptr[i];
        htab_item *to_remove;

        while(current_ptr){
            to_remove = current_ptr;
            current_ptr = current_ptr->next;
            free((void* const)to_remove->pair->key);
            free(to_remove->pair);
            free(to_remove);
        }

        t->arr_ptr[i] = NULL;
    }
    t->size = 0;
    free(t->arr_ptr);
}
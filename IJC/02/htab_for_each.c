/*
 * ======================================================== *
 * Name:       htab_for_each.c                              *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/18/2022                                   *
 * ======================================================== *
 */

#include <stdio.h>

#include "htab.h"
#include "htab_structs.h"

void htab_for_each(const htab_t *t, void (*f)(htab_pair_t *data))
{
    for(int i = 0; i < t->arr_size; i++){
        if (t->arr_ptr[i] == NULL){
            continue;
        }

        htab_item *current_ptr = t->arr_ptr[i];

        while(current_ptr){
            f(current_ptr->pair);
            current_ptr = current_ptr->next;
        }
    }
}
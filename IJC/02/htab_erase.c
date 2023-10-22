/*
 * ======================================================== *
 * Name:       htab_erase.c                                 *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/19/2022                                   *
 * ======================================================== *
 */

#include <stdlib.h>

#include "htab.h"
#include "htab_structs.h"

#define AVG_MIN 3

bool htab_erase(htab_t *t, htab_key_t key){
    size_t index = htab_hash_function(key) % htab_bucket_count(t);
    
    htab_item *currentPtr = t->arr_ptr[index];
    htab_item *secondaryPtr;
    while(currentPtr){
        if(currentPtr->pair->key == key){
            secondaryPtr = currentPtr;
            currentPtr = currentPtr->next;
            secondaryPtr->next = currentPtr->next;
            free(currentPtr->pair);
            free(currentPtr);

            int tmp = 0;
            int sum = 0;
            htab_item *currentPtr;

            for(int i = 0; i < t->arr_size; i++){
                if(t->arr_ptr[i] == NULL){
                    continue;
                }
                currentPtr = t->arr_ptr[i];
                while(currentPtr){
                    sum++;
                    currentPtr = currentPtr->next;
                }
                tmp++;
            }

            if (sum/tmp < AVG_MIN){
                htab_resize(t, t->arr_size/2);
                return htab_find(t, key);
            }

            return true;
        }
        currentPtr = currentPtr->next;
    }
    return false;
}
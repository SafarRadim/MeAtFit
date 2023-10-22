/*
 * ======================================================== *
 * Name:       htab_lookup_add.c                            *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/19/2022                                   *
 * ======================================================== *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "htab.h"
#include "htab_structs.h"

#define AVG_MAX 5

htab_pair_t *htab_lookup_add(htab_t *t, htab_key_t key)
{
    size_t index = htab_hash_function(key) % htab_bucket_count(t);

    htab_item *ptr = t->arr_ptr[index];
    
    while(ptr){
        if(!strcmp(ptr->pair->key, key)){
            return ptr->pair;
        }
        ptr = ptr->next;
    }
    
    if(ptr == NULL){
        size_t index = htab_hash_function(key) % htab_bucket_count(t);
        ptr = t->arr_ptr[index];

        htab_pair_t *pair = (htab_pair_t*) malloc (sizeof(htab_pair_t));
        htab_item *item = (htab_item*) malloc (sizeof(htab_item));
        pair->key = malloc(sizeof(htab_key_t));

        if(pair == NULL){
            fprintf(stderr, "Failed to malloc pair in htab_lookup_add.c");
            return NULL;
        }

        if(item == NULL){
            fprintf(stderr, "Failed to malloc item in htab_lookup_add.c");
            return NULL;
        }

        if(pair->key == NULL){
            fprintf(stderr, "Failed to malloc pair->key in htab_lookup_add.c");
            return NULL;
        }

        memcpy((void * restrict)pair->key, key, strlen(key));
        pair->value = 0;
        item->pair = pair;

        if (ptr == NULL){
            t->arr_ptr[index] = item;
        } else {
            while(ptr->next){
                ptr = ptr->next;
            }
            ptr->next = item;
        }

        t->size++;
        ptr = item;
    }

    int sum = 0;
    int amn = 0;
    htab_item *currentPtr = NULL;

    for(int i = 0; i < t->arr_size; i++){
        if (t->arr_ptr[i] == NULL) { continue; }
        amn++;
        currentPtr=t->arr_ptr[i];
        while(currentPtr){
            sum++;
            currentPtr = currentPtr->next;
        }
    }
    if (sum/amn > AVG_MAX){
        htab_resize(t, t->arr_size * 2);
    }

    return ptr->pair;
}
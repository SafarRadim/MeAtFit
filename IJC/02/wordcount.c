/*
 * ======================================================== *
 * Name:       wordcount.c                                  *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/18/2022                                   *
 * ======================================================== *
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "htab.h"
#include "io.h"

#define MAXWORDLEN 127

/* 
 * I like weird numbers, and this one is also prime, its also close to 5000
 * 4999
 */

#define HTABSIZE 4999

int main()
{
    htab_t *map = htab_init(HTABSIZE);
    if(map == NULL){
        return EXIT_FAILURE;
    }

    char *word = (char*) malloc(MAXWORDLEN);
    if(word == NULL){
        fprintf(stderr, "Failed to malloc word in wordcount.c");
        return EXIT_FAILURE;
    }
    htab_pair_t *item;

    while(read_word(word, MAXWORDLEN, stdin)){
        item = htab_lookup_add(map, word);
        if(item == NULL) {
            return EXIT_FAILURE;
        }
        item->value++;
    }

    htab_for_each(map, *htab_print);

    free(word);
    htab_clear(map);
    free(map);
    return 0;
}
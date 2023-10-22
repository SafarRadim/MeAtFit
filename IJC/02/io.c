/*
 * ======================================================== *
 * Name:       io.c                                         *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/19/2022                                   *
 * ======================================================== *
 */

#include <stdio.h>
#include <ctype.h>

#include "io.h"
#include "htab.h"

int read_word(char *s, int max, FILE *f)
{
    char c;
    int amn = 0;
    while((c = fgetc(f)) != EOF){
        if (amn == max){
            s[amn] = 0;
            while(!isspace(c=fgetc(f))) {};
            break;
        }

        if(isspace(c)){
            s[amn] = 0;
            break;
        } else {
            s[amn++] = c;
        }
    }

    return amn;
}

void htab_print(htab_pair_t *pair){
    printf("%s\t%d\n", pair->key, pair->value);
}
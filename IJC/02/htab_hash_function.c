/*
 * ======================================================== *
 * Name:       htab_hash_function.c                         *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/18/2022                                   *
 * ======================================================== *
 */

#include <stdint.h>
#include "htab.h"

size_t htab_hash_function(const char *str)
{
    uint32_t hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

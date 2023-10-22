/*
 * ======================================================== *
 * Name:       htab_bucket_count.c                          *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/18/2022                                   *
 * ======================================================== *
 */

#include "htab.h"
#include "htab_structs.h"

size_t htab_bucket_count(const htab_t *t)
{
    return t->arr_size;
}
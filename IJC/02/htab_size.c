/*
 * ======================================================== *
 * Name:       htab_size.c                                  *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/18/2022                                   *
 * ======================================================== *
 */

#include "htab.h"
#include "htab_structs.h"

size_t htab_size(const htab_t *t)
{
    return t->size;
}
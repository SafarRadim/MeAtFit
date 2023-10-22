/*
 * ======================================================== *
 * Name:       htab_free.c                                  *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/19/2022                                   *
 * ======================================================== *
 */

#include <stdlib.h>

#include "htab.h"
#include "htab_structs.h"

void htab_free(htab_t *t)
{
    htab_clear(t);
    free(t);
}
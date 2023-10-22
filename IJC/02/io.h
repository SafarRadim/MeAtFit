/*
 * ======================================================== *
 * Name:       io.h                                         *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/19/2022                                   *
 * ======================================================== *
 */

#ifndef IO
#define IO

#include "htab.h"

int read_word(char *s, int max, FILE *f);

void htab_print(htab_pair_t *pair);

#endif // IO
/*
 * ======================================================== *
 * Name:       hydrogen.h                                   *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/24/2022                                   *
 * ======================================================== *
 */

#ifndef HYDROGEN
#define HYDROGEN

int h_generate(int amount, pid_t *list, FILE *file);

int h_main(int rank, FILE *file);

#endif // HYDROGEN
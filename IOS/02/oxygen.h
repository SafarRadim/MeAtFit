/*
 * ======================================================== *
 * Name:       oxygen.h                                     *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/24/2022                                   *
 * ======================================================== *
 */

#ifndef OXYGEN
#define OXYGEN

int o_generate(int amount, pid_t *list, FILE *file);

int o_main(int rank, FILE *file);

#endif // OXYGEN
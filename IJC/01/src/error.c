/*
 * ======================================================== *
 * Name:        error.c                                     *
 * Author:      RADIM SAFAR - FIT VUT                       *
 * Email:       xsafar27(at)stud.fit.vutbr.cz               *
 * Last modif:  03/21/2022                                  *
 * ======================================================== *
 */


#include "../headers/error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void error_exit(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "Chyba: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}

void warning_msg(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "Chyba: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
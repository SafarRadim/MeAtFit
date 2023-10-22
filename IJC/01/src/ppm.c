/*
 * ======================================================== *
 * Name:        ppm.c                                       *
 * Author:      RADIM SAFAR - FIT VUT                       *
 * Email:       xsafar27(at)stud.fit.vutbr.cz               *
 * Last modif:  03/22/2022                                  *
 * ======================================================== *
 */

#include "../headers/ppm.h"
#include "../headers/error.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct ppm* ppm_read(const char *filename)
{
    FILE *file;

    file = fopen(filename, "r");
    if(file == NULL){
        error_exit("Failed to load file");
    }

    char magicChar[3];
    int width;
    int height;
    int maxColor;

    if (fscanf(file, "%s %d %d %d ", magicChar, &width, &height, &maxColor) == EOF){
        warning_msg("Error while reading file header");
        fclose(file);
        return NULL;
    }

    if(strcmp(magicChar, "P6")){
        warning_msg("Wrong filetype.");
        fclose(file);
        return NULL;
    }

    int fileSize = width * height * 3;
    if (fileSize > maxPpmSize){
        warning_msg("Image too big.");
        fclose(file);
        return NULL;
    }
    
    struct ppm *ppmFile = malloc(sizeof(struct ppm) + fileSize);

    if(ppmFile == NULL){
        error_exit("Failed to alloc memory for ppmFile.");
    }

    ppmFile->xsize = width;
    ppmFile->ysize = height;

    char c;
    int n = 0;
    while ((c = fgetc(file)) != EOF){
        ppmFile->data[n++] = c;
    }

    fclose(file);
    return ppmFile;
}

void ppm_free (struct ppm *p)
{
    free (p);
}
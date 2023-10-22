/*
 * ======================================================== *
 * Name:        ppm.h                                       *
 * Author:      RADIM SAFAR - FIT VUT                       *
 * Email:       xsafar27(at)stud.fit.vutbr.cz               *
 * Last modif:  03/18/2022                                  *
 * ======================================================== *
 */

#include "error.h"


/**
 * @brief Max size of RGB data of loaded ppm file in bytes.
 * 
 */
#define maxPpmSize 8000*8000*3

/**
 * @brief Struct for .ppm image
 * 
 * Struct for storing .ppm image.
 * xsize is equal to width of the image.
 * ysize is equal to height of the image.
 * data is binary stored RGB data.
 * 
 */
struct ppm {
    unsigned xsize;
    unsigned ysize;
    char data[];
};

/**
 * @brief Reads .ppm file, returns ppm struct.
 * 
 * Reads the contents of .ppm file and saves it into a struct.
 * Exits program if loading of file or malloc fails.
 * Returns NULL if error occures while reading the header or if image is larger
 * than maxPpmSize or if file is not .ppm
 * 
 * @param filename 
 * @return struct ppm* 
 */
struct ppm* ppm_read(const char *filename);

/**
 * @brief Frees ppm struct.
 * 
 * @param p 
 */
void ppm_free(struct ppm *p);
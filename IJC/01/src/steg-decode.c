/*
 * ======================================================== *
 * Name:        steg-decode.c                               *
 * Author:      RADIM SAFAR - FIT VUT                       *
 * Email:       xsafar27(at)stud.fit.vutbr.cz               *
 * Last modif:  03/22/2022                                  *
 * ======================================================== *
 */

#include <stdio.h>
#include "../headers/error.h"
#include "../headers/ppm.h"
#include "../headers/bitset.h"
#include "../headers/eratosthenes.h"


int main(int argc, char** argv)
{
    if (argc != 2){
        error_exit("Wrong amount of arguments. I just need .ppm file");
        return 0;
    }

    struct ppm *image = ppm_read(argv[1]);

    if (image == NULL){
        error_exit("Could not load file.");
    }

    int imgSize = image->xsize * image->ysize * 3;

    bitset_alloc(imgBitset, imgSize);
    eratosthenes(imgBitset);

    int bit = 0;
    int outByte = 0;
    for (int i = 29; i < imgSize; i++){
        // Skips all the non-primes
        if(bitset_getbit(imgBitset, i)){
            continue;
        }

        // Checks the last bit of the char (byte)
        if(image->data[i] % 2 != 0){
            outByte |= 1 << (bit % CHAR_BIT);
        } else {
            outByte &= ~(1 << (bit % CHAR_BIT));
        }

        bit++;

        // When we fill outByte
        if((bit % CHAR_BIT) == 0){
            
            // if its 0 print new line and end program
            if (outByte == 0){
                printf("\n");
                ppm_free(image);
                bitset_free(imgBitset);
                return EXIT_SUCCESS;
            }

            // print out the byte
            putchar(outByte);

            // reset outByte
            outByte = 0;
        }
    } 
}
/*
 * ======================================================== *
 * Name:        bitset.h                                    *
 * Author:      RADIM SAFAR - FIT VUT                       *
 * Email:       xsafar27(at)stud.fit.vutbr.cz               *
 * Last modif:  03/16/2022                                  *
 * ======================================================== *
 */


// Includes
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

#include "error.h"


// Helper defines
#define ul unsigned long
#define ul_bit (sizeof(ul) * CHAR_BIT)


// Code
#ifndef BITSET_H
#define BITSET_H

typedef ul *bitset_t;

typedef ul bitset_index_t;

/**
 * @brief Creates a bitset with jmeno_pole name and velikost size on stack
 * 
 * @param jmeno_pole
 * @param velikost
 */
#define bitset_create(jmeno_pole, velikost)                                    \
        static_assert(velikost > 0, "Velikost pole musi by vetsi jak 0");      \
        ul jmeno_pole[(velikost + ul_bit - 1) / ul_bit + 1] = {velikost}


/**
 * @brief Creates a bitset with jmeno_pole name and velikost size on heap
 * 
 * @param jmeno_pole
 * @param velikost
 */
#define bitset_alloc(jmeno_pole, velikost)                                     \
    if (velikost <= 0){                                                        \
        error_exit("Velikost musi byt vetsi jak 0 v bitset_alloc.");           \
    }                                                                          \
    bitset_t jmeno_pole;                                                       \
    jmeno_pole = calloc(                                                       \
        ((velikost + ul_bit - 1) / ul_bit + 1), sizeof(ul)                     \
    );                                                                         \
    if (jmeno_pole == NULL){                                                   \
        error_exit("Chyba alokace v bitset_alloc.");                           \
    }                                                                          \
    jmeno_pole[0] = velikost

#ifndef USE_INLINE
    /**
     * @brief Frees a bitset
     * 
     * @param jmeno_pole
     */
    #define bitset_free(jmeno_pole)                                            \
        free(jmeno_pole)                                                       \

    /**
     * @brief Returns size of a bitset
     * 
     * @param jmeno_pole
     */
    #define bitset_size(jmeno_pole)                                            \
        jmeno_pole[0]

    /**
     * @brief Sets a bit of a bitset on index, 0 for 0 value, 1 for non-0 value
     *
     * @param jmeno_pole
     * @param index
     * @param value
     */
    #define bitset_setbit(jmeno_pole, index, value)                            \
        if(value){                                                             \
            jmeno_pole[index / ul_bit + 1] |= 1UL << (index % ul_bit);         \
        } else {                                                               \
            jmeno_pole[index / ul_bit + 1] &= ~(1UL << (index % ul_bit));      \
        }                                                                  

    /**
     * @brief Gets a bit of a bitset on index
     * 
     * @param jmeno_pole
     * @param index
     */
    #define bitset_getbit(jmeno_pole, index)                                   \
        jmeno_pole[index / ul_bit + 1] & 1UL << (index % ul_bit)

#else // USE_INLINE defined

    /**
     * @brief Frees a bitset
     * 
     * @param jmeno_pole
     */
    inline void bitset_free(bitset_t jmeno_pole)
    {
        free(jmeno_pole);
    }

    /**
     * @brief Returns size of a bitset
     * 
     * @param jmeno_pole
     */
    inline ul bitset_size(bitset_t jmeno_pole)
    {
        return jmeno_pole[0];
    }

    /**
     * @brief Sets a bit of a bitset on index, 0 for 0 value, 1 for non-0 value
     *
     * @param jmeno_pole
     * @param index
     * @param value
     */
    inline void bitset_setbit(bitset_t jmeno_pole, bitset_index_t index, int value)
    {
        if(value){
            jmeno_pole[index / ul_bit + 1] |= 1UL << (index % ul_bit);
        } else {
            jmeno_pole[index / ul_bit + 1] &= ~(1UL << (index % ul_bit));
        }
    }
    
    /**
     * @brief Gets a bit of a bitset on index
     * 
     * @param jmeno_pole
     * @param index
     */
    inline ul bitset_getbit(bitset_t jmeno_pole, bitset_index_t index)
    {
        return jmeno_pole[index / ul_bit + 1] & 1UL << (index % ul_bit);
    }

#endif // end USE_INLINE

#endif // end BITSET_H
/*
 * ======================================================== *
 * Name:       atoms_structs.h                              *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 05/01/2022                                   *
 * ======================================================== *
 */


#ifndef ATOMS_STRUCTS
#define ATOMS_STRUCTS

#include <semaphore.h>

typedef struct params {
    int NO; // Number of Oxygens
    int NH; // Number of Hydrogens
    int TI; // Time to queue addition
    int TB; // Time to molecule creation
} params_t;

sem_t *write_sem;
sem_t *mutex;

sem_t *oxygen_sem;
sem_t *hydrogen_sem;

sem_t *turnstile0;
sem_t *turnstile1;
sem_t *turnstile_mutex;

sem_t *hydEntry_sem;
sem_t *oxyEntryMutex;



int sharedLineNum;
int sharedWaterNum;
int sharedOxyCount;
int sharedHydCount;
int sharedQueue;
int sharedAtomWait;
int sharedWaterWait;
int sharedHydAmount;
int sharedOxyAmount;

int *lineNum;
int *waterNum;
int *oxyCount;
int *hydCount;
int *queue;
int *atomWait;
int *waterWait; 
int *hydAmount;
int *oxyAmount;

#endif // ATOMS_STRUCTS
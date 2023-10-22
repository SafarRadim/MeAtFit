/*
 * ======================================================== *
 * Name:       atoms.c                                      *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 05/01/2022                                   *
 * ======================================================== *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "atoms_structs.h"
#include "oxygen.h"
#include "hydrogen.h"

#define HELPSTRING "\
Usage: atoms [NO] [NH] [TI] [TB]\n\
\n\
NO - number of oxygen atoms\n\
NH - number of hydrogen atoms\n\
TI - max time to wait before addition to queue in ms (0-1000)\n\
TB - max time to for water creation in ms (0-1000)\n"

#define MMAP_INIT NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0
#define SHMGET_INIT IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666

int argCheck(long arg, char* remainder, long minval){
    if (*remainder != '\0' || arg < minval || arg == *remainder){
        return 1;
    }
    return 0;
}

int argParse(int argc, char **argv, params_t *params)
{
    if (argc != 5) {
        fprintf(stderr, "Wrong arguments!\n%s", HELPSTRING);
        return EXIT_FAILURE;
    }

    char *remainder;
    int argC = 0;

    params->NO = strtol(argv[1], &remainder, 10);
    argC += argCheck(params->NO, remainder, 1);

    params->NH = strtol(argv[2], &remainder, 10);
    argC += argCheck(params->NH, remainder, 1);

    params->TI = strtol(argv[3], &remainder, 10);
    argC += argCheck(params->TI, remainder, 0);

    params->TB = strtol(argv[4], &remainder, 10);
    argC += argCheck(params->TB, remainder, 0);

    if (argC || params->TB > 1000 || params->TI > 1000){
        fprintf(stderr, "Wrong arguments!\n%s", HELPSTRING);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int semInit()
{
    // Writing
    write_sem = mmap(MMAP_INIT);
    if (write_sem == MAP_FAILED){
        fprintf(stderr, "ERROR: Failed to map write_sem.\n");
        return EXIT_FAILURE;
    }

    if (sem_init(write_sem, 1, 1) == -1){
        fprintf(stderr, "ERROR: Failed to init write_sem.\n");
        return EXIT_FAILURE;
    }

    // Mutex
    mutex = mmap(MMAP_INIT);
    if (mutex == MAP_FAILED){
        fprintf(stderr, "ERROR: Failed to map mutex.\n");
        return EXIT_FAILURE;
    }

    if (sem_init(mutex, 1, 1) == -1){
        fprintf(stderr, "ERROR: Failed to init mutex.\n");
        return EXIT_FAILURE;
    }

    // Oxygen
    oxygen_sem = mmap(MMAP_INIT);
    if (oxygen_sem == MAP_FAILED){
        fprintf(stderr, "ERROR: Failed to map oxygen_sem.\n");
        return EXIT_FAILURE;
    }

    if (sem_init(oxygen_sem, 1, 0)){
        fprintf(stderr, "ERROR: Failed to init oxygen_sem.\n");
        return EXIT_FAILURE;
    }

    // Hydrogen
    hydrogen_sem = mmap(MMAP_INIT);
    if (oxygen_sem == MAP_FAILED){
        fprintf(stderr, "ERROR: Failed to map hydrogen_sem.\n");
        return EXIT_FAILURE;
    }

    if (sem_init(hydrogen_sem, 1, 0)){
        fprintf(stderr, "ERROR: Failed to init hydrogen_sem.\n");
        return EXIT_FAILURE;
    }

    // Turnstile0
    turnstile0 = mmap(MMAP_INIT);
    if (turnstile0 == MAP_FAILED){
        fprintf(stderr, "ERROR: Failed to map turnstile0.\n");
        return EXIT_FAILURE;
    }

    if (sem_init(turnstile0, 1, 0)){
        fprintf(stderr, "ERROR: Failed to init turnstile0.\n");
        return EXIT_FAILURE;
    }

    // Turnstile1
    turnstile1 = mmap(MMAP_INIT);
    if (turnstile1 == MAP_FAILED){
        fprintf(stderr, "ERROR: Failed to map turnstile1.\n");
        return EXIT_FAILURE;
    }

    if (sem_init(turnstile1, 1, 1)){
        fprintf(stderr, "ERROR: Failed to init turnstile1.\n");
        return EXIT_FAILURE;
    }

    // Turnstile_mutex
    turnstile_mutex = mmap(MMAP_INIT);
    if (turnstile_mutex == MAP_FAILED){
        fprintf(stderr, "ERROR: Failed to map turnstile_mutex.\n");
        return EXIT_FAILURE;
    }

    if (sem_init(turnstile_mutex, 1, 1)){
        fprintf(stderr, "ERROR: Failed to init turnstile_mutex.\n");
        return EXIT_FAILURE;
    }

    // hydEntry_Sem
    hydEntry_sem = mmap(MMAP_INIT);
    if (hydEntry_sem == MAP_FAILED){
        fprintf(stderr, "ERROR: Failed to map turnstileA.\n");
        return EXIT_FAILURE;
    }

    if (sem_init(hydEntry_sem, 1, 0)){
        fprintf(stderr, "ERROR: Failed to init turnstileA.\n");
        return EXIT_FAILURE;
    }

    // oxyEnterMutex
    oxyEntryMutex = mmap(MMAP_INIT);
    if (oxyEntryMutex == MAP_FAILED){
        fprintf(stderr, "ERROR: Failed to map turnstile_mutex_2.\n");
        return EXIT_FAILURE;
    }

    if (sem_init(oxyEntryMutex, 1, 3)){
        fprintf(stderr, "ERROR: Failed to init turnstile_mutex_2.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int semDestroy()
{
    if (sem_destroy(write_sem) == -1){
        fprintf(stderr, "ERROR: Failed to destroy write_sem.\n");
        return EXIT_FAILURE;
    }

    if (sem_destroy(mutex) == -1){
        fprintf(stderr, "ERROR: Failed to destroy mutex.\n");
        return EXIT_FAILURE;
    }

    if (sem_destroy(oxygen_sem) == -1){
        fprintf(stderr, "ERROR: Failed to destroy oxygen_sem.\n");
        return EXIT_FAILURE;
    }

    if (sem_destroy(hydrogen_sem) == -1){
        fprintf(stderr, "ERROR: Failed to destroy hydrogen_sem.\n");
        return EXIT_FAILURE;
    }
    
    if (sem_destroy(turnstile0) == -1){
        fprintf(stderr, "ERROR: Failed to destroy turnstile0.\n");
        return EXIT_FAILURE;
    }
    
    if (sem_destroy(turnstile1) == -1){
        fprintf(stderr, "ERROR: Failed to destroy turnstile1.\n");
        return EXIT_FAILURE;
    }
    
    if (sem_destroy(turnstile_mutex) == -1){
        fprintf(stderr, "ERROR: Failed to destroy turnstile_mutex.\n");
        return EXIT_FAILURE;
    }

    if (sem_destroy(hydEntry_sem) == -1){
        fprintf(stderr, "ERROR: Failed to destroy turnstileA.\n");
        return EXIT_FAILURE;
    }
    
    if (sem_destroy(oxyEntryMutex) == -1){
        fprintf(stderr, "ERROR: Failed to destroy turnstile_mutex_2.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int memInit()
{
    // lineNum
    sharedLineNum = shmget(SHMGET_INIT);
    if (sharedLineNum == -1){
        fprintf(stderr, "ERROR: Failed to get page for sharedLineNum.\n");
        return EXIT_FAILURE;
    }

    lineNum = (int*) shmat(sharedLineNum, NULL, 0);
    if (lineNum == NULL){
        fprintf(stderr, "ERROR: Failed to attach lineNum.\n");
        return EXIT_FAILURE;
    }

    // waterNum
    sharedWaterNum = shmget(SHMGET_INIT);
    if (sharedWaterNum == -1){
        fprintf(stderr, "ERROR: Failed to get page for sharedWaterNum.\n");
        return EXIT_FAILURE;
    }

    waterNum = (int*) shmat(sharedWaterNum, NULL, 0);
    if (waterNum == NULL){
        fprintf(stderr, "ERROR: Failed to attach waterNum.\n");
        return EXIT_FAILURE;
    }

    // oxyCount
    sharedOxyCount = shmget(SHMGET_INIT);
    if (sharedOxyCount == -1){
        fprintf(stderr, "ERROR: Failed to get page for sharedOxyNum.\n");
        return EXIT_FAILURE;
    }

    oxyCount = (int*) shmat(sharedOxyCount, NULL, 0);
    if (oxyCount == NULL){
        fprintf(stderr, "ERROR: Failed to attach oxyCount.\n");
        return EXIT_FAILURE;
    }

    // hydCount
    sharedHydCount = shmget(SHMGET_INIT);
    if (sharedHydCount == -1){
        fprintf(stderr, "ERROR: Failed to get page for sharedHydCount.\n");
        return EXIT_FAILURE;
    }

    hydCount = (int*) shmat(sharedHydCount, NULL, 0);
    if (hydCount == NULL){
        fprintf(stderr, "ERROR: Failed to attach hydCount.\n");
        return EXIT_FAILURE;
    }

    // queue
    sharedQueue = shmget(SHMGET_INIT);
    if (sharedQueue == -1){
        fprintf(stderr, "ERROR: Failed to get page for sharedQueue.\n");
        return EXIT_FAILURE;
    }

    queue = (int*) shmat(sharedQueue, NULL, 0);
    if (queue == NULL){
        fprintf(stderr, "ERROR: Failed to attach queue.\n");
        return EXIT_FAILURE;
    }

    // atomWait
    sharedAtomWait = shmget(SHMGET_INIT);
    if (sharedAtomWait == -1){
        fprintf(stderr, "ERROR: Failed to get page for sharedAtomWait.\n");
        return EXIT_FAILURE;
    }

    atomWait = (int*) shmat (sharedAtomWait, NULL, 0);
    if (atomWait == NULL){
        fprintf(stderr, "ERROR: Failed to attach atomWait.\n");
        return EXIT_FAILURE;
    }

    // waterWait
    sharedWaterWait = shmget(SHMGET_INIT);
    if (sharedWaterWait == -1){
        fprintf(stderr, "ERROR: Failed to get page for sharedWaterWait.\n");
        return EXIT_FAILURE;
    }

    waterWait = (int*) shmat (sharedWaterWait, NULL, 0);
    if (waterWait == NULL){
        fprintf(stderr, "ERROR: Failed to attach waterWait.\n");
        return EXIT_FAILURE;
    }

    // hydAmount
    sharedHydAmount = shmget(SHMGET_INIT);
    if (sharedHydAmount == -1){
        fprintf(stderr, "ERROR: Failed to get page for sharedHydAmount.\n");
        return EXIT_FAILURE;
    }

    hydAmount = (int*) shmat(sharedHydAmount, NULL, 0);
    if (hydAmount == NULL){
        fprintf(stderr, "ERROR: Failed to attach hydAmount.\n");
        return EXIT_FAILURE;
    }
    
    // oxyAmount
    sharedOxyAmount = shmget(SHMGET_INIT);
    if (sharedHydAmount == -1){
        fprintf(stderr, "ERROR: Failed to get page for sharedOxyAmount.\n");
        return EXIT_FAILURE;
    }

    oxyAmount = (int*) shmat(sharedOxyAmount, NULL, 0);
    if (oxyAmount == NULL){
        fprintf(stderr, "ERROR: Failed to attach oxyAmount.\n");
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}

int memDestroy()
{
    // lineNum
    if (shmctl(sharedLineNum, IPC_RMID, NULL) == -1){
        fprintf(stderr, "ERROR: Failed to remove page for sharedLineNum.\n");
        return EXIT_FAILURE;
    }

    if (shmdt(lineNum) == -1){
        fprintf(stderr, "ERROR: Failed to detach lineNum.\n");
        return EXIT_FAILURE;
    }

    // waterNum
    if (shmctl(sharedWaterNum, IPC_RMID, NULL) == -1){
        fprintf(stderr, "ERROR: Failed to remove page for sharedWaterNum.\n");
        return EXIT_FAILURE;
    }

    if (shmdt(waterNum) == -1){
        fprintf(stderr, "ERROR: Failed to detach waterNum.\n");
        return EXIT_FAILURE;
    }

    // oxyCount
    if (shmctl(sharedOxyCount, IPC_RMID, NULL) == -1){
        fprintf(stderr, "ERROR: Failed to remove page for sharedOxyCount.\n");
        return EXIT_FAILURE;
    }

    if (shmdt(oxyCount) == -1){
        fprintf(stderr, "ERROR: Failed to detach oxyCount.\n");
        return EXIT_FAILURE;
    }

    // hydCount
    if (shmctl(sharedHydCount, IPC_RMID, NULL) == -1){
        fprintf(stderr, "ERROR: Failed to remove page for sharedOxyCount.\n");
        return EXIT_FAILURE;
    }

    if (shmdt(hydCount) == -1){
        fprintf(stderr, "ERROR: Failed to detach hydCount.\n");
        return EXIT_FAILURE;
    }

    // queue
    if (shmctl(sharedQueue, IPC_RMID, NULL) == -1){
        fprintf(stderr, "ERROR: Failed to remove page for sharedQueue.\n");
        return EXIT_FAILURE;
    }

    if (shmdt(queue) == -1){
        fprintf(stderr, "ERROR: Failed to detach queue.\n");
        return EXIT_FAILURE;
    }

    // atomWait
    if (shmctl(sharedAtomWait, IPC_RMID, NULL) == -1){
        fprintf(stderr, "ERROR: Failed to remove page for sharedAtomWait.\n");
        return EXIT_FAILURE;
    }

    if (shmdt(atomWait) == -1){
        fprintf(stderr, "ERROR: Failed to detach atomWait.\n");
        return EXIT_FAILURE;
    }

    // waterWait
    if (shmctl(sharedWaterWait, IPC_RMID, NULL) == -1){
        fprintf(stderr, "ERROR: Failed to remove page for sharedWaterWait.\n");
        return EXIT_FAILURE;
    }

    if (shmdt(waterWait) == -1){
        fprintf(stderr, "ERROR: Failed to detach waterWait.\n");
        return EXIT_FAILURE;
    }

    // queue2
    if (shmctl(sharedHydAmount, IPC_RMID, NULL) == -1){
        fprintf(stderr, "ERROR: Failed to remove page for sharedQueue2.\n");
        return EXIT_FAILURE;
    }

    if (shmdt(hydAmount) == -1){
        fprintf(stderr, "ERROR: Failed to detach queue2.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main (int argc, char **argv)
{
    int retVal = EXIT_SUCCESS;
    params_t *params = (params_t*) malloc (sizeof(params_t));

    if (argParse(argc, argv, params)){
        free(params);
        exit(EXIT_FAILURE);
    }

    if (semInit()){
        free(params);
        exit(EXIT_FAILURE);
    }

    if (memInit()){
        free(params);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen("proj2.out", "w");
    if (file == NULL){
        fprintf(stderr, "ERROR: failed to open proj2.out.\n");
        free(params);
        exit(EXIT_FAILURE);
    }
    setbuf(file, NULL);

    pid_t oxyChildren[params->NO];
    pid_t hydChildren[params->NH];

    *lineNum = 1;
    *waterNum = 0;
    *atomWait = params->TI;
    *waterWait = params->TB;
    *oxyAmount = params->NO;
    *hydAmount = params->NH;

    pid_t main_pid = fork();

    // oxygen generation
    if (main_pid == 0) {
        if(o_generate(params->NO, oxyChildren, file)){
            retVal = EXIT_FAILURE;
            goto CLEANUP;
        }

        for(int i = 0; i < params->NO; i++){
            waitpid(oxyChildren[i], NULL, 0);
        }
        exit(EXIT_SUCCESS);
    }

    // hydrogen generation
    if (main_pid > 0) {
        if(h_generate(params->NH, hydChildren, file)){
            retVal = EXIT_FAILURE;
            goto CLEANUP;
        }

        for(int i = 0; i < params->NH; i++){
            waitpid(hydChildren[i], NULL, 0);
        }
    }

CLEANUP:
    free(params);
    fclose(file);
    if(semDestroy()){
        exit(EXIT_FAILURE);
    }
    return retVal;
}
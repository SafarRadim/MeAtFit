/*
 * ======================================================== *
 * Name:       hydrogen.c                                   *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/25/2022                                   *
 * ======================================================== *
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>

#include "hydrogen.h"
#include "atoms_structs.h"

int h_generate (int amount, pid_t *list, FILE *file)
{
    for(int i = 0; i < amount; i++){
        pid_t hyd_pid = fork();
        if (hyd_pid == 0){
            if(h_main(i + 1, file)){
                fprintf(stderr, "ERROR: h_main for hydrogen %d failed.\n", i);
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        } else if (hyd_pid > 0) {
            list[i] = hyd_pid;
        }
    }
    return EXIT_SUCCESS;
}

int h_main (int rank, FILE *file)
{
    sem_wait(write_sem);
        fprintf(file, "%d: H %d: started\n", *lineNum, rank);
        (*lineNum)++;
        fflush(file);
    sem_post(write_sem);


    sem_wait(mutex); // makes sure only he is working
        usleep(random() % (*atomWait + 1));

        sem_wait(write_sem);
            fprintf(file, "%d: H %d: going to queue\n", *lineNum, rank);
            (*lineNum)++;
            fflush(file);
        sem_post(write_sem);

        sem_wait(turnstile_mutex);
            if ((*oxyAmount) * 2 > (*hydAmount)){
                sem_wait(write_sem);
                    fprintf(file, "%d: H %d not enough O\n", *lineNum, rank);
                    (*lineNum)++;
                    (*hydAmount)--;
                    fflush(file);
                sem_post(write_sem);
                sem_post(turnstile_mutex);
                sem_post(mutex);
                return 0;
            }
        sem_post(turnstile_mutex);

        (*hydCount)++;
        if(*hydCount >= 2 && *oxyCount >= 1){
            sem_post(hydrogen_sem); // allows "his" two hydrogens to go
            sem_post(hydrogen_sem);
            (*hydCount) -= 2;
            sem_post(oxygen_sem); // creates a spot in semaphore for itself
            (*oxyCount)--;
        } else {
            sem_post(mutex); // lets other atoms continue
        }
        
        sem_wait(hydrogen_sem);
        sem_wait(hydEntry_sem);

        sem_wait(write_sem);
            fprintf(file, "%d: H %d: creating molecule %d\n", *lineNum, rank, *waterNum);
            int num = *waterNum;
            (*lineNum)++;
            fflush(file);
        sem_post(write_sem);

        usleep(random() % (*waterWait + 1));

        sem_wait(write_sem);
            fprintf(file, "%d: H %d: molecule %d created\n", *lineNum, rank, num);
            (*lineNum)++;
            fflush(file);
        sem_post(write_sem);

        // barrier magic
        sem_wait(turnstile_mutex);

            (*queue)++;
            if(*queue == 3){
                sem_wait(turnstile1);
                sem_post(turnstile0);
            }
        sem_post(turnstile_mutex);

        sem_wait(turnstile0);
        sem_post(turnstile0);

        sem_wait(turnstile_mutex);
            (*queue)--;
            if(*queue == 0){
                sem_wait(turnstile0);
                sem_post(turnstile1);
            }
        sem_post(turnstile_mutex);

        sem_wait(turnstile1);
        sem_post(turnstile1);
        // end of barrier magic

    sem_post(mutex);

    return 0;
}
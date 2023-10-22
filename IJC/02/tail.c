/*
 * ======================================================== *
 * Name:       tail.c                                       *
 * Author:     RADIM SAFAR - FIT VUT                        *
 * Email:      xsafar27(at)stud.fit.vutbr.cz                *
 * Last modif: 04/18/2022                                   *
 * ======================================================== *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HELPSTRING "Program use: tail [-n AMN] [file]\n"
#define MAXLINE 100

typedef struct tail_input_t {
    FILE *file;
    int amn;
} tail_input;

tail_input* arg_parse(int argc, char **argv)
{
    if (argc > 4){
        printf(HELPSTRING);
        exit(EXIT_SUCCESS);
    }

    tail_input *out = (tail_input*) malloc(sizeof(tail_input));
    
    out->amn = 10;
    if (argc == 1) {
        out->file = stdin;
        return out;
    }

    for (int i = 1; i < argc; i++){
        if (!strcmp(argv[i], "-n")){
            out->amn = atoi(argv[++i]);
        } else {
            out->file = fopen(argv[i], "r");
        }
    }

    return out;
}

void shiftArr(char *newItem, char **array, int arrLen)
{
    for(int i = arrLen - 1; i > 0; i--){
        strcpy(array[i], array[i-1]);
    }
    strcpy(array[0], newItem);
}

int main (int argc, char **argv)
{
    tail_input *input = arg_parse(argc, argv);

    char **tail;
    tail = malloc(sizeof(char*) * input->amn);
    if(tail == NULL){
        fprintf(stderr, "Failed to malloc tail in tail.c");
        return NULL;
    }

    for(int i = 0; i < input->amn + 1; i++){
        tail[i] = malloc(MAXLINE * sizeof(char));
        if(tail[i] == NULL){
            fprintf(stderr, "Failed to malloc tail[%d] in tail.c", i);
            return NULL;
        }
    }

    char *newItem = malloc(sizeof(char) * MAXLINE);
    if(newItem == NULL){
        fprintf(stderr, "Failed to malloc newItem in tail.c");
        return NULL;
    }
    *newItem = 0;
    
    int maxLine = 0;
    while(fgets(newItem, MAXLINE, input->file) != NULL){
        if(strlen(newItem) == MAXLINE - 1 && newItem[MAXLINE - 2] != '\n'){
            if(!maxLine){
                maxLine++;
                fprintf(stderr, "Warning: Line too long, skipping the rest.\n");
                fprintf(stderr, "         Maxline is %d.\n", MAXLINE);
            }
            shiftArr(newItem, tail, input->amn);
            if(fscanf(input->file, "%*[^\n]") == EOF){
                break;
            }; 
        } else {
            shiftArr(newItem, tail, input->amn);
        }
    }
    
    for(int i = input->amn; i >= 0; i--){
        printf("%s", tail[i]);
    }
    
    free(newItem);
    for(int i = 0; i < input->amn + 1; i++){
        free(tail[i]);
    }
    free(tail);
    fclose(input->file);
    free(input);

    return 0;
}

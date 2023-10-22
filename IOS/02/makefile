# ======================================================== *
# Name:       makefile                                     *
# Author:     RADIM SAFAR - FIT VUT                        *
# Email:      xsafar27(at)stud.fit.vutbr.cz                *
# Last modif: 04/24/2022                                   *
# ======================================================== *

CFLAGS := -std=gnu99 -Wall -Wextra -Werror -pedantic -g
LDFLAGS := -pthread
CC := gcc

all: proj2

proj2: proj2.o oxygen.o hydrogen.o

proj2.o: proj2.c

oxygen.o: oxygen.c

hydrogen.o: hydrogen.c

clear: clean

clean:
	rm proj2 proj2.out *.o

#
# ======================================================== *
# Name:        Makefile                                    *
# Author:      RADIM SAFAR - xsafar27(at)stud.fit.vutbr.cz *
# Last modif:  03/18/2022                                  *
# ======================================================== *
#


#======#
# VARS #
#======#

LC_ALL := cs._CZ.utf8
CC := gcc
CFLAGS := -g --std=c11 -pedantic -Wall -Wextra -O2
LDLIBS := -lm
VPATH := src:headers


#=====#
# ALL #
#=====#

all: primes primes-i steg-decode


#========#
# PRIMES #
#========#

run: primes primes-i
	./primes
	./primes-i

primes: primes.o eratosthenes.o bitset.o error.o

primes.o: primes.c

eratosthenes.o: CFLAGS := $(CFLAGS) -lm
eratosthenes.o: eratosthenes.c

bitset.o: bitset.c

error.o: error.c


primes-i: primes-i.o eratosthenes-i.o bitset-i.o error.o
	$(CC) $(CFLAGS) primes-i.o eratosthenes-i.o bitset-i.o error.o -o primes-i -lm

primes-i.o: primes.c
	$(CC) $(CFLAGS) -c src/primes.c -o primes-i.o

eratosthenes-i.o: eratosthenes.c
	$(CC) $(CFLAGS) -c src/eratosthenes.c -o eratosthenes-i.o

bitset-i.o: bitset.c
	$(CC) $(CFLAGS) -DUSE_INLINE -c src/bitset.c -o bitset-i.o


#=====#
# PPM #
#=====#

steg-decode: steg-decode.o error.o ppm.o bitset-i.o eratosthenes.o

steg-decode.o: steg-decode.c

ppm.o: ppm.c


#==========#
# CLEAN UP #
#==========#

clean-o:
	rm *.o

clear: clean
clean:
	rm *.o
	rm primes primes-i steg-decode
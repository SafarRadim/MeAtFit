#
# ======================================================== *
# Name:        makefile                                    *
# Author:      RADIM SAFAR - xsafar27(at)stud.fit.vutbr.cz *
# Last modif:  04/19/2022                                  *
# ======================================================== *
#

#======#
# VARS #
#======#

LC_ALL := cs._CZ.utf8
CC := gcc
CFLAGS := -g --std=c11 -pedantic -Wall -Wextra -O2

#=====#
# ALL #
#=====#
all: tail wordcount wordcount_dynamic wordcount_ref

#======#
# TAIL #
#======#
tail: tail.c

#===========#
# WORDCOUNT #
#===========#
wordcountRef:
	g++ -O2 -o wordcount_ref wordcount_ref.cc

wordcount_dynamic: wordcount.o io.o libhtab.so
	$(CC) $(CFLAGS) $^ -o wordcount-dynamic

wordcount: wordcount.o io.o libhtab.a

wordcount.o: wordcount.c

io.o: io.c

libtab.so: CFLAGS := $(CFLAGS) -fPIC -c
libhtab.so: htab_bucket_count.o htab_clear.o htab_erase.o htab_find.o htab_for_each.o htab_free.o htab_hash_function.o htab_init.o htab_lookup_add.o htab_resize.o htab_size.o
	$(CC) $(CFLAGS) -shared $^ -o $@

libtahb.a: CFLAGS := $(CFLAGS) -fPIC -c
libhtab.a: htab_bucket_count.o htab_clear.o htab_erase.o htab_find.o htab_for_each.o htab_free.o htab_hash_function.o htab_init.o htab_lookup_add.o htab_resize.o htab_size.o
	ar rcs $@ $^

htab_bucket_count.o: htab_bucket_count.c

htab_clear.o: htab_clear.c

htab_erase.o: htab_erase.c

htab_find.o: htab_find.c

htab_for_each.o: htab_for_each.c

htab_free.o: htab_free.c

htab_hash_function.o: htab_hash_function.c

htab_init.o: htab_init.c

htab_lookup_add.o: htab_lookup_add.c

htab_resize.o: htab_resize.c

htab_size.o: htab_size.c

#---------#
# cleanup #
#---------#
clear:
	rm *.o wordcount wordcount_ref tail libhtab.a libhtab.so

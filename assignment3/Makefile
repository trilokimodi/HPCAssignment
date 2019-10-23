# Usage:
# make			# compile all binary with CFLAGS
				# except one that is filtered out
# make clean	# remove all binaries and objects

.PHONY = all clean

CC = gcc			# compiler to use

CFLAGS =-g -O2 -lm -fopenmp

#SRCS := $(wildcard *.c)
SRCS := cell_distances.c

# SRCS := $(filter-out mul_cpx_separatefile.c, $(SRCS)) # Doesn't compile mul_cpx_separatefile.c
BINS := $(SRCS:%.c=%)

all: ${BINS}

# file: file.c
# 	gcc -g valgrind.c -o valgrind

clean:
	@echo "Cleaning up..."
	rm -rvf *.o *.ppm ${BINS}




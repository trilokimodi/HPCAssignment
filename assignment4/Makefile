# Usage:
# make			# compile all binary with CFLAGS
				# except one that is filtered out
# make clean	# remove all binaries and objects

.PHONY = all clean

#CC = gcc			# compiler to use

#CFLAGS =-o -lm OpenCL

#SRCS := $(wildcard *.c)
#SRCS := Diffusion_TemperaturesWithoutOpenCL.c
#SRCS := OpenCL_Diffusion_Temperature.c

# SRCS := $(filter-out mul_cpx_separatefile.c, $(SRCS)) # Doesn't compile mul_cpx_separatefile.c
#BINS := $(SRCS:%.c=%)

#all: ${BINS}

# file: file.c
# 	gcc -g valgrind.c -o valgrind



all : diffusion_Temperature Diffusion_TemperaturesWithoutOpenCL doubleglobal_Nobarrier badlocality double_global_cpu doubleglobal_withbarrier
#all : doubleglobal_Nobarrier

diffusion_Temperature : OpenCL_Diffusion_Temperature.c
	gcc -O2 OpenCL_Diffusion_Temperature.c -o diffusion_Temperature -g -lm -lOpenCL

badlocality : OpenCLWithBadLocality.c
	gcc -O2 OpenCLWithBadLocality.c -o badlocality -g -lm -lOpenCL

#doubleglobal_withbarrier : OpenCL_DoubleGlobal_withbarrier.c
#	gcc -O2 OpenCL_DoubleGlobal_withbarrier.c -o doubleglobal_withbarrier -lm -lOpenCL

doubleglobal_Nobarrier : OpenCL_DoubleGlobal_Nobarrier.c
	gcc -O2 OpenCL_DoubleGlobal_Nobarrier.c -o doubleglobal_Nobarrier -lm -lOpenCL

Diffusion_TemperaturesWithoutOpenCL : Diffusion_TemperaturesWithoutOpenCL.c
	gcc -O2 Diffusion_TemperaturesWithoutOpenCL.c -o Diffusion_TemperaturesWithoutOpenCL -g -lm

double_global_cpu : OpenCL_DoubleGlobal_CPU.c
	gcc -O2 OpenCL_DoubleGlobal_CPU.c -o double_global_cpu -lm -lOpenCL


# all : diffusion_Temperature

# diffusion_Temperature : OpenCL_Diffusion_Temperature.c
# 	gcc OpenCL_Diffusion_Temperature.c -o diffusion_Temperature -g -lm -lOpenCL

clean:
	rm -rf *.o *.ppm Diffusion_TemperaturesWithoutOpenCL diffusion_Temperature badlocality doubleglobal_withbarrier doubleglobal_Nobarrier
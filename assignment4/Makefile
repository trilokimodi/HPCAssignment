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



# all : diffusion_Temperature Diffusion_TemperaturesWithoutOpenCL badlocality doubleglobal

# diffusion_Temperature : OpenCL_Diffusion_Temperature.c
# 	gcc OpenCL_Diffusion_Temperature.c -o diffusion_Temperature -g -lm -lOpenCL

# badlocality : OpenCLWithBadLocality.c
# 	gcc OpenCLWithBadLocality.c -o badlocality -g -lm -lOpenCL

# doubleglobal : OpenCL_DoubleGlobal.c
# 	gcc OpenCL_DoubleGlobal.c -o doubleglobal -g -lm -lOpenCL

# Diffusion_TemperaturesWithoutOpenCL : Diffusion_TemperaturesWithoutOpenCL.c
# 	gcc Diffusion_TemperaturesWithoutOpenCL.c -o Diffusion_TemperaturesWithoutOpenCL -lm


all : diffusion_Temperature

diffusion_Temperature : OpenCL_Diffusion_Temperature.c
	gcc OpenCL_Diffusion_Temperature.c -o diffusion_Temperature -g -lm -lOpenCL

clean:
	rm -rf *.o *.ppm Diffusion_TemperaturesWithoutOpenCL diffusion_Temperature badlocality doubleglobal
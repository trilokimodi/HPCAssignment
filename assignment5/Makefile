#all: ${EXECS}
all : heat_diffusion

heat_diffusion : MPI_heat_diffusion.c
	gcc -g -O2 MPI_heat_diffusion.c -o heat_diffusion -I/usr/include/openmpi-x86_64 -pthread -Wl,-rpath -Wl,/usr/lib64/openmpi/lib -Wl,--enable-new-dtags -L/usr/lib64/openmpi/lib -lmpi -lm

clean:
	rm heat_diffusion
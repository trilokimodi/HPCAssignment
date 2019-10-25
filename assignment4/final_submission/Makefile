.PHONY = all clean

all : heat_diffusion

heat_diffusion : heat_diffusion.c
	gcc -O2 heat_diffusion.c -o heat_diffusion -lm -lOpenCL

clean:
	rm -rf *.o *.ppm heat_diffusion
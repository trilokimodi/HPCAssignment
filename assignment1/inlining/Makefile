all : mainfile separatefile inlined

mainfile : mainfile.c
	gcc mainfile.c -o mainfile

separatefile : separatefile.c mul_cpx_separatefile.c
	gcc separatefile.c mul_cpx_separatefile.c -o separatefile

inlined : inlined.c
	gcc inlined.c -o inlined

clean :
	rm !(*.c|"Makefile") 


CF = -O2
all : mainfile separatefile inlined

mainfile : mainfile.s
	gcc mainfile.s -o mainfile

mainfile.s : mainfile.c
	gcc mainfile.c -S -O2 -o mainfile.s

#mainfile : mainfile.c
#gcc $(CF) mainfile.c -o mainfile

separatefile : separatefile.c mul_cpx_separatefile.c
	gcc $(CF) separatefile.c mul_cpx_separatefile.c -o separatefile

inlined : inlined.c
	gcc $(CF) inlined.c -o inlined

clean :
	rm mainfile inlined separatefile *.s
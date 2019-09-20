CF = -O2
all : locality

locality : locality.s
	gcc $(CF) locality.s -o locality

locality.s : locality.c
	gcc $(CF) locality.c -S -o locality.s

clean :
	rm -f locality *.s
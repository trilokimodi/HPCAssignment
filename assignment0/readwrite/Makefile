#all : memfra filehandling 
all : assignment0write assignment0read

#memfra : memfra.o
#	gcc memfra.o -o memfra

#memfra.o : memfra.c
#	gcc -c memfra.c

#filehandling : filehandling.o
#	gcc filehandling.o -o filehandling

#filehandling.o : filehandling.c
#	gcc -c filehandling.c

assignment0write : assignment0filewrite.c
	gcc assignment0filewrite.c -o assignment0write

assignment0read : assignment0fileread.c matrix.c
	gcc assignment0fileread.c matrix.c -o assignment0read

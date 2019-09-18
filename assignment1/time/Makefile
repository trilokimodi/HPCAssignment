all : timeO0 timeO1 timeO2 timeO3 timeOs timeOg

timeO0 : timeO0.s
	gcc timeO0.s -o timeO0

timeO0.s : time.c
	gcc time.c -S -O0 -o timeO0.s

timeO1 : timeO1.s
	gcc timeO1.s -o timeO1

timeO1.s : time.c
	gcc time.c -S -O1 -o timeO1.s

timeO2 : timeO2.s
	gcc timeO2.s -o timeO2

timeO2.s : time.c
	gcc time.c -S -O2 -o timeO2.s

timeO3 : timeO3.s
	gcc timeO3.s -o timeO3

timeO3.s : time.c
	gcc time.c -S -O3 -o timeO3.s

timeOs : timeOs.s
	gcc timeOs.s -o timeOs

timeOs.s : time.c
	gcc time.c -S -Os -o timeOs.s

timeOg : timeOg.s
	gcc timeOg.s -o timeOg

timeOg.s : time.c
	gcc time.c -S -Og -o timeOg.s

clean : 
	rm timeO*
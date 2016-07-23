How to start?
Ans: type "make"

Key Point:
1. [gcc --help] 
	-c          Compile and assemble, but do not link

2. Output file: main.s main.o

3. Command:
		gcc -S main.c -o main.s
		as main.s -o main.o
	Equal to:
		gcc -c main.c -o main.o
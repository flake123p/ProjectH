How to start?
Ans: type "make"

Key Point:
1. [gcc --help] 
	-S    Compile only; do not assemble or link

2. Output intel syntax!!
	gcc -S   -masm=intel   main.c -o main_intel.s
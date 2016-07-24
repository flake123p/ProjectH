

nasm -fwin32 ta.asm
gcc -m32 -c main.c
gcc ta.obj main.o
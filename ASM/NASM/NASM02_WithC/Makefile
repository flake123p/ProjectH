#
# Add NASM Assembler
#
export PATH := ../../../../_toolchains/nasm-2.11.08-win32:$(PATH)

a.exe: ta.obj main.o
	gcc ta.obj main.o

ta.obj: ta.asm
	nasm -fwin32 ta.asm
	
main.o: main.c
	gcc -m32 -c main.c
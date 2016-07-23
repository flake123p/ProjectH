#
# Add NASM Assembler
#
export PATH := ../../../../_toolchains/nasm-2.11.08-win32:$(PATH)

a.exe: main.asm
	nasm -fwin32 main.asm
	gcc main.obj

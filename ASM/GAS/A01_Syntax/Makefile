
.PHONY: ALL BUILD_EXE BUILD_ASM BUILD_ASM_EXE

# 1st TARGET
BUILD_ASM_EXE :
	gcc -o main_asm.exe -m32 main.s

# 2nd TARGET
ALL : BUILD_EXE BUILD_ASM BUILD_ASM_EXE

BUILD_EXE :
	gcc -o main.exe main.c

#This should create a file called "hello.s" (".s" is the file extension 
#    that the GNU system gives to assembly files). 
#On more recent 64-bit systems, the 32-bit source tree may not be included, 
#    which will cause a "bits/predefs.h fatal error"; you may replace the 
#    "-m32" gcc directive with an "-m64" directive to generate 64-bit 
#    assembly instead.
BUILD_ASM :
	gcc -S  -m32 main.c

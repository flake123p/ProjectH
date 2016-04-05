main.o: src/main.cpp include/My_Basics.hpp include/My_Basics.h \
 include/My_Macros.h include/My_MacroFuncs.h include/My_Types.h
	g++ -Iinclude -c -g -Wall -std=c++11 src/main.cpp -o build/main.o 

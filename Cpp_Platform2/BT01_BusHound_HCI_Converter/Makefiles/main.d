main.o: src/main.c include/Everything_App.hpp \
 ../_Library/include/Everything_Lib_Mgr.hpp \
 ../_Library/include/My_Basics.hpp ../_Library/include/My_Basics.h \
 ../_Library/include/My_Macros.h ../_Library/include/My_MacroFuncs.h \
 ../_Library/include/_LibError.hpp ../_Library/include/My_Types.h \
 ../_Library/include/_LibLinkedList.hpp \
 ../_Library/include/_LibMemory.hpp ../_Library/include/_LibMenu.hpp \
 ../_Library/include/_LibString.hpp ../_Library/include/_Lib51Hex.hpp \
 ../_Library/include/_LibSyntex.hpp ../_Library/include/LibUart.hpp \
 ../_Library/include/LibTime.hpp ../_Library/include/LibFileIO.hpp \
 ../_Library/include/LibBuffer.hpp ../_Library/include/LibFileIO2.hpp \
 ../_Library/include/LibUtility.hpp ../_Library/include/LibOs.hpp \
 ../_Library/include/LibTui.hpp ../_Library/include/LibSysCmd.hpp \
 ../_Library/include/LibScreenLogic.hpp \
 ../_Library/include/LibSharedMemory.hpp ../_Library/include/LibEvent.hpp \
 ../_Library/include/LibThread.hpp ../_Library/include/LibBT_HCI.hpp \
 ../_Library/include/LibArgParser.hpp ../_Library/include/LibSimpleFS.hpp \
 ../_Library/include/LibEncrypt.hpp
	g++ -Iinclude -I../_Library/include -c -g -Wall -std=c++11 src/main.c -o build/main.o 

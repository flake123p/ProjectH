

#ifndef _LIB_FILE_IO_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================

#include <stdio.h>
#include <stdarg.h>

typedef struct File_Profiles{
	const char *fileName;
	const char *openMode;
	FILE       *fp;
} File_Profiles_t;

int  LibFileIo_OpenFile(File_Profiles_t *fileProfile);
void LibFileIo_CloseFile(File_Profiles_t *fileProfile);

//
// C++
//
#include "LibBuffer.hpp" // LibBufferBasic
#include <string.h>
typedef enum {
	NextLine_Win,
	NextLine_Unix,
	NextLine_None,
} NextLineStyle_t;
class LibFileIoClass 
{
	public:
		std::string fileName;
		std::string openMode;
		//const char *fileName;
		//const char *openMode;
		FILE *fp;
		bool isFileDbgMsgOn;
		LibBufferBasic lineBuffer;
		char *lineStr;
		int lineLen;
		u32 lineCount;
		u32 scanCount;
		bool printErrorMsg;
		
		LibFileIoClass(const char *inFileName = NULL, const char *inOpenMode = NULL);
		~LibFileIoClass(void);
		int  FileOpen(void);
		int  FileOpenForRead(u32 lineBufferSize = 0);
		int  FileClose(void);
		bool IsFileExist(void);
		void EnableFileDbgMsg(void){isFileDbgMsgOn=true;};
		void DisableFileDbgMsg(void){isFileDbgMsgOn=false;};
		int  GetLine(unsigned char *outputString, int maxLength, OUT NextLineStyle_t *nextLineStyle = NULL);
		int  GetLine(OUT NextLineStyle_t *nextLineStyle = NULL);
		int  GetLineEx(unsigned char *outputString, int maxLength, OUT int *readLength, OUT NextLineStyle_t *nextLineStyle = NULL);
		int  GetCharacter(void);
		int  FileScan(const char *format, ...);
};

void LibFileIoClass_Demo_Output_A_File(void);
void LibFileIoClass_Demo_Lite(void);

int  LibFileIO_TestNextLine_InWinAndLinux(const char *testFileName);

#define _LIB_FILE_IO_HPP_INCLUDED_
#endif//_LIB_FILE_IO_HPP_INCLUDED_


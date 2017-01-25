

#ifndef _LIB_FILE_IO_HPP_INCLUDED_

#include <stdio.h>

typedef struct File_Profiles{
	const char *fileName;
	const char *openMode;
	FILE       *fp;
} File_Profiles_t;

void LibFileIo_OpenFile(File_Profiles_t *fileProfile);
void LibFileIo_CloseFile(File_Profiles_t *fileProfile);
void LibLibFileIoClass_Demo_Output_A_File(void);

//
// C++
//
class LibFileIoClass 
{
	public:
		const char *fileName;
		const char *openMode;
		FILE *fp;
		bool isFileDbgMsgOn;
		
		LibFileIoClass(const char *inFileName, const char *inOpenMode)
		{
			fileName=inFileName;
			openMode=inOpenMode;
			fp=NULL;
			isFileDbgMsgOn=false;
		};
		~LibFileIoClass(void);
		void FileOpen(void);
		void FileClose(void);
		bool IsFileExist(void);
		void EnableFileDbgMsg(void){isFileDbgMsgOn=true;};
		void DisableFileDbgMsg(void){isFileDbgMsgOn=false;};
		int GetLine(unsigned char *outputString, int maxLength);
		int GetCharacter(void);
};

class LibFileIoClass_Lite : public LibFileIoClass {
	public:
		LibFileIoClass_Lite(const char *inFileName, const char *inOpenMode) : LibFileIoClass(inFileName, inOpenMode)
		{
			EnableFileDbgMsg();
			FileOpen();
		};
};

void LibLibFileIoClass_Demo_Output_A_File_Cpp(void);
void LibLibFileIoClass_Demo_Output_A_File_Cpp_Lite(void);

#define _LIB_FILE_IO_HPP_INCLUDED_
#endif//_LIB_FILE_IO_HPP_INCLUDED_




#ifndef _LIB_FILE_IO_HPP_INCLUDED_

#include <stdio.h>

typedef struct File_Profiles{
	const char *fileName;
	const char *openMode;
	FILE       *fp;
} File_Profiles_t;

void FileIoOpen(File_Profiles_t *fileProfile);
void FileIoClose(File_Profiles_t *fileProfile);
void Demo_Output_A_File(void);

//
// C++
//
class FileIO 
{
	public:
		const char *fileName;
		const char *openMode;
		FILE *fp;
		bool isFileDbgMsgOn;
		
		FileIO(const char *inFileName, const char *inOpenMode)
		{
			fileName=inFileName;
			openMode=inOpenMode;
			fp=NULL;
			isFileDbgMsgOn=false;
		};
		~FileIO(void);
		void FileOpen(void);
		void FileClose(void);
		bool IsFileExist(void);
		void EnableFileDbgMsg(void){isFileDbgMsgOn=true;};
		void DisableFileDbgMsg(void){isFileDbgMsgOn=false;};
		int GetLine(unsigned char *inputString, int maxLength);
};

class FileIO_Lite : public FileIO {
	public:
		FileIO_Lite(const char *inFileName, const char *inOpenMode) : FileIO(inFileName, inOpenMode)
		{
			EnableFileDbgMsg();
			FileOpen();
		};
};

void Demo_Output_A_File_Cpp(void);
void Demo_Output_A_File_Cpp_Lite(void);

#define _LIB_FILE_IO_HPP_INCLUDED_
#endif//_LIB_FILE_IO_HPP_INCLUDED_


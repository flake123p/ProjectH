//#include "stdafx.h" //For porting to "Visual Studio"
#include <stdio.h>
#include <stdlib.h>
#include "LibFileIO.hpp"
#include "My_Basics.hpp"

void FileIoOpen(File_Profiles_t *fileProfile)
{
	fileProfile->fp = fopen(fileProfile->fileName, fileProfile->openMode); 

	if (fileProfile->fp == NULL) { 
		printf("Cannot open: %s in mode: %s. Exit Now!\n", fileProfile->fileName, fileProfile->openMode);
		exit (EXIT_FAILURE);
	}
}

void FileIoClose(File_Profiles_t *fileProfile)
{
	if (fileProfile->fp == NULL) { 
		printf("fp is NULL: %s in mode: %s. Exit Now!\n", fileProfile->fileName, fileProfile->openMode);
		exit (EXIT_FAILURE);
	}

	int closeResult = 0;
	
	closeResult = fclose(fileProfile->fp);

	if(closeResult == EOF) {
		BASIC_ASSERT(0);
	}

	fileProfile->fp = NULL;
}

void Demo_Output_A_File(void)
{
	File_Profiles_t outFp = {
		"Demo_Output_A_File.txt",
		"w+b",
		NULL
	};

	FileIoOpen(&outFp);

	fprintf(outFp.fp, "Hello: Demo_Output_A_File\n");

	FileIoClose(&outFp);
}

FileIO::~FileIO(void)
{
	if(fp != NULL) {
		FileClose();
	}
}

void FileIO::FileOpen(void)
{
	File_Profiles_t filePara = {
		NULL,
		NULL,
		NULL
	};

	filePara.fileName = this->fileName;
	filePara.openMode = this->openMode;

	if(isFileDbgMsgOn) {
		printf("[LibFileIO]Open: %s (mode:%s)\n", this->fileName, this->openMode);
	}
	
	FileIoOpen(&filePara);

	this->fp = filePara.fp;
}

void FileIO::FileClose(void)
{
	File_Profiles_t filePara = {
		NULL,
		NULL,
		NULL
	};

	if (this->fp == NULL) {
		return;
	}
	
	filePara.fileName = this->fileName;
	filePara.openMode = this->openMode;
	filePara.fp       = this->fp;

	if(isFileDbgMsgOn) {
		printf("[LibFileIO]Close: %s (mode:%s)\n", this->fileName, this->openMode);
	}
	
	FileIoClose(&filePara);

	this->fp = NULL;
}

bool FileIO::IsFileExist(void)
{
	fp = NULL;
	fp = fopen(fileName, openMode); 

	if (fp == NULL) { 
		return false;
	}
	else {
		fclose(fp);
		fp = NULL;
		return true;
	}
}

// Max return value = maxLength - 1
int FileIO::GetLine(unsigned char *inputString, int maxLength)
{
	int ch;
	int numberOfChar = 0;

	inputString[0] = 0; // Clear string
	
	while (1) {
		if(numberOfChar >= maxLength-1) {
			break;
		}
		
		ch = fgetc(this->fp);
		if(ch == '\n' || ch == EOF) {
			break;
		}

		inputString[numberOfChar] = (unsigned char)ch;
		numberOfChar++;
	}

	inputString[numberOfChar] = 0; // End of string

	return numberOfChar;
}

void Demo_Output_A_File_Cpp(void)
{
	FileIO outFile("Demo_Output_A_File_Cpp.txt", "w+b");

	outFile.FileOpen();

	fprintf(outFile.fp, "Hello: Demo_Output_A_File_Cpp\n");
}

void Demo_Output_A_File_Cpp_Lite(void)
{
	FileIO_Lite outFile("Demo_Output_A_File_Cpp_Lite.txt", "w+b");

	fprintf(outFile.fp, "Hello: Demo_Output_A_File_Cpp_Lite\n");
}


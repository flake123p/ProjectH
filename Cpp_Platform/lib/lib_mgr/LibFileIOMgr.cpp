//#include "stdafx.h" //For porting to "Visual Studio"
#include <stdio.h>
#include <stdlib.h>

#include "My_Basics.hpp"

#include "LibFileIO.hpp"


void LibFileIo_OpenFile(File_Profiles_t *fileProfile)
{
	fileProfile->fp = fopen(fileProfile->fileName, fileProfile->openMode); 

	if (fileProfile->fp == NULL) { 
		printf("Cannot open: %s in mode: %s. Exit Now!\n", fileProfile->fileName, fileProfile->openMode);
		exit (EXIT_FAILURE);
	}
}

void LibFileIo_CloseFile(File_Profiles_t *fileProfile)
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

void LibLibFileIoClass_Demo_Output_A_File(void)
{
	File_Profiles_t outFp = {
		"LibLibFileIoClass_Demo_Output_A_File.txt",
		"w+b",
		NULL
	};

	LibFileIo_OpenFile(&outFp);

	fprintf(outFp.fp, "Hello: LibLibFileIoClass_Demo_Output_A_File\n");

	LibFileIo_CloseFile(&outFp);
}

LibFileIoClass::~LibFileIoClass(void)
{
	if(fp != NULL) {
		FileClose();
	}
}

void LibFileIoClass::FileOpen(void)
{
	File_Profiles_t filePara = {
		NULL,
		NULL,
		NULL
	};

	filePara.fileName = this->fileName;
	filePara.openMode = this->openMode;

	if(isFileDbgMsgOn) {
		printf("[LibLibFileIoClass]Open: %s (mode:%s)\n", this->fileName, this->openMode);
	}
	
	LibFileIo_OpenFile(&filePara);

	this->fp = filePara.fp;
}

void LibFileIoClass::FileClose(void)
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
		printf("[LibLibFileIoClass]Close: %s (mode:%s)\n", this->fileName, this->openMode);
	}
	
	LibFileIo_CloseFile(&filePara);

	this->fp = NULL;
}

bool LibFileIoClass::IsFileExist(void)
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
int LibFileIoClass::GetLine(unsigned char *outputString, int maxLength)
{
	int ch;
	int numberOfChar = 0;

	outputString[0] = 0; // Clear string
	
	while (1) {
		if(numberOfChar >= maxLength-1) {
			break;
		}
		
		ch = fgetc(this->fp);
		if(ch == '\n' || ch == EOF) {
			break;
		}

		outputString[numberOfChar] = (unsigned char)ch;
		numberOfChar++;
	}

	outputString[numberOfChar] = 0; // End of string

	return numberOfChar;
}

int LibFileIoClass::GetCharacter(void)
{
	return fgetc(this->fp);
}

void LibLibFileIoClass_Demo_Output_A_File_Cpp(void)
{
	LibFileIoClass outFile("LibLibFileIoClass_Demo_Output_A_File_Cpp.txt", "w+b");

	outFile.FileOpen();

	fprintf(outFile.fp, "Hello: LibLibFileIoClass_Demo_Output_A_File_Cpp\n");
}

void LibLibFileIoClass_Demo_Output_A_File_Cpp_Lite(void)
{
	LibFileIoClass_Lite outFile("LibLibFileIoClass_Demo_Output_A_File_Cpp_Lite.txt", "w+b");

	fprintf(outFile.fp, "Hello: LibLibFileIoClass_Demo_Output_A_File_Cpp_Lite\n");
}


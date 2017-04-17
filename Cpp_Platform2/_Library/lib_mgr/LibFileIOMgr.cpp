
#include "Everything_Lib_Mgr.hpp"

int LibFileIo_OpenFile(File_Profiles_t *fileProfile)
{
	fileProfile->fp = fopen(fileProfile->fileName, fileProfile->openMode); 

	if (fileProfile->fp == NULL) { 
		printf("Cannot open: %s in mode: %s. Exit Now!\n", fileProfile->fileName, fileProfile->openMode);
		return RC_FILE_OPEN_ERROR;
	}

	return 0;
}

void LibFileIo_CloseFile(File_Profiles_t *fileProfile)
{
	if (fileProfile->fp == NULL) { 
		printf("fp is NULL: %s in mode: %s. Exit Now!\n", fileProfile->fileName, fileProfile->openMode);
		return;
	}

	int closeResult = fclose(fileProfile->fp);
	if(closeResult == EOF) {
		BASIC_ASSERT(0);
	}

	fileProfile->fp = NULL;
}

LibFileIoClass::LibFileIoClass(const char *inFileName /* = NULL */, const char *inOpenMode /* = NULL */)
{
	if (inFileName == NULL) {
		fileName = "";
	} else {
		fileName = inFileName;
	}

	if (inOpenMode == NULL) {
		openMode = "";
	} else {
		openMode = inOpenMode;
	}

	fp=NULL;
	isFileDbgMsgOn=false;
	lineStr=NULL;
	lineCount=0;

	printErrorMsg = true;
};

LibFileIoClass::~LibFileIoClass(void)
{
	FileClose();
}

int LibFileIoClass::FileOpen(void)
{
	if(isFileDbgMsgOn) {
		printf("[LibFileIoClass] Open: %s (mode:%s)\n", this->fileName.c_str(), this->openMode.c_str());
	}

	this->fp = fopen(this->fileName.c_str(), this->openMode.c_str()); 

	if (this->fp == NULL) {
		if (printErrorMsg)
			printf("[LibFileIoClass] Cannot open: %s in mode: %s.\n", this->fileName.c_str(), this->openMode.c_str());
		return RC_FILE_OPEN_ERROR;
	}

	return 0;
}

int LibFileIoClass::FileOpenForRead(u32 lineBufferSize /* = 0 */)
{
	int retVal = FileOpen();
	RETURN_IF(retVal);
	
	if (lineBufferSize == 0) {
		lineBufferSize = 1024;
	}

	lineBuffer.Init(lineBufferSize);
	lineStr = (char *)lineBuffer.bufPtr;

	return 0;
}

int LibFileIoClass::FileClose(void)
{
	if (this->fp == NULL) {
		if(isFileDbgMsgOn) {
			printf("[LibFileIoClass] fp is NULL: %s in mode: %s. Do nothing!\n", this->fileName.c_str(), this->openMode.c_str());
		}
		return 0;
	}

	if(isFileDbgMsgOn) {
		printf("[LibFileIoClass] Close: %s (mode:%s)\n", this->fileName.c_str(), this->openMode.c_str());
	}
	
	int closeResult = fclose(this->fp);
	if(closeResult == EOF) {
		if (printErrorMsg)
			printf("[LibFileIoClass] File Close Error - %s in mode: %s.\n", this->fileName.c_str(), this->openMode.c_str());
		return RC_FILE_CLOSE_ERROR;
	}
	
	this->fp = NULL;

	return 0;
}

bool LibFileIoClass::IsFileExist(void)
{
	fp = NULL;
	fp = fopen(fileName.c_str(), openMode.c_str()); 

	if (fp == NULL) { 
		return false;
	}
	else {
		fclose(fp);
		fp = NULL;
		return true;
	}
}

/*
	 Return value:
	   1.) Return EOF at end
	   2.) Not include \n character
	   3.) Max return value = maxLength - 1
	   4.) Last character will be set to 0
*/
int LibFileIoClass::GetLine(unsigned char *outputString, int maxLength, OUT NextLineStyle_t *nextLineStyle /* = NULL */)
{
	int ch;
	int numberOfChar = 0;

	outputString[0] = 0; // Clear string

	if (nextLineStyle != NULL)
		*nextLineStyle = NextLine_None;
	
	while (1) {
		if(numberOfChar >= maxLength-1) {
			break;
		}
		
		ch = fgetc(this->fp);
		if(ch == EOF) {
			break;
		} else if(ch == '\n') {
			if (nextLineStyle != NULL)
				*nextLineStyle = NextLine_Unix;
			break;
		} else if (ch == '\r') {
			ch = fgetc(this->fp);
			if(ch == '\n') {
				if (nextLineStyle != NULL)
					*nextLineStyle = NextLine_Win;
				break;
			} else {
				BASIC_ASSERT(0); // file format error
			}
		}

		outputString[numberOfChar] = (unsigned char)ch;
		numberOfChar++;
	}

	outputString[numberOfChar] = 0; // End of string

	if (ch == EOF && numberOfChar == 0) {
		return EOF;
	} else {
		lineCount++;
		return numberOfChar;
	}
}

int LibFileIoClass::GetLine(OUT NextLineStyle_t *nextLineStyle /* = NULL */)
{
	return GetLineEx((u8 *)lineStr, lineBuffer.bufSize, &lineLen, nextLineStyle);
}

int LibFileIoClass::GetLineEx(unsigned char *outputString, int maxLength, OUT int *readLength, OUT NextLineStyle_t *nextLineStyle /* = NULL */)
{
	int ch;
	int numberOfChar = 0;

	outputString[0] = 0; // Clear string

	if (nextLineStyle != NULL)
		*nextLineStyle = NextLine_None;
	
	while (1) {
		if(numberOfChar >= maxLength-1) {
			break;
		}
		
		ch = fgetc(this->fp);
		if(ch == EOF) {
			break;
		} else if(ch == '\n') {
			if (nextLineStyle != NULL)
				*nextLineStyle = NextLine_Unix;
			break;
		} else if (ch == '\r') {
			ch = fgetc(this->fp);
			if(ch == '\n') {
				if (nextLineStyle != NULL)
					*nextLineStyle = NextLine_Win;
				break;
			} else {
				BASIC_ASSERT(0); // file format error
			}
		}

		outputString[numberOfChar] = (unsigned char)ch;
		numberOfChar++;
	}

	outputString[numberOfChar] = 0; // End of string
	if (numberOfChar == maxLength-1) {
		return RC_BUFFER_FULL;
	} else if (numberOfChar > maxLength-1) {
		BASIC_ASSERT(0);
	}

	*readLength = numberOfChar;
	
	if (ch == EOF && numberOfChar == 0) {
		return RC_FILE_REACH_EOF;
	} else {
		lineCount++;
		return 0;
	}
}

int LibFileIoClass::GetCharacter(void)
{
	return fgetc(this->fp);
}

int LibFileIoClass::FileScan(const char *format, ...)
{
	int retVal;

	scanCount = 0;
	
	va_list vl;
	va_start(vl, format);
	retVal = vfscanf(this->fp, format, vl);
	va_end(vl);

	if (retVal == EOF) {
		return RC_FILE_REACH_EOF;
	} else if (retVal == 0) {
		return RC_FILE_SCAN_ERROR;
	} else {
		scanCount = (u32)retVal;
		return 0;
	}
}

void LibFileIoClass_Demo_Output_A_File(void)
{
	LibFileIoClass outFile("LibFileIoClass_Demo_Output_A_File.txt", "w+b");

	outFile.FileOpen();

	fprintf(outFile.fp, "Hello: LibFileIoClass_Demo_Output_A_File\n");
}

void LibFileIoClass_Demo_Lite(void)
{

}

int LibFileIO_TestNextLine_InWinAndLinux(const char *testFileName)
{
	int retVal;
	
	PRINT_FUNC;

	LibFileIoClass testFile(testFileName, "r+b");

	retVal = testFile.FileOpen();
	DUMPX(retVal);

	LibBufferBasic obj;//(102400);
	obj.Init(1024);
	u8 *outStr = (u8 *)obj.bufPtr;
	NextLineStyle_t nextLineStyle;

	printf("[LINE] [LEN]\n");
	int lineCtr = 0;
	const char *nextLineStyleString;
	while (1) {
		retVal = testFile.GetLine(outStr, 1024, &nextLineStyle);
		if (retVal == 1023) {
			printf("Error, buffer too small. The length in single line is too long!! In line:%d\n", lineCtr);
			return RC_BUFFER_TOO_SMALL;
		}

		if (retVal == EOF)
			break;

		switch (nextLineStyle) {
			case NextLine_Win:
				nextLineStyleString = "\\r\\n";
			break;

			case NextLine_Unix:
				nextLineStyleString = "\\n";
			break;

			case NextLine_None:
				nextLineStyleString = "EOF";
			break;

			default:
				BASIC_ASSERT(0);
				break;
		}
		printf("[%4d] [%3d] [%4s] :%s\n", lineCtr, retVal, nextLineStyleString, outStr);
		lineCtr++;
	}

	return 0;
}

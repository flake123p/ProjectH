

#ifndef _LIB_FILE_IO_2_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================
#include <map>
#include <string>
#include "My_Types.h"
#include "LibFileIO.hpp"

#define VAR_MAP_TYPE std::map<std::string,std::string>

typedef struct {
	VAR_MAP_TYPE varMap;
	std::vector<std::string> singleVarVector;
}SECTION_CONTENT_t;

typedef int (*ExtProc_CB)(LibStringClass &str);
typedef int (*ExtProc_CB2)(LibStringClass &str, LibStringClass &rawStr, u32 lineNumber);

class LibFile_INI : public LibFileIoClass 
{
private:
	u32 section_name_index_counter_for_vector;
	bool _IsSector(std::string &str);
	int  _AddNewSector(std::string &str);
	int  _AddNewVarToMap(std::string &keyStr, std::string &valStr);
	int  _AddNewSingleVarToVector(std::string &valStr);
	
public:
	std::map<std::string,u32> mapSectionName;
	std::vector<SECTION_CONTENT_t> vecVarBlock;
	
	LibFile_INI(const char *inFileName = NULL, const char *inOpenMode = NULL) : LibFileIoClass(inFileName, inOpenMode)
	{
		ClearParameters();
	}

	int ClearParameters(void);
	int StartParse(bool ingoreInvalidLine = true, ExtProc_CB externalProcess = NULL);
	int StartParse2(bool ingoreInvalidLine = true, ExtProc_CB2 externalProcess2 = NULL);
	int GetValueString(const char *secName, const char *varName, OUT char *valStr);
	int GetValueString(const char *secName, const char *varName, OUT std::string &valStr);
	int GetValueString(std::string &secName, std::string &varName, OUT std::string &valStr);

	int GetValueU32(const char *secName, const char *varName, OUT u32 &val);
	int GetValueU32(std::string &secName, std::string &varName, OUT u32 &val);
	int GetValueS32(const char *secName, const char *varName, OUT s32 &val);
	int GetValueS32(std::string &secName, std::string &varName, OUT s32 &val);

	int GetSingleVarString(const char *secName, u32 index, OUT char *valStr);
	int GetSingleVarString(const char *secName, u32 index, OUT std::string &valStr);
	int GetSingleVarString(std::string &secName, u32 index, OUT std::string &valStr);

	int GetSingleVarStringTotalCount(const char *secName, OUT u32 &totalCount);
	int GetSingleVarStringTotalCount(std::string &secName, OUT u32 &totalCount);
	
	void Dump(void);
};

void LibFileIO2_Demo_INI_File(void);

#define _LIB_FILE_IO_2_HPP_INCLUDED_
#endif//_LIB_FILE_IO_2_HPP_INCLUDED_


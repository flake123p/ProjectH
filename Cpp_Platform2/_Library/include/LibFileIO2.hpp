

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

class LibFile_INI : public LibFileIoClass 
{
private:
	u32 section_name_index_counter_for_vector;
	bool _IsSector(std::string &str);
	int  _AddNewSector(std::string &str);
	int  _AddNewVarMap(std::string &keyStr, std::string &valStr);
	
public:
	std::map<std::string,u32> mapSectionName;
	std::vector<SECTION_CONTENT_t> vecVarBlock;
	
	LibFile_INI(const char *inFileName = NULL, const char *inOpenMode = NULL) : LibFileIoClass(inFileName, inOpenMode)
	{
		ClearParameters();
	}

	int ClearParameters(void);
	int StartParse(bool ingoreInvalidLine = true);
	int GetValueString(const char *secName, const char *varName, OUT char *valStr);
	int GetValueString(const char *secName, const char *varName, OUT std::string &valStr);
	int GetValueString(std::string &secName, std::string &varName, OUT std::string &valStr);

	int GetValueU32(const char *secName, const char *varName, OUT u32 &val);
	int GetValueU32(std::string &secName, std::string &varName, OUT u32 &val);
	int GetValueS32(const char *secName, const char *varName, OUT s32 &val);
	int GetValueS32(std::string &secName, std::string &varName, OUT s32 &val);

	void Dump(void);
};

void LibFileIO2_Demo_INI_File(void);

#define _LIB_FILE_IO_2_HPP_INCLUDED_
#endif//_LIB_FILE_IO_2_HPP_INCLUDED_


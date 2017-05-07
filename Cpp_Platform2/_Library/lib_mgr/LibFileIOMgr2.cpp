
#include "Everything_Lib_Mgr.hpp"

bool LibFile_INI::_IsSector(std::string &str)
{
	if (str[0] == '[' && str[str.size()-1] == ']') {
		return true;
	} else {
		return false;
	}
}

int LibFile_INI::_AddNewSector(std::string &str)
{
	BASIC_ASSERT(_IsSector(str) == true);

	std::pair<std::map<std::string,u32>::iterator,bool> ret;
	ret = mapSectionName.insert ( std::pair<std::string,u32>(str, section_name_index_counter_for_vector) );
	if (ret.second==false) {
		printf("Sector %s already existed\n", str.c_str());
		EXIT_LOC_IF(1);
	}

	section_name_index_counter_for_vector++;

	VAR_MAP_TYPE newMap;
	vecVarMap.push_back(newMap);

	return 0;
}

int LibFile_INI::_AddNewVarMap(std::string &keyStr, std::string &valStr)
{
	BASIC_ASSERT(section_name_index_counter_for_vector>0);

	u32 vecIndex = section_name_index_counter_for_vector - 1;
	
	std::pair<std::map<std::string,std::string>::iterator,bool> ret;
	ret = vecVarMap[vecIndex].insert ( std::pair<std::string,std::string>(keyStr, valStr) );
	if (ret.second==false) {
		printf("KEY %s already existed\n", keyStr.c_str());
		EXIT_LOC_IF(1);
	}

	return 0;
}

int LibFile_INI::ClearParameters(void)
{
	section_name_index_counter_for_vector = 0;
	mapSectionName.clear();
	vecVarMap.clear();

	return 0;
}

typedef enum {
	IPS_FINDING_SECTOR,
	IPS_EXTRACTING_VAR,
}INI_PARSE_STATE_t;
int LibFile_INI::StartParse(void)
{
	int retVal;

	RETURN_CHK( retVal, FileOpenForRead(1024) );

	LibStringClass str;

	INI_PARSE_STATE_t state = IPS_FINDING_SECTOR;
	while ( 0 == this->GetLine() ) 
	{
		// 1. clearify whole line
		str.Init(this->lineStr);
		str.RemoveRestString("//");
		str.RemoveRestString(";");
		str.RemoveEmptyPrefixChar();
		str.RemoveEmptyPostfixChar();

		// 2. skip empty line
		if (0 == str.Size()) {
			continue;
		}

		// 3. handle symbol of equal
		str.InsertBefore("=", " ");
		str.InsertAfter("=", " ");

		switch (state) {
			case IPS_FINDING_SECTOR: {
				if (_IsSector(str.str)) {
					_AddNewSector(str.str);
					state = IPS_EXTRACTING_VAR;
				} else {
					// drop it
				}
			} break;

			case IPS_EXTRACTING_VAR: {
				if (_IsSector(str.str)) {
					_AddNewSector(str.str);
				} else {
					str.Split(true);

					if (str.subStrVector.size() < 3) {
						goto ERROR_HANDLE;
					} else if (0 != str.subStrVector[1].compare("=")) {
						goto ERROR_HANDLE;
					} else {
						_AddNewVarMap(str.subStrVector[0], str.subStrVector[2]);
					}
				}
			} break;

			default:
				EXIT_LOC_IF(1);
				break;
		}
	}
	
	return 0;

ERROR_HANDLE:
	printf("Invalid variable string in line:%d\n", this->lineCount);
	printf(">> %s\n", str.CStr());
	EXIT_LOC_IF(1);
	return 1;
}

int LibFile_INI::GetValueString(const char *secName, const char *varName, OUT std::string &valStr)
{
	std::string s1 = secName;
	std::string s2 = varName;
	return GetValueString(s1, s2, valStr);
}

int LibFile_INI::GetValueString(std::string &secName, std::string &varName, OUT std::string &valStr)
{
	u32 vecIndex;

	std::map<std::string,u32>::iterator secMapIt;

	secMapIt = mapSectionName.find(secName);
	if (secMapIt == mapSectionName.end()) {
		LibError_SetExtErrorMessage("Can't find section: %s\n", secName.c_str());
		return 1;
	}

	vecIndex = secMapIt->second;

	std::map<std::string,std::string>::iterator varMapIt;
	
	varMapIt = vecVarMap[vecIndex].find(varName);
	if (varMapIt == vecVarMap[vecIndex].end()) {
		LibError_SetExtErrorMessage("Can't find variable %s in section: %s\n", varName.c_str(), secName.c_str());
		return 2;
	}

	valStr = varMapIt->second;
	return 0;
}

int LibFile_INI::GetValueU32(const char *secName, const char *varName, OUT u32 &val)
{
	std::string s1 = secName;
	std::string s2 = varName;
	return GetValueU32(s1, s2, val);
}

int LibFile_INI::GetValueU32(std::string &secName, std::string &varName, OUT u32 &val)
{
	std::string valStr;
	int retVal;

	RETURN_CHK( retVal, GetValueString(secName, varName, valStr) );

	if (valStr[0] == '0' && (valStr[1] == 'x' || valStr[1] == 'X'))
	{
		val = LibString_HexStringToU32(valStr.c_str());
	}
	else
	{
		val = (u32)LibString_DecStringToInt(valStr.c_str());
	}

	return 0;
}

int LibFile_INI::GetValueS32(const char *secName, const char *varName, OUT s32 &val)
{
	std::string s1 = secName;
	std::string s2 = varName;
	return GetValueS32(s1, s2, val);
}

int LibFile_INI::GetValueS32(std::string &secName, std::string &varName, OUT s32 &val)
{
	std::string valStr;
	int retVal;

	RETURN_CHK( retVal, GetValueString(secName, varName, valStr) );

	if (valStr[0] == '0' && (valStr[1] == 'x' || valStr[1] == 'X'))
	{
		val = LibString_HexStringToU32(valStr.c_str());
	}
	else
	{
		val = (s32)LibString_DecStringToInt(valStr.c_str());
	}

	return 0;
}

void LibFile_INI::Dump(void)
{
	std::map<std::string,u32>::iterator it;

	DUMPD(section_name_index_counter_for_vector);
	PRINT_NEXT_LINE;
	
	printf("mapSectionName:\n");
	for (it=mapSectionName.begin(); it!=mapSectionName.end(); it++) {
		printf("\t(%d) %s\n", it->second, it->first.c_str());
	}
	PRINT_NEXT_LINE;

	DUMPD(vecVarMap.size());
	EXIT_LOC_IF(vecVarMap.size() != section_name_index_counter_for_vector);
	for (u32 i = 0; i < section_name_index_counter_for_vector; i++) 
	{
		printf("\tvecVarMap[%d].size() = %d\n", i, vecVarMap[i].size());

		std::map<std::string,std::string>::iterator varMapIt;
		for (varMapIt=vecVarMap[i].begin(); varMapIt!=vecVarMap[i].end(); varMapIt++) 
		{
			printf("\t\t%-20s = %s\n", varMapIt->first.c_str(), varMapIt->second.c_str());
		}
	}
	PRINT_NEXT_LINE;
}

void LibFileIO2_Demo_INI_File(void)
{
	LibFile_INI ini_file("test.ini", "r+b");
	
	EXIT_CHK( rc, ini_file.StartParse() );

	ini_file.Dump();

	u32 val;
	
	EXIT_CHK( rc, ini_file.GetValueU32("[FW FilePointer]", "Mirror_Addr_2", val) );

	DUMPD(val);
	DUMPX(val);
}

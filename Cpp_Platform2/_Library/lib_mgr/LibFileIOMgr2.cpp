
#include "Everything_Lib_Mgr.hpp"

ExtProc_CB gExtProc_CB = NULL;
ExtProc_CB2 gExtProc_CB2 = NULL;

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

	SECTION_CONTENT_t newBlock;
	vecVarBlock.push_back(newBlock);

	return 0;
}

int LibFile_INI::_AddNewVarToMap(std::string &keyStr, std::string &valStr)
{
	BASIC_ASSERT(section_name_index_counter_for_vector>0);

	u32 vecIndex = section_name_index_counter_for_vector - 1;

	std::pair<std::map<std::string,std::string>::iterator,bool> ret;
	ret = vecVarBlock[vecIndex].varMap.insert ( std::pair<std::string,std::string>(keyStr, valStr) );
	if (ret.second==false) {
		LibError_SetExtErrorMessage("KEY %s already existed\n", keyStr.c_str());
		return 1;
	}

	return 0;
}

int LibFile_INI::_AddNewSingleVarToVector(std::string &valStr)
{
	BASIC_ASSERT(section_name_index_counter_for_vector>0);

	u32 vecIndex = section_name_index_counter_for_vector - 1;

	vecVarBlock[vecIndex].singleVarVector.push_back(valStr);

	return 0;
}

int LibFile_INI::ClearParameters(void)
{
	section_name_index_counter_for_vector = 0;
	mapSectionName.clear();
	//vecVarBlock.varMap.clear();
	//vecVarBlock.singleVarVector.clear();
	vecVarBlock.clear();

	return 0;
}

typedef enum {
	IPS_FINDING_SECTOR,
	IPS_EXTRACTING_VAR,
}INI_PARSE_STATE_t;
int LibFile_INI::StartParse(bool ingoreInvalidLine /* = true */, ExtProc_CB externalProcess /* = NULL */)
{
	int retVal;

	RETURN_CHK( retVal, FileOpenForRead(1024) );

	gExtProc_CB = externalProcess;

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
					if (gExtProc_CB != NULL) {
						int retVal = (*gExtProc_CB)(str);
						if (retVal)
							break;
					}

					str.Split(true);

					if (str.subStrVector.size() == 1) {
						_AddNewSingleVarToVector(str.subStrVector[0]);
					} else if (str.subStrVector.size() < 3) {
						if (ingoreInvalidLine)
							break;
						else
							goto ERROR_HANDLE;
					} else if (0 != str.subStrVector[1].compare("=")) {
						if (ingoreInvalidLine)
							break;
						else
							goto ERROR_HANDLE;
					} else {
						retVal = _AddNewVarToMap(str.subStrVector[0], str.subStrVector[2]);
						if (retVal) {
							if (ingoreInvalidLine)
								break;
							else
								goto ERROR_HANDLE;
						}
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

int LibFile_INI::StartParse2(bool ingoreInvalidLine /* = true */, ExtProc_CB2 externalProcess2 /* = NULL */)
{
	int retVal;

	RETURN_CHK( retVal, FileOpenForRead(1024) );

	gExtProc_CB2 = externalProcess2;

	LibStringClass str;
	LibStringClass rawStr;

	INI_PARSE_STATE_t state = IPS_FINDING_SECTOR;
	while ( 0 == this->GetLine() )
	{
		// 1. clearify whole line.
		rawStr.Init(this->lineStr);
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
					if (gExtProc_CB2 != NULL) {
						int retVal = (*gExtProc_CB2)(str, rawStr, this->lineCount);
						if (retVal)
							break;
					}

					str.Split(true);

					if (str.subStrVector.size() == 1) {
						_AddNewSingleVarToVector(str.subStrVector[0]);
					} else if (str.subStrVector.size() < 3) {
						if (ingoreInvalidLine)
							break;
						else
							goto ERROR_HANDLE;
					} else if (0 != str.subStrVector[1].compare("=")) {
						if (ingoreInvalidLine)
							break;
						else
							goto ERROR_HANDLE;
					} else {
						retVal = _AddNewVarToMap(str.subStrVector[0], str.subStrVector[2]);
						if (retVal) {
							if (ingoreInvalidLine)
								break;
							else
								goto ERROR_HANDLE;
						}
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

int LibFile_INI::GetValueString(const char *secName, const char *varName, OUT char *valStr)
{
	std::string tempValStr;

	int retVal = GetValueString(secName, varName, tempValStr);

	if (0 == retVal) {
		strcpy(valStr, tempValStr.c_str());
	}

	return retVal;
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

	varMapIt = vecVarBlock[vecIndex].varMap.find(varName);
	if (varMapIt == vecVarBlock[vecIndex].varMap.end()) {
		LibError_SetExtErrorMessage("Can't find variable %s in section: %s\n", varName.c_str(), secName.c_str());
		return 2;
	}

	valStr = varMapIt->second;
	return 0;
}

int LibFile_INI::GetSingleVarString(const char *secName, u32 index, OUT char *valStr)
{
	std::string tempValStr;

	int retVal = GetSingleVarString(secName, index, tempValStr);

	if (0 == retVal) {
		strcpy(valStr, tempValStr.c_str());
	}

	return retVal;
}

int LibFile_INI::GetSingleVarString(const char *secName, u32 index, OUT std::string &valStr)
{
	std::string s1 = secName;
	return GetSingleVarString(s1, index, valStr);
}

int LibFile_INI::GetSingleVarString(std::string &secName, u32 index, OUT std::string &valStr)
{
	u32 vecIndex;

	std::map<std::string,u32>::iterator secMapIt;

	secMapIt = mapSectionName.find(secName);
	if (secMapIt == mapSectionName.end()) {
		LibError_SetExtErrorMessage("Can't find section: %s\n", secName.c_str());
		return 1;
	}

	vecIndex = secMapIt->second;


	if (index >= vecVarBlock[vecIndex].singleVarVector.size() ) {
		return 2; //index too big
	}

	valStr = vecVarBlock[vecIndex].singleVarVector[index];
	return 0;
}

int LibFile_INI::GetSingleVarStringTotalCount(const char *secName, OUT u32 &totalCount)
{
	std::string s1 = secName;
	return GetSingleVarStringTotalCount(s1, totalCount);
}

int LibFile_INI::GetSingleVarStringTotalCount(std::string &secName, OUT u32 &totalCount)
{
	u32 vecIndex;

	std::map<std::string,u32>::iterator secMapIt;

	secMapIt = mapSectionName.find(secName);
	if (secMapIt == mapSectionName.end()) {
		LibError_SetExtErrorMessage("Can't find section: %s\n", secName.c_str());
		return 1;
	}

	vecIndex = secMapIt->second;

	totalCount = vecVarBlock[vecIndex].singleVarVector.size();

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

	DUMPD(vecVarBlock.size());
	EXIT_LOC_IF(vecVarBlock.size() != section_name_index_counter_for_vector);
	for (u32 i = 0; i < section_name_index_counter_for_vector; i++)
	{
		printf("\tvecVarBlock[%u].varMap.size() = %u\n", i, (unsigned int)vecVarBlock[i].varMap.size());

		std::map<std::string,std::string>::iterator varMapIt;
		for (varMapIt=vecVarBlock[i].varMap.begin(); varMapIt!=vecVarBlock[i].varMap.end(); varMapIt++)
		{
			printf("\t\t%-20s = %s\n", varMapIt->first.c_str(), varMapIt->second.c_str());
		}

		printf("\tvecVarBlock[%u].singleVarVector.size() = %u\n", i, (unsigned int)vecVarBlock[i].singleVarVector.size());

		std::vector<std::string>::iterator varMapIt2;
		for (varMapIt2=vecVarBlock[i].singleVarVector.begin(); varMapIt2!=vecVarBlock[i].singleVarVector.end(); varMapIt2++)
		{
			printf("\t\t%-20s\n", varMapIt2->c_str());
		}
		PRINT_NEXT_LINE;
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

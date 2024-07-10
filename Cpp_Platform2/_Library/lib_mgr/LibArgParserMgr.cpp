
#include "Everything_Lib_Mgr.hpp"

static bool gDoesCharOptionExist = false;
/*
	'A' = 0x41 (65)
	'a' = 0x61 (97)
*/
static int  gCharOption[52 + 10] = {0};

/*
	return true if option exists.
*/
static bool _CharOptionParser(char *str, const char *legalCharAry, int aryLen)
{
	// Skip 1st char, which is '-'
	str++;

	/*
	if (false == LibString_IsStringAllLetter(str)) {
		return false;
	}
	*/

	int index;
	do {
		if (legalCharAry != NULL) {
			if (false == LibString_IsCharLegal(*str, legalCharAry, aryLen)) {
				gDoesCharOptionExist = false;
				return false;
			}
		}
		
		LibString_CharToIndex(*str, true, &index);

		gCharOption[index] = 1;
		gDoesCharOptionExist = true;
		str++;
	} while (0 != *str);

	return true;
}

void LibArgParser_ClearCharOption(void)
{
	gDoesCharOptionExist = false;
	memset(gCharOption, 0, 52 * sizeof(int));
}

/*
	return true if option exists.
*/
bool LibArgParser_CharOptionParser(int argc, char *argv[], const char *legalCharAry /* = NULL */, int aryLen /* = 0 */)
{
	bool doesOptionExist = false;
	
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			doesOptionExist = _CharOptionParser(argv[i], legalCharAry, aryLen);
			if (false == doesOptionExist) {
				return false;
			}
		}
	}

	return true;
}

bool LibArgParser_CharOptionParserEx(int argc, char *argv[], const char *legalCharAry)
{
	int aryLen = strlen(legalCharAry);

	return LibArgParser_CharOptionParser(argc, argv, legalCharAry, aryLen);
}

bool LibArgParser_CheckCharOption(char ch)
{
	if (false == gDoesCharOptionExist) {
		return false;
	}

	int index;
	if (false == LibString_CharToIndex(ch, true, &index)) {
		return false;
	}

	if (gCharOption[index])
		return true;
	else
		return false;
}

bool LibArgParser_DoesCharOptionExist(void)
{
	return gDoesCharOptionExist;
}

bool LibArgParser_ArgvsToVariables(ARG_TYPE_DB_t *db_buf, u32 count, char *argv[], u32 argv_start_index, OUT u32 *error_index)
{
	int retVal;
	char *strPtr;
	u32  *u32Ptr;
	u32 temp; //for sscanf
	u32 cur_index;
	bool doSscanf;
	const char *sscanfType;
	
	for (u32 i = 0; i < count; i++) {
		cur_index = argv_start_index + i;
		strPtr = (char *)(db_buf[i].ptr);
		u32Ptr = (u32 *)(db_buf[i].ptr);
		doSscanf = false;
		
		switch (db_buf[i].type) {
			case ARG_TYPE_STRING:
				LibString_Copy(strPtr, argv[cur_index]);
			break;
			
			case ARG_TYPE_DEC:
				sscanfType = "%d";
				doSscanf = true;
			break;

			case ARG_TYPE_HEX_WITH_0x:
				sscanfType = "0x%x";
				doSscanf = true;
			break;

			case ARG_TYPE_HEX_WITHOUT_0x:
				sscanfType = "%x";
				doSscanf = true;
			break;

			default:
				BASIC_ASSERT(0);
			break;
		}

		if (doSscanf) {
			retVal = sscanf(argv[cur_index], sscanfType, &temp);
			if (retVal != 1) {
				*error_index = cur_index;
				return false;
			} else {
				*u32Ptr = temp;
			}
		}
	}
	
	return true;
}


ArgOptionSet::ArgOptionSet(Option_Set_Big_t *big_set /* = NULL */, u32 big_set_len /* = 0 */, Option_Set_Small_t * small_set /* = NULL */, u32 small_set_len /* = 0 */, u32 stand_alone_str_size /* = 0 */)
{
	Init(big_set, big_set_len, small_set, small_set_len, stand_alone_str_size);
}

void ArgOptionSet::Init(Option_Set_Big_t *big_set, u32 big_set_len, Option_Set_Small_t * small_set, u32 small_set_len, u32 stand_alone_str_size /* = 0 */)
{
	bigSet      = big_set;
	bigSetLen   = big_set_len;
	smallSet    = small_set;
	smallSetLen = small_set_len;
	standAloneArgSize = stand_alone_str_size;

	argMap.clear();
	standAloneArgs.clear();
}

void ArgOptionSet::Dump(void)
{
	printf("=============== ARG DUMP START ============= \\\\\\\\\\\\\n");
	printf("Dump big set:\n");
	for (u32 i = 0; i < bigSetLen; i++) {
		printf(
			"%2d %2d %-12s %-12s %-12s %-12s => %s\n", 
			bigSet[i].keyValue,
			bigSet[i].followingArgType,
			(bigSet[i].keyStr1==NULL?"":bigSet[i].keyStr1),
			(bigSet[i].keyStr2==NULL?"":bigSet[i].keyStr2),
			(bigSet[i].keyStr3==NULL?"":bigSet[i].keyStr3),
			(bigSet[i].keyStr4==NULL?"":bigSet[i].keyStr4),
			(bigSet[i].description==NULL?"":bigSet[i].description));
	}

	printf("Dump small set:\n");
	for (u32 i = 0; i < smallSetLen; i++) {
		printf("%2d %c\n", smallSet[i].keyValue, smallSet[i].keyCharacter);
	}

	printf("Dump arg map:\n");
	std::map<u32,std::string>::iterator it;
	for (it=argMap.begin(); it!=argMap.end(); ++it) {
		printf("%2u, %s\n", it->first, it->second.c_str());
	}

	DUMPD(standAloneArgSize);
	printf("Dump stand alone argu:\n");
	for (u32 i = 0; i < standAloneArgs.size(); i++) {
		printf("%s\n", standAloneArgs[i].c_str());
	}

	printf("=============== ARG DUMP END =============== //////\n\n");
}

int ArgOptionSet::StartParsing(int argc, char *argv[])
{
	int retVal;
	char *currArgv;
	char *nextArgv;
	bool passNextRound;
	
	for (int argcCtr=0; argcCtr<argc; argcCtr++) {
		currArgv = argv[argcCtr];
		if (argcCtr+1 < argc)
			nextArgv = argv[argcCtr+1];
		else
			nextArgv = NULL;

		retVal = _ExtractToMap(currArgv, nextArgv, &passNextRound);
		if (retVal) {
			printf("argcCtr=%d\n", argcCtr+1);
			return retVal;
		}

		if(passNextRound)
			argcCtr++;
	}
	return 0;
}

bool ArgOptionSet::CheckArgByKey(u32 key, OUT const char **followingStr /* = NULL */)
{
	std::map<u32,std::string>::iterator it = argMap.find(key);
	if (it == argMap.end()) {
		return false;
	} else {
		if (followingStr != NULL)
			*followingStr = it->second.c_str();
		return true;
	}
}

bool ArgOptionSet::CheckArgByKey_StrToDec(u32 key, OUT int *followingDec /* = NULL */)
{
	const char *followingStr = NULL;

	bool retVal;

	retVal = CheckArgByKey(key, &followingStr);

	if (retVal && followingDec != NULL && followingStr != NULL) {
		*followingDec = LibString_DecStringToInt(followingStr);
	}

	return retVal;
}

int ArgOptionSet::_ExtractToMap(char *currArgv, char *nextArgv, bool *passNextRound)
{
	*passNextRound = false;
	bool matchSuccess = false;
	u32 str_offset_for_arg_concat = 0;
	
	//printf("%s %s\n", currArgv, (nextArgv==NULL?"":nextArgv));

	for (u32 i = 0; i < bigSetLen; i++) {
		matchSuccess = false;

		switch (bigSet[i].followingArgType) {
			case ARG_CONCAT: {
				if (bigSet[i].keyStr1 != NULL && 0 == strncmp(bigSet[i].keyStr1, currArgv, strlen(bigSet[i].keyStr1))) {
					matchSuccess = true;
					str_offset_for_arg_concat = strlen(bigSet[i].keyStr1);
				}
				else if (bigSet[i].keyStr2 != NULL && 0 == strncmp(bigSet[i].keyStr2, currArgv, strlen(bigSet[i].keyStr2))) {
					matchSuccess = true;
					str_offset_for_arg_concat = strlen(bigSet[i].keyStr2);
				}
				else if (bigSet[i].keyStr3 != NULL && 0 == strncmp(bigSet[i].keyStr3, currArgv, strlen(bigSet[i].keyStr3))) {
					matchSuccess = true;
					str_offset_for_arg_concat = strlen(bigSet[i].keyStr3);
				}
				else if (bigSet[i].keyStr4 != NULL && 0 == strncmp(bigSet[i].keyStr4, currArgv, strlen(bigSet[i].keyStr4))) {
					matchSuccess = true;
					str_offset_for_arg_concat = strlen(bigSet[i].keyStr4);
				}
			} break;

			default: {
				if (bigSet[i].keyStr1 != NULL && 0 == strcmp(bigSet[i].keyStr1, currArgv))
					matchSuccess = true;

				if (bigSet[i].keyStr2 != NULL && 0 == strcmp(bigSet[i].keyStr2, currArgv))
					matchSuccess = true;

				if (bigSet[i].keyStr3 != NULL && 0 == strcmp(bigSet[i].keyStr3, currArgv))
					matchSuccess = true;

				if (bigSet[i].keyStr4 != NULL && 0 == strcmp(bigSet[i].keyStr4, currArgv))
					matchSuccess = true;
			} break;
		}

		if (matchSuccess)
		{
			//printf("!! %s\n", currArgv);

			std::string tempStr;

			switch (bigSet[i].followingArgType) {
				case ARG_BY_SPACE: {
					if (nextArgv == NULL) {
						printf("Need more argument after: %s\n", currArgv);
						return 1;
					} else {
						tempStr = nextArgv;
					}
					*passNextRound = true;
				} break;

				case ARG_CONCAT: {
					tempStr = ADDRX(currArgv, str_offset_for_arg_concat);
				} break;

				default:
					tempStr = "";
					break;
			}

			std::pair<std::map<u32,std::string>::iterator,bool> ret;
			ret = argMap.insert ( std::pair<u32,std::string>(bigSet[i].keyValue, tempStr) );
			if (ret.second==false) {
				printf("Option string redefine in argument: %s\n", currArgv);
				return 2;
			}
			
			return 0;
		}
	}

	if (currArgv[0] == '-') {
		u32 subStrLen = strlen(currArgv);
		DUMPD(subStrLen);
		for (u32 i=1; i<subStrLen; i++) {
			matchSuccess = false;
			for (u32 j=0; j<smallSetLen; j++) {
				//printf("%2d %c\n", smallSet[i].keyValue, smallSet[i].keyCharacter);
				if (currArgv[i] == smallSet[j].keyCharacter) {
					matchSuccess = true;

					std::string tempStr = "";
					std::pair<std::map<u32,std::string>::iterator,bool> ret;
					ret = argMap.insert ( std::pair<u32,std::string>(smallSet[j].keyValue, tempStr) );
					if (ret.second==false) {
						printf("Option character redefine in argument: %s, location:%d\n", currArgv, j);
						return 3;
					}
					break;
				}
			}
			if (matchSuccess == false) {
				printf("Unknown option in argument: %s, location:%d\n", currArgv, i);
				return 3;
			}
		}
		return 0;
	}

	if (standAloneArgSize > standAloneArgs.size()) {
		std::string tempStr = currArgv;
		standAloneArgs.push_back(tempStr);
		return 0;
	}
	
	printf("Unknown option: %s\n", currArgv);
	return 10;
}

#define ARGC (9)
static int myArgc = ARGC;
static char *myArgv[ARGC];
static char argv0[] = "dummy";
static char argv1[] = "-i";
static char argv2[] = "in.txt";
static char argv3[] = "first.txt";
static char argv4[] = "--output";
static char argv5[] = "out.txt";
static char argv6[] = "-abc";
static char argv7[] = "-g3";
static char argv8[] = "-xy";
Option_Set_Big_t gDemo_BigOptionSet[] = {
	{1, ARG_BY_SPACE, "-i", "--input", NULL, NULL, NULL},
	{2, ARG_BY_SPACE, "-o", "--output", NULL, NULL, NULL},
	{3, ARG_CONCAT,   "-g", NULL, NULL, NULL, NULL},
};
Option_Set_Small_t gDemo_SmallOptionSet[] = {
	{5, 'a'},
	{6, 'b'},
	{7, 'c'},
	{8, 'x'},
	{9, 'y'},
};
void LibArgParser_OptionSet_Demo(void)
{
	myArgv[0]=argv0;
	myArgv[1]=argv1;
	myArgv[2]=argv2;
	myArgv[3]=argv3;
	myArgv[4]=argv4;
	myArgv[5]=argv5;
	myArgv[6]=argv6;
	myArgv[7]=argv7;
	myArgv[8]=argv8;
	
	//ARRAYDUMPS(myArgv, myArgc);
	
	ArgOptionSet obj;
	obj.Init(ARRAY_AND_SIZE(gDemo_BigOptionSet), ARRAY_AND_SIZE(gDemo_SmallOptionSet), 1);
	

	rc = obj.StartParsing(myArgc-1, &myArgv[1]);
	LibError_PrintExtErrorMessage("[EXT ERROR]:");
	//LibError_PrintErrorMessage(rc, true);

	obj.Dump();

	const char *followingStr = "XXX";

	if (obj.CheckArgByKey(1) && obj.standAloneArgs.size()>0) {
		printf("Redundent argu: %s\n", obj.standAloneArgs[0].c_str());
	}

	obj.CheckArgByKey(1, &followingStr);
	DUMPS(followingStr);
}



#ifndef _LIB_ARG_PARSER_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================

// ============================== Library: Cross-Platform (Manager) ==============================
#include "My_Types.h"

typedef enum {
	ARG_TYPE_STRING = 0,
	ARG_TYPE_DEC,
	ARG_TYPE_HEX_WITH_0x,
	ARG_TYPE_HEX_WITHOUT_0x,

	ARG_TYPE_INVALID,
} ARG_TYPE_t;

typedef struct {
	ARG_TYPE_t  type;
	void        *ptr;
} ARG_TYPE_DB_t;

// For minimizing the global variable uses
void LibArgParser_ClearCharOption(void);
bool LibArgParser_CharOptionParser(int argc, char *argv[], const char *legalCharAry = NULL, int aryLen = 0);
bool LibArgParser_CharOptionParserEx(int argc, char *argv[], const char *legalCharAry);
bool LibArgParser_CheckCharOption(char ch);
bool LibArgParser_DoesCharOptionExist(void);
bool LibArgParser_ArgvsToVariables(ARG_TYPE_DB_t *db_buf, u32 count, char *argv[], u32 argv_start_index, OUT u32 *error_index);

typedef enum {
	NO_ARG,
	ARG_BY_SPACE,
	ARG_CONCAT,
} FOLLOWING_ARG_TYPES;
typedef struct {
	u32 keyValue;
	FOLLOWING_ARG_TYPES followingArgType;
	const char *keyStr1;
	const char *keyStr2;
	const char *keyStr3;
	const char *keyStr4;
	const char *description;
} Option_Set_Big_t;
typedef struct {
	u32 keyValue;
	const char keyCharacter;
} Option_Set_Small_t;
#include <string.h>
#include <map>
#include <vector>
class ArgOptionSet {
public:
	Option_Set_Big_t *bigSet;
	u32 bigSetLen;
	Option_Set_Small_t *smallSet;
	u32 smallSetLen;
	u32 standAloneArgSize;
	
	std::map<u32,std::string> argMap;
	std::vector<std::string> standAloneArgs;
	
	ArgOptionSet(Option_Set_Big_t *big_set = NULL, u32 big_set_len = 0, Option_Set_Small_t * small_set = NULL, u32 small_set_len = 0, u32 stand_alone_str_size = 0);
	~ArgOptionSet(void){}; // do nothing
	void Init(Option_Set_Big_t *big_set, u32 big_set_len, Option_Set_Small_t * small_set, u32 small_set_len, u32 stand_alone_str_size = 0);
	void Dump(void);
	int  StartParsing(int argc, char *argv[]);
	bool CheckArgByKey(u32 key, OUT const char **followingStr = NULL);
	bool CheckArgByKey_StrToDec(u32 key, OUT int *followingDec = NULL);
private:
	int  _ExtractToMap(char *currArgv, char *nextArgv, bool *passNextRound);
};
void LibArgParser_OptionSet_Demo(void);
// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================


#define _LIB_ARG_PARSER_HPP_INCLUDED_
#endif//_LIB_ARG_PARSER_HPP_INCLUDED_



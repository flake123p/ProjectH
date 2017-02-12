

#ifndef _LIB_ARG_PARSER_HPP_INCLUDED_

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

// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================


#define _LIB_ARG_PARSER_HPP_INCLUDED_
#endif//_LIB_ARG_PARSER_HPP_INCLUDED_




#include "Everything_Lib_Mgr.hpp"

int rc = 0;

#define TO_STR(a) {a, #a}

typedef struct {
	int returnCode;
	const char *returnCodeString;
} ERROR_MESSAGE_t;
ERROR_MESSAGE_t gErrorMessage[] = {
	TO_STR(RC_MEMORY_ALLOCATE_FAILED),
	TO_STR(RC_BUFFER_FULL),
	TO_STR(RC_BUFFER_OVERFLOW),
	TO_STR(RC_BUFFER_TOO_SMALL),

	TO_STR(RC_FILE_REACH_EOF),
	TO_STR(RC_FILE_OPEN_ERROR),
	TO_STR(RC_FILE_CLOSE_ERROR),
	TO_STR(RC_FILE_SCAN_ERROR),
};
u32 gErrorMessageLen = SIZE_OF_ARRAY(gErrorMessage);

void LibError_PrintErrorMessage(int retVal, bool verbose /* = false */)
{
	//DUMPD(gErrorMessageLen);
	for (u32 i=0; i<gErrorMessageLen; i++) {
		if (retVal == gErrorMessage[i].returnCode) {
			printf("[Error Message] : %s\n", gErrorMessage[i].returnCodeString);
			return;
		}
	}

	if (retVal != 0) {
		printf("[Error Message] : Return Code = %d\n", retVal);
	} else {
		if (verbose)
			printf("[Error Message] : No Error!! Return Code = %d\n", retVal);
	}
}

void LibError_Demo(void)
{
	LibError_PrintErrorMessage(RC_FILE_REACH_EOF);
}
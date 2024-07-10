#include "Everything_App.hpp"
#include "LineReadMachine.hpp"
#include "LRM.hpp"

LibFileIoClass gFileObj;

int LRM_FileOpen(LRM_t state, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{PRINT_FUNC;
	gFileObj.openMode = "r+b";
	if ( gFileObj.FileOpenForRead(4096) )
		return LRM_FileOpen_NG;

	return LRM_FileOpen_OK;
}

int LRM_LineRead(LRM_t state, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{PRINT_FUNC;
	int retVal = gFileObj.GetLine();

	if (RC_FILE_REACH_EOF == retVal)
		return LRM_LineRead_EOF;
	else if (RC_FILE_SCAN_ERROR == retVal)
		return LRM_LineRead_NG;

	return LRM_LineRead_OK; // Error Code, true for error.
}

int LRM_FileClose(LRM_t state, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{PRINT_FUNC;
	gFileObj.FileClose();
	return 0;
}

LRM_t LRM_SetInputFile(LRM_t &state, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{PRINT_FUNC;
	gFileObj.fileName = (*evtPara).inputFileName;
	return state;
}

LRM_t LRM_GetLineObj(LRM_t &state, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{PRINT_FUNC;
	(*evtPara).lineObj = &gFileObj;
	return state;
}


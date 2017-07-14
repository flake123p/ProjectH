
#include "LineReadMachine.hpp"

//LRM_EVT_EvtPara_t gLRM_EvtPara;
LibFileIoClass gFileObj;

int LRM_S0_Init_Function(LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	return 0; // Error Code, true for error.
}

int LRM_S1_File_Opened_Function(LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	gFileObj.openMode = "r+b";
	if ( gFileObj.FileOpenForRead(4096) )
		return LRM_EC_File_Open_Failed;

	return 0; // Error Code, true for error.
}

int LRM_S2_Line_Read_Function(LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	int retVal = gFileObj.GetLine();

	if (RC_FILE_REACH_EOF == retVal)
		return LRM_EC_End_Of_File;

	if (retVal)
		return retVal;

	return 0; // Error Code, true for error.
}

int LRM_S3_Exit_Function(LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	return 0; // Error Code, true for error.
}

int LRM_EVT_E3_GetLineObj_Function(LRM_t &state, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	switch (state) {
		case LRM_S0_Init:
		case LRM_S1_File_Opened:
		case LRM_S2_Line_Read:
		case LRM_S3_Exit:
		default:
			break;
	}

	(*evtPara).lineObj = &gFileObj;
	return 0; // Error Code, true for error.
}
int LRM_EVT_E4_SetInputFile_Function(LRM_t &state, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	switch (state) {
		case LRM_S0_Init:
		case LRM_S1_File_Opened:
		case LRM_S2_Line_Read:
		case LRM_S3_Exit:
		default:
			break;
	}

	gFileObj.fileName = (*evtPara).inputFileName;
	
	return 0; // Error Code, true for error.
}

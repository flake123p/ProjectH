
#include "LineReadMachine.hpp"

int LRM_S2_File_Open_OK_Entry(LRM_t oldState, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{PRINT_FUNC;
	return 0;
}

int LRM_S2_File_Open_OK_Leave(LRM_t newState, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{PRINT_FUNC;
	return 0;
}

int LRM_S3_File_Open_NG_Entry(LRM_t oldState, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	return 0;
}

int LRM_S3_File_Open_NG_Leave(LRM_t newState, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	return 0;
}

int LRM_S4_iLine_Reading_Leave(LRM_t newState, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	return 0;
}

int LRM_S5_Line_Read_OK_Entry(LRM_t oldState, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	return 0;
}

int LRM_S5_Line_Read_OK_Leave(LRM_t newState, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	return 0;
}

int LRM_S7_Line_Read_NG_Entry(LRM_t oldState, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	return 0;
}

int LRM_S7_Line_Read_NG_Leave(LRM_t newState, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	return 0;
}

int LRM_S8_Line_Read_EOF_Entry(LRM_t oldState, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{PRINT_FUNC;
	return 0;
}

int LRM_S8_Line_Read_EOF_Leave(LRM_t newState, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{PRINT_FUNC;
	return 0;
}

LRM_t LRM_EVT_E4_TestGEvent_2_Function(LRM_t &state, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	return state;
}


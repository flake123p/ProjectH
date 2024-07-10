
#include "LineReadMachine.hpp"

LRM_t  gLRM = LRM_S0_Init;

int LRM_StateChange(LRM_t nextState, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	switch (nextState) {
		case LRM_S0_Init:
			gLRM = nextState;
			return LRM_S0_Init_Function(evtPara);

		case LRM_S1_File_Opened:
			gLRM = nextState;
			return LRM_S1_File_Opened_Function(evtPara);

		case LRM_S2_Line_Read:
			gLRM = nextState;
			return LRM_S2_Line_Read_Function(evtPara);

		case LRM_S3_Exit:
			gLRM = nextState;
			return LRM_S3_Exit_Function(evtPara);

		default:
			return -1;
	}
}

int LRM_StateMachine(LRM_EVT_t event, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	int retVal;

	switch (event) {
		case LRM_EVT_E3_GetLineObj:
			return LRM_EVT_E3_GetLineObj_Function(gLRM, evtPara);
		case LRM_EVT_E4_SetInputFile:
			return LRM_EVT_E4_SetInputFile_Function(gLRM, evtPara);
		default:
			break;
	}

	bool repeat;
	do {
		repeat = false;

		switch (gLRM) {
			case LRM_S0_Init: {
				switch (event) {
					case LRM_EVT_E0_OpenFile: {
						retVal = LRM_StateChange(LRM_S1_File_Opened, evtPara);
						if (retVal) {
							return retVal;
						}
					} break;

					case LRM_EVT_E1_ReadLine: {
						repeat = true;
						retVal = LRM_StateChange(LRM_S1_File_Opened, evtPara);
						if (retVal) {
							return retVal;
						}
					} break;

					case LRM_EVT_E2_Exit: {
						retVal = LRM_StateChange(LRM_S3_Exit, evtPara);
						if (retVal) {
							return retVal;
						}
					} break;

					case LRM_EVT_E3_GetLineObj: {
					} break;

					case LRM_EVT_E4_SetInputFile: {
					} break;

					default: 
						break;
				}
			} break;

			case LRM_S1_File_Opened: {
				switch (event) {
					case LRM_EVT_E0_OpenFile: {
					} break;

					case LRM_EVT_E1_ReadLine: {
						retVal = LRM_StateChange(LRM_S2_Line_Read, evtPara);
						if (retVal) {
							return retVal;
						}
					} break;

					case LRM_EVT_E2_Exit: {
						retVal = LRM_StateChange(LRM_S3_Exit, evtPara);
						if (retVal) {
							return retVal;
						}
					} break;

					case LRM_EVT_E3_GetLineObj: {
					} break;

					case LRM_EVT_E4_SetInputFile: {
					} break;

					default: 
						break;
				}
			} break;

			case LRM_S2_Line_Read: {
				switch (event) {
					case LRM_EVT_E0_OpenFile: {
					} break;

					case LRM_EVT_E1_ReadLine: {
						retVal = LRM_StateChange(LRM_S2_Line_Read, evtPara);
						if (retVal) {
							return retVal;
						}
					} break;

					case LRM_EVT_E2_Exit: {
						retVal = LRM_StateChange(LRM_S3_Exit, evtPara);
						if (retVal) {
							return retVal;
						}
					} break;

					case LRM_EVT_E3_GetLineObj: {
					} break;

					case LRM_EVT_E4_SetInputFile: {
					} break;

					default: 
						break;
				}
			} break;

			case LRM_S3_Exit: {
				switch (event) {
					case LRM_EVT_E0_OpenFile: {
					} break;

					case LRM_EVT_E1_ReadLine: {
					} break;

					case LRM_EVT_E2_Exit: {
					} break;

					case LRM_EVT_E3_GetLineObj: {
					} break;

					case LRM_EVT_E4_SetInputFile: {
					} break;

					default: 
						break;
				}
			} break;

			default:
				break;
		}
	} while (repeat);

	return retVal;
}

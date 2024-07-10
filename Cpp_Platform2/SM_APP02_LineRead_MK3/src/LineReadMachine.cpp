
#include "LineReadMachine.hpp"

LRM_t  gLrm = LRM_S0_Init;

State_Row_t gSE_Table[] = {
//                                            LRM_EVT_E0_OpenFile                  LRM_EVT_E1_ReadLine                  LRM_EVT_E2_Exit                      
/* LRM_S0_Init           */ { SimpleState, { {DirectPass, LRM_S1_iFile_Opening }, {RepeatPass, LRM_S1_iFile_Opening }, {DirectPass, LRM_S9_Exit          }, }, },
/* LRM_S1_iFile_Opening  */ { InterState , { {NullPass,   LRM_DUMMY            }, {NullPass,   LRM_DUMMY            }, {NullPass,   LRM_DUMMY            }, }, },
/* LRM_S2_File_Open_OK   */ { SimpleState, { {NullPass,   LRM_DUMMY            }, {DirectPass, LRM_S4_iLine_Reading }, {DirectPass, LRM_S9_Exit          }, }, },
/* LRM_S3_File_Open_NG   */ { SimpleState, { {NullPass,   LRM_DUMMY            }, {NullPass,   LRM_DUMMY            }, {DirectPass, LRM_S9_Exit          }, }, },
/* LRM_S4_iLine_Reading  */ { InterState , { {NullPass,   LRM_DUMMY            }, {NullPass,   LRM_DUMMY            }, {NullPass,   LRM_DUMMY            }, }, },
/* LRM_S5_Line_Read_OK   */ { SimpleState, { {NullPass,   LRM_DUMMY            }, {DirectPass, LRM_S4_iLine_Reading }, {DirectPass, LRM_S9_Exit          }, }, },
/* LRM_S6_Line_Read_LAST */ { SimpleState, { {NullPass,   LRM_DUMMY            }, {DirectPass, LRM_S8_Line_Read_EOF }, {DirectPass, LRM_S9_Exit          }, }, },
/* LRM_S7_Line_Read_NG   */ { SimpleState, { {NullPass,   LRM_DUMMY            }, {NullPass,   LRM_DUMMY            }, {DirectPass, LRM_S9_Exit          }, }, },
/* LRM_S8_Line_Read_EOF  */ { SimpleState, { {NullPass,   LRM_DUMMY            }, {NullPass,   LRM_DUMMY            }, {DirectPass, LRM_S9_Exit          }, }, },
/* LRM_S9_Exit           */ { SimpleState, { {NullPass,   LRM_DUMMY            }, {RepeatPass, LRM_S0_Init          }, {NullPass,   LRM_DUMMY            }, }, },
};

State_CB_t gState_CB_Table[] = {
	{NULL,                          NULL                          }, // LRM_S0_Init
	{LRM_FileOpen,                  NULL                          }, // LRM_S1_iFile_Opening
	{LRM_S2_File_Open_OK_Entry,     LRM_S2_File_Open_OK_Leave     }, // LRM_S2_File_Open_OK
	{LRM_S3_File_Open_NG_Entry,     LRM_S3_File_Open_NG_Leave     }, // LRM_S3_File_Open_NG
	{LRM_LineRead,                  LRM_S4_iLine_Reading_Leave    }, // LRM_S4_iLine_Reading
	{LRM_S5_Line_Read_OK_Entry,     LRM_S5_Line_Read_OK_Leave     }, // LRM_S5_Line_Read_OK
	{NULL,                          NULL                          }, // LRM_S6_Line_Read_LAST
	{LRM_S7_Line_Read_NG_Entry,     LRM_S7_Line_Read_NG_Leave     }, // LRM_S7_Line_Read_NG
	{LRM_S8_Line_Read_EOF_Entry,    LRM_S8_Line_Read_EOF_Leave    }, // LRM_S8_Line_Read_EOF
	{LRM_FileClose,                 NULL                          }, // LRM_S9_Exit
};

LRM_t Lrm_StateChange(LRM_t nextState, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	int retVal = 0;
	State_CB leaveCB = gState_CB_Table[gLrm].leave;
	State_CB entryCB = gState_CB_Table[nextState].entry;

	if (NULL != leaveCB) {
		retVal = (*leaveCB)(nextState, evtPara);
	}

	if (NULL != entryCB) {
		retVal = (*entryCB)(nextState, evtPara);
	}

	switch (nextState) {
		case LRM_S1_iFile_Opening: {
			switch (retVal) {
				case LRM_FileOpen_OK:
					gLrm = nextState;
					return Lrm_StateChange(LRM_S2_File_Open_OK, evtPara);

				case LRM_FileOpen_NG:
					gLrm = nextState;
					return Lrm_StateChange(LRM_S3_File_Open_NG, evtPara);

			}
		} break;

		case LRM_S4_iLine_Reading: {
			switch (retVal) {
				case LRM_LineRead_OK:
					gLrm = nextState;
					return Lrm_StateChange(LRM_S5_Line_Read_OK, evtPara);

				case LRM_LineRead_EOF:
					gLrm = nextState;
					return Lrm_StateChange(LRM_S6_Line_Read_LAST, evtPara);

				case LRM_LineRead_NG:
					gLrm = nextState;
					return Lrm_StateChange(LRM_S7_Line_Read_NG, evtPara);

			}
		} break;

		default:
			break;
	}

	return nextState;
}

Event_CB gGlobalEvent_CB_List[] = {
	NULL,
	LRM_EVT_E4_TestGEvent_2_Function,
	LRM_SetInputFile,
	LRM_GetLineObj,
};

LRM_t Lrm_StateMachine(LRM_EVT_t event, LRM_EVT_EvtPara_t *evtPara /* = NULL */)
{
	switch (event) {
		case LRM_EVT_E3_TestGEvent_1:
		case LRM_EVT_E4_TestGEvent_2:
		case LRM_EVT_E5_SetInputFile:
		case LRM_EVT_E6_GetLineObj:
			if (NULL != gGlobalEvent_CB_List[event-3]) {
				return (*(gGlobalEvent_CB_List[event-3]))(gLrm, evtPara);
			} else {
				return gLrm;
			} break;

		default:
			break;
	}

	Event_Pass_Type_t passType;
	LRM_t nextState;

	do {
		passType  = gSE_Table[gLrm].cell[event].passType;
		nextState = gSE_Table[gLrm].cell[event].nextState;

		if (NullPass == passType)
			break;

		gLrm = Lrm_StateChange(nextState);
	} while (RepeatPass == passType);

	return gLrm;
}


#ifndef _LINEREADMACHINE_HPP_
#define _LINEREADMACHINE_HPP_

#include <stdlib.h>

#include "Everything_App.hpp"

typedef enum {
	LRM_EVT_E0_OpenFile     =  0, // 0x00
	LRM_EVT_E1_ReadLine     =  1, // 0x01
	LRM_EVT_E2_Exit         =  2, // 0x02
	LRM_EVT_E3_TestGEvent_1 =  3, // 0x03
	LRM_EVT_E4_TestGEvent_2 =  4, // 0x04
	LRM_EVT_E5_SetInputFile =  5, // 0x05
	LRM_EVT_E6_GetLineObj   =  6, // 0x06
} LRM_EVT_t;

typedef enum {
	LRM_S0_Init           =  0, // 0x00
	LRM_S1_iFile_Opening  =  1, // 0x01
	LRM_S2_File_Open_OK   =  2, // 0x02
	LRM_S3_File_Open_NG   =  3, // 0x03
	LRM_S4_iLine_Reading  =  4, // 0x04
	LRM_S5_Line_Read_OK   =  5, // 0x05
	LRM_S6_Line_Read_LAST =  6, // 0x06
	LRM_S7_Line_Read_NG   =  7, // 0x07
	LRM_S8_Line_Read_EOF  =  8, // 0x08
	LRM_S9_Exit           =  9, // 0x09

	LRM_DUMMY,
} LRM_t;

typedef struct {
	std::string inputFileName;
	LibFileIoClass *lineObj;
} LRM_EVT_EvtPara_t;

LRM_t Lrm_StateChange(LRM_t nextState, LRM_EVT_EvtPara_t *evtPara = NULL);

LRM_t Lrm_StateMachine(LRM_EVT_t event, LRM_EVT_EvtPara_t *evtPara = NULL);
typedef enum {
	SimpleState,
	InterState,
} State_Type_t;
typedef enum {
	DirectPass,
	RepeatPass,
	NullPass,
} Event_Pass_Type_t;
typedef struct {
	Event_Pass_Type_t passType;
	LRM_t nextState;
} State_Cell_t;
typedef struct {
	State_Type_t stateType;
	State_Cell_t cell[3];
} State_Row_t;

int LRM_S2_File_Open_OK_Entry(LRM_t oldState, LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_S2_File_Open_OK_Leave(LRM_t newState, LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_S3_File_Open_NG_Entry(LRM_t oldState, LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_S3_File_Open_NG_Leave(LRM_t newState, LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_S4_iLine_Reading_Leave(LRM_t newState, LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_S5_Line_Read_OK_Entry(LRM_t oldState, LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_S5_Line_Read_OK_Leave(LRM_t newState, LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_S7_Line_Read_NG_Entry(LRM_t oldState, LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_S7_Line_Read_NG_Leave(LRM_t newState, LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_S8_Line_Read_EOF_Entry(LRM_t oldState, LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_S8_Line_Read_EOF_Leave(LRM_t newState, LRM_EVT_EvtPara_t *evtPara = NULL);

typedef int (*State_CB)(LRM_t state, LRM_EVT_EvtPara_t *evtPara);
typedef struct {
	State_CB entry;
	State_CB leave;
} State_CB_t;


LRM_t LRM_EVT_E4_TestGEvent_2_Function(LRM_t &state, LRM_EVT_EvtPara_t *evtPara = NULL);
typedef LRM_t (*Event_CB)(LRM_t &state, LRM_EVT_EvtPara_t *evtPara);

#include "LRM.hpp"


#endif //_LINEREADMACHINE_HPP_



#ifndef _LINEREADMACHINE_HPP_

#include <stdlib.h>
#include "Everything_App.hpp"

typedef enum {
	LRM_EVT_E0_OpenFile     =  0, // 0x00
	LRM_EVT_E1_ReadLine     =  1, // 0x01
	LRM_EVT_E2_Exit         =  2, // 0x02
	LRM_EVT_E3_GetLineObj   =  3, // 0x03
	LRM_EVT_E4_SetInputFile =  4, // 0x04
} LRM_EVT_t;

typedef enum {
	LRM_S0_Init        =  0, // 0x00, INIT STATE
	LRM_S1_File_Opened =  1, // 0x01, 
	LRM_S2_Line_Read   =  2, // 0x02, 
	LRM_S3_Exit        =  3, // 0x03, 
} LRM_t;

typedef struct {
	std::string inputFileName;
	LibFileIoClass *lineObj;
} LRM_EVT_EvtPara_t;

//ERROR CODE
#define LRM_EC_No_Error          (  0 ) // 0x00
#define LRM_EC_File_Open_Failed  (  1 ) // 0x01
#define LRM_EC_End_Of_File       (  2 ) // 0x02

int LRM_StateChange(LRM_t nextState, LRM_EVT_EvtPara_t *evtPara = NULL);

int LRM_StateMachine(LRM_EVT_t event, LRM_EVT_EvtPara_t *evtPara = NULL);

int LRM_S0_Init_Function(LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_S1_File_Opened_Function(LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_S2_Line_Read_Function(LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_S3_Exit_Function(LRM_EVT_EvtPara_t *evtPara = NULL);

int LRM_EVT_E3_GetLineObj_Function(LRM_t &state, LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_EVT_E4_SetInputFile_Function(LRM_t &state, LRM_EVT_EvtPara_t *evtPara = NULL);

#define _LINEREADMACHINE_HPP_
#endif //_LINEREADMACHINE_HPP_


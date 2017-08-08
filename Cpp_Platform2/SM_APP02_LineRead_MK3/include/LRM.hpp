
#ifndef _LRM_HPP_
#define _LRM_HPP_

typedef enum {
	LRM_FileOpen_OK,
	LRM_FileOpen_NG,
	LRM_LineRead_OK,
	LRM_LineRead_EOF,
	LRM_LineRead_NG,
} LRM_Return_Code_t;

int LRM_FileOpen(LRM_t state, LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_LineRead(LRM_t state, LRM_EVT_EvtPara_t *evtPara = NULL);
int LRM_FileClose(LRM_t state, LRM_EVT_EvtPara_t *evtPara = NULL);
LRM_t LRM_SetInputFile(LRM_t &state, LRM_EVT_EvtPara_t *evtPara = NULL);
LRM_t LRM_GetLineObj(LRM_t &state, LRM_EVT_EvtPara_t *evtPara = NULL);

#endif //_LRM_HPP_


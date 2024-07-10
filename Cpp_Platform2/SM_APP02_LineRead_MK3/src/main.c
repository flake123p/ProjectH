
#include "Everything_App.hpp"

#include "LineReadMachine.hpp"

int main(int argc, char *argv[])
{
	LRM_EVT_EvtPara_t evtPara = {"zzTest_input.txt"};

	EXIT_CHK( rc, Lrm_StateMachine(LRM_EVT_E5_SetInputFile, &evtPara) );
	EXIT_CHK( rc, Lrm_StateMachine(LRM_EVT_E6_GetLineObj, &evtPara) );

	LRM_t st = Lrm_StateMachine(LRM_EVT_E1_ReadLine);
	DUMPD(st);
	DUMPS((*(evtPara.lineObj)).lineStr);

	while (LRM_S6_Line_Read_LAST != Lrm_StateMachine(LRM_EVT_E1_ReadLine)) {
		DUMPS((*(evtPara.lineObj)).lineStr);
	}

	Lrm_StateMachine(LRM_EVT_E2_Exit);

	while (LRM_S8_Line_Read_EOF != Lrm_StateMachine(LRM_EVT_E1_ReadLine)) {
		DUMPS((*(evtPara.lineObj)).lineStr);
	}
	
	return 0;
}

